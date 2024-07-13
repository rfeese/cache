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
#endif // BAR_H
