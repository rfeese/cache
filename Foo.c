#include <stdlib.h>
#include "Foo.h"
#include "Bar.h"

void *foo_create(CacheEntry *entry);
int foo_load(Cache *cache, void *item, const char *filename, int scope);
void foo_destroy(Cache *cache, void *item, int update_subitem_refs);
void foo_update_refs(Cache *cache, void *item, int change, int sope);

static const CacheEntryVTable foo_vtable = {
	.create = foo_create,
	.item_load = foo_load,
	.destroy = foo_destroy,
	.update_refs = foo_update_refs
};

void *foo_create(CacheEntry *entry){
	Foo *newfoo = malloc(sizeof(Foo));
	newfoo->centry = entry;
	return newfoo;
}

int foo_load(Cache *cache, void *item, const char *filename, int scope){
	// routine for loading foo content
	Foo *f = item;
	f->property1 = 1;
	f->bar = Bar_cache_load_with_scope(cache, "bar1", scope);
	return f->bar ? 1:0;
}

void foo_destroy(Cache *cache, void *item, int update_subitem_refs){
	if(update_subitem_refs){
		foo_update_refs(cache, item, -1, CSCOPE_UNSPECIFIED);
	}
	free(item);
}

void foo_update_refs(Cache *cache, void *item, int change, int scope){
	// if foo had sub-item references, we would update them here.
}

Foo *Foo_cache_load(Cache *cache, const char *filename){

	Foo *f = Cache_load(
			cache,
			filename,
			&foo_vtable);
	return f;
}

Foo *Foo_cache_load_with_scope(Cache *cache, const char *filename, int scope){

	Foo *f = Cache_load_with_scope(
			cache,
			filename,
			scope,
			&foo_vtable);
	return f;
}
