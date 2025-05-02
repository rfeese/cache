#include <stdlib.h>
#include <stdio.h>
#include "Bar.h"

static const CacheEntryVTable bar_vtable = {
	.create = &bar_create,
	.item_load = &bar_load,
	.destroy = &bar_destroy,
	.update_refs = &bar_update_refs
};

CACHEENTRY_CREATE(bar_create){
	Bar *newbar = malloc(sizeof(Bar));
	newbar->centry = entry;
	return newbar;
}

CACHEENTRY_ITEM_LOAD(bar_load){
	// routine for loading bar content
	Bar *bar = item;
	bar->property1 = 1;
	return 1;
}

CACHEENTRY_DESTROY(bar_destroy){
	if(update_subitem_refs){
		bar_update_refs(cache, item, -1, CSCOPE_UNSPECIFIED);
	}
	/*
	Bar *bar = item;
	printf("Destroying Bar %s.\n", bar->centry->filename);
	*/
	free(item);
}

CACHEENTRY_UPDATE_REFS(bar_update_refs){
	// if bar had sub-item references, we would update them here.
}

Bar *Bar_cache_load(Cache *cache, const char *filename){

	Bar *b = Cache_load(
			cache,
			filename,
			&bar_vtable);
	return b;
}


Bar *Bar_cache_load_with_scope(Cache *cache, const char *filename, int scope){

	Bar *b = Cache_load_with_scope(
			cache,
			filename,
			scope,
			&bar_vtable);
	return b;
}
