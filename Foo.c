#include <stdlib.h>
#include <stdio.h>
#include "Foo.h"
#include "Bar.h"

CACHEENTRY_CREATE(foo_create);
CACHEENTRY_ITEM_LOAD(foo_load);
CACHEENTRY_DESTROY(foo_destroy);
CACHEENTRY_UPDATE_REFS(foo_update_refs);

static const CacheEntryVTable foo_vtable = {
	.create = 	&foo_create,
	.item_load = 	&foo_load,
	.destroy = 	&foo_destroy,
	.update_refs = 	&foo_update_refs
};

CACHEENTRY_CREATE(foo_create){
	Foo *newfoo = malloc(sizeof(Foo));
	newfoo->centry = entry;
	return newfoo;
}

CACHEENTRY_ITEM_LOAD(foo_load){
	// routine for loading foo content
	Foo *f = item;
	f->property1 = 1;
	char barname[32] = {};
	snprintf(barname, 32, "bar-%s", filename);
	f->bar = Bar_cache_load_with_scope(cache, barname, scope);
	return f->bar ? 1:0;
}

CACHEENTRY_DESTROY(foo_destroy){
	if(update_subitem_refs){
		foo_update_refs(cache, item, -1, CSCOPE_UNSPECIFIED);
	}
	free(item);
}

CACHEENTRY_UPDATE_REFS(foo_update_refs){
	// update foo sub-item references
	Foo *f = item;
	bar_update_refs(cache, f->bar, change, scope);
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
