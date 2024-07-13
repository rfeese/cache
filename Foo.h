#ifndef FOO_H
#define FOO_H
#include "src/Cache.h"
#include "Bar.h"

// Foo objects are cacheable
typedef struct Foo {
	int property1;
	Bar *bar; // foo contains sub-item bar
	CacheEntry *centry; // foo references its own CacheEntry in order to be able to call update_refs.
} Foo;

extern Foo *Foo_cache_load(Cache *cache, const char *filename);
extern Foo *Foo_cache_load_with_scope(Cache *cache, const char *filename, int scope);

#endif // FOO_H
