#include <stdlib.h>
#include "Bar.h"

void *bar_create(CacheEntry *entry);
int bar_load_from_file(Cache *cache, void *item, const char *filename, int scope);
void bar_destroy(Cache *cache, void *item, int update_subitem_refs);
void bar_update_refs(Cache *cache, void *item, int change, int sope);

void *bar_create(CacheEntry *entry){
	Bar *newbar = malloc(sizeof(Bar));
	newbar->centry = entry;
	return newbar;
}

int bar_load(Cache *cache, void *item, const char *filename, int scope){
	// routine for loading bar content
	Bar *bar = item;
	bar->property1 = 1;
	return 1;
}

void bar_destroy(Cache *cache, void *item, int update_subitem_refs){
	if(update_subitem_refs){
		bar_update_refs(cache, item, -1, CSCOPE_UNSPECIFIED);
	}
	free(item);
}

void bar_update_refs(Cache *cache, void *item, int change, int scope){
	// if bar had sub-item references, we would update them here.
}

Bar *Bar_cache_load(Cache *cache, const char *filename){

	Bar *b = Cache_load(
			cache,
			filename,
			bar_create,
			bar_load,
			bar_destroy,
			bar_update_refs);
	return b;
}


Bar *Bar_cache_load_with_scope(Cache *cache, const char *filename, int scope){

	Bar *b = Cache_load_with_scope(
			cache,
			filename,
			scope,
			bar_create,
			bar_load,
			bar_destroy,
			bar_update_refs);
	return b;
}
