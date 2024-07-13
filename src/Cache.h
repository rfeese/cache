/*
 * Copyright 2024 Roger Feese
*/
#ifndef CACHE_H
#define CACHE_H

#define CACHE_FILENAME_MAX	256

/**
 * Cache scope tags in order of priority.
 * X-macro
 */
#define CACHE_SCOPES \
	X(CSCOPE_UNSPECIFIED, "UNSPECIFIED") \
	X(CSCOPE_LEVEL, "LEVEL") \
	X(CSCOPE_GLOBAL, "GLOBAL")

#define X(a, b) a,
enum Cache_scopes {
	CACHE_SCOPES
	NUM_CSCOPES
};
#undef X

#define X(a, b) b,
static char *Cache_scopenames[] = { CACHE_SCOPES };
#undef X
#undef CACHE_SCOPES

/**
 * Private structure for tracking items loaded (from disk) into memory.
 * Items are reference by filename and stored in a hashmap.
 */
typedef struct Cache Cache;

/**
 * An item in the cache with metadata required for cache maintenance.
 * Users should not directly modify values or call the functions.
 */
typedef struct CacheEntry {
	// public
	char filename[CACHE_FILENAME_MAX];
	void *item;
	unsigned int scope; 
	// private
	unsigned int refs; // number of active references based on load calls
	// item maintenance methods
	void (*destroy)(Cache *cache, void *item, int update_subitem_refs);
	void (*update_refs)(Cache *cache, void *item, int change, int scope);
} CacheEntry;

/**
 * Create a new cache.
 *
 * @param size_hint determines address space allocated for the hashmap
 */
extern Cache *Cache_new(int size_hint);

/**
 * @return the number of entries in the cache
 */
int Cache_get_num_entries(Cache *cache);

/**
 * De-allocate all items in the cache
 */
void Cache_purge(Cache *cache);

/**
 * Empty and de-allocate cache
 */
void Cache_destroy(Cache *cache);

/**
 * Load or fetch item from cache if it has already been loaded.
 *
 * @param filename the filename/identifier
 * @param scope tag to be assigned or updated.
 * @param create callback used to allocate memory for the item
 * @param load callback for loading the item (presumably from disk). Must return value indicates success (1) or failure (0).
 * @param destroy callback for freeing item from memory and decrementing references on any sub-items.
 * @param update_refs callback for updating reference counts on sub-items.
 */
extern void *Cache_load_with_scope(
		Cache *cache,
		const char *filename,
		unsigned int scope,
		void *(*create)(CacheEntry *entry),
		int (*load)(Cache *cache, void *item, const char *filename, int scope),
		void (*destroy)(Cache *cache, void *item, int update_subitem_refs),
		void (*update_refs)(Cache *cache, void *item, int change, int scope)
);

/**
 * Load or fetch item from cache if it has already been loaded.
 *
 * @param filename the filename/identifier
 * @param create callback used to allocate memory for the item
 * @param load callback for loading the item (presumably from disk). Must return value indicates success (1) or failure (0).
 * @param destroy callback for freeing item from memory and decrementing references on any sub-items.
 * @param update_refs callback for updating reference counts on sub-items.
 */
extern void *Cache_load(
		Cache *cache,
		const char *filename,
		void *(*create)(CacheEntry *entry),
		int (*load)(Cache *cache,void *item, const char *filename, int scope),
		void (*destroy)(Cache *cache, void *item, int update_subitem_refs),
		void (*update_refs)(Cache *cache, void *item, int change, int scope)
);

/**
 * Force remove a specific item from the cache.
 *
 * @param filename the filename/identifier
 */
extern void Cache_remove(Cache *cache, const char *filename);

/**
 * Update reference counts on an item
 *
 * @param entry cache entry to update
 * @param change amount to change the reference count. Typically 1 or -1.
 * @param scope tag to update the entry with
 */
extern void Cache_entry_update_refs(Cache *cache, CacheEntry *entry, int change, int scope);

/**
 * Remove items with the requested scope.
 *
 * @param scope tag used to identify items to remove.
 */
extern void Cache_clean_with_scope(Cache *cache, unsigned int scope);
#endif // CACHE_H
