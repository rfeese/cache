/*
 * Copyright 2024 Roger Feese
*/
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "Cache.h"

// linked list for hashmap buckets
typedef struct BucketList {
	CacheEntry *cache_entry;
	struct BucketList *next;
} BucketList;

/**
 * Private structure for tracking items loaded (from disk) into memory.
 * Items are reference by filename and stored in a hashmap.
 */
struct Cache {
	int size; // size of the hashmap address space
	int num_entries; // number of cache entries
	BucketList **bucket; // array[size] of buckets with linked lists of cache entries

};

Cache *Cache_new(int size_hint){
	static int primes[] = { 509, 509, 1021, 2053, 4093, 8191, 16381, 32771, 65521 };

	int i;
	for(i = 1; primes[i] < size_hint; i++){}
	// primes[i] is first prime larger than size_hint
	// primes[i-1] is number of buckets

	Cache *c;
	// allocate cache and the initial empty buckets
	c = malloc(sizeof(Cache));
	c->size = primes[i-1];
	c->num_entries = 0;

	// initialize the bucket pointers
	c->bucket = malloc(primes[i-1] * sizeof(c->bucket[0]));
	for(int i = 0; i < c->size; i++){
		c->bucket[i] = NULL;
	}
	return c;
}

int Cache_get_num_entries(Cache *cache){
	return cache->num_entries;
}

void Cache_purge(Cache *cache){
	for(int i = 0; i < cache->size; i++){
		while(cache->bucket[i]){
			BucketList *bl = cache->bucket[i];
			cache->bucket[i] = bl->next;
			// purge the item
			bl->cache_entry->destroy(cache, bl->cache_entry->item, 0);
			free(bl->cache_entry);
			free(bl);
		}
	}
	cache->num_entries = 0;
}

void Cache_destroy(Cache *cache){
	// free all items
	Cache_purge(cache);
	free(cache->bucket);
	free(cache);
}

int Cache_hash(const char *filename){
	int hash = 0;
	for(int i = 0; filename[i] && i < CACHE_FILENAME_MAX; i++){
		hash += filename[i] * (i + 1);
	}
	return hash;
}

CacheEntry *Cache_get(Cache *cache, const char *filename){
	// find bucket
	int i = Cache_hash(filename) % cache->size;

	// search for item in list
	BucketList *bl;
	for(bl = cache->bucket[i]; bl; bl = bl->next){
		if(strncmp(filename, bl->cache_entry->filename, CACHE_FILENAME_MAX) == 0)
			break;
	}

	// if bl is not null, item was found
	return bl ? bl->cache_entry : NULL;
}

void Cache_put(Cache *cache, CacheEntry *entry, 
		void (*destroy)(Cache *cache, void *item, int update_subitem_refs),
		void (*update_refs)(Cache *cache, void *item, int change, int scope) ){
	// find bucket
	int i = Cache_hash(entry->filename) % cache->size;

	// search for item in list
	BucketList *bl;
	for(bl = cache->bucket[i]; bl; bl = bl->next){
		if(strncmp(entry->filename, bl->cache_entry->filename, CACHE_FILENAME_MAX) == 0)
			break;
	}

	// if bl is not null, item was found
	if(!bl){
		// insert at head
		entry->refs = 1;
		entry->destroy = destroy;
		entry->update_refs = update_refs;
		BucketList *newbl = malloc(sizeof(BucketList));
		newbl->cache_entry = entry;
		newbl->next = cache->bucket[i];
		cache->bucket[i] = newbl;
		cache->num_entries++;
	}
}

void Cache_entry_update_refs(Cache *cache, CacheEntry *entry, int change, int scope){
	entry->update_refs(cache, entry->item, change, scope);
	entry->refs = entry->refs + change;
	if(entry->refs <= 0){
		Cache_remove(cache, entry->filename);
		return;
	}
	if(entry->scope < scope){
		entry->scope = scope;
	}
}

void Cache_update_refs(Cache *cache, const char *filename, int change, int scope){
	CacheEntry *entry = Cache_get(cache, filename);
	Cache_entry_update_refs(cache, entry, change, scope);
}

void *Cache_load_with_scope(
		Cache *cache,
		const char *filename,
		unsigned int scope,
		void *(*create)(CacheEntry *entry),
		int (*load)(Cache *cache, void *item, const char *filename, int scope),
		void (*destroy)(Cache *cache, void *item, int update_subitem_refs),
		void (*update_refs)(Cache *cache, void *item, int change, int scope)
){
	CacheEntry *entry = NULL;
	// if item already exists in cache, update refs and return it
	if((entry = Cache_get(cache, filename))){
		Cache_entry_update_refs(cache, entry, 1, scope);
		return entry->item;
	}
	// create new instance of item
	entry = malloc(sizeof(CacheEntry));
	void *item = create(entry);
	if(!item){
		free(entry);
		return NULL;
	}
	// load item
	if(load(cache, item, filename, scope)){
		snprintf(entry->filename, CACHE_FILENAME_MAX, "%s", filename);
		entry->item = item;
		entry->scope = scope;
		Cache_put(cache, entry, destroy, update_refs);
		return item;
	}
	// item did not load correctly so destroy it
	else{
		destroy(cache, item, 1);
		free(entry);
	}
	return NULL;
}

void *Cache_load(
		Cache *cache,
		const char *filename,
		void *(*create)(CacheEntry *entry),
		int (*load)(Cache *cache, void *item, const char *filename, int scope),
		void (*destroy)(Cache *cache, void *item, int update_subitem_refs),
		void (*update_refs)(Cache *cache, void *item, int change, int scope)
){
	return Cache_load_with_scope(
		cache,
		filename,
		CSCOPE_UNSPECIFIED,
		create,
		load,
		destroy,
		update_refs);
}

void Cache_remove(Cache *cache, const char *filename){
	// find bucket
	int i = Cache_hash(filename) % cache->size;

	// search for item in list
	BucketList **blp;
	for(blp = &cache->bucket[i]; *blp; blp = &(*blp)->next){
		if(strncmp(filename, (*blp)->cache_entry->filename, CACHE_FILENAME_MAX) == 0)
			break;
	}

	// if *blp is not null, item was found
	if(*blp){
		// parent->next needs to point to blp->next
		BucketList *bl = *blp;
		*blp = bl->next;
		bl->cache_entry->destroy(cache, bl->cache_entry->item, 1);
		free(bl->cache_entry);
		free(bl);
		cache->num_entries--;
	}
}

void Cache_clean_with_scope(Cache *cache, unsigned int scope){
	for(int i = 0; i < cache->size; i++){
		BucketList **blp = &cache->bucket[i];
		while(*blp){
			if((*blp)->cache_entry->scope == scope){
				(*blp)->cache_entry->destroy(cache, (*blp)->cache_entry->item, 1);
				BucketList *oldbl = *blp;
				// modify the list pointer to point to next
				*blp = oldbl->next;
				free(oldbl->cache_entry);
				free(oldbl);
				cache->num_entries--;
			}
			else{
				// move on to the next list item
				blp = &(*blp)->next;
			}
		}
	}
}
