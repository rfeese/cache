#ifndef BAR_H
#define BAR_H
#include "src/Cache.h"

// Bar objects are cacheable
typedef struct Bar {
	int property1;
	CacheEntry *centry; // bar references its own CacheEntry in order to be able to call update_refs.
} Bar;

extern Bar *Bar_cache_load(Cache *cache, const char *filename);
extern Bar *Bar_cache_load_with_scope(Cache *cache, const char *filename, int scope);

CACHEENTRY_CREATE(bar_create);
CACHEENTRY_ITEM_LOAD(bar_load);
CACHEENTRY_DESTROY(bar_destroy);
CACHEENTRY_UPDATE_REFS(bar_update_refs);

#endif // BAR_H
