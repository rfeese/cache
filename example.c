/*
 * Copyright 2024 Roger Feese
 */
#include <stdio.h>
#include <stdlib.h>
#include "src/Cache.h"
#include "Foo.h"

int main(int argc, char* argv[]){

	Cache *c = Cache_new(80000);


	// demonstrate loading intsances of Foo
#define FOONUM 42
	Foo *foos[FOONUM];
	for(int i = 0; i < FOONUM; i++){
		char fooname[32] = {};
		snprintf(fooname, 32, "foofile%d", i);
		foos[i] = Foo_cache_load_with_scope(c, fooname, CSCOPE_LEVEL);
	}

	printf("%d Foos loaded.\n", FOONUM);
	printf("There are %d entries in the cache.\n", Cache_get_num_entries(c));
	printf("Cache bucket utilization: %0.4f.\n", Cache_bucket_utilization(c));
	printf("Cache average entry depth: %0.4f.\n\n", Cache_average_entry_depth(c));

	// loading a file already in the cache should be faster
	Foo *foo1_copy = Foo_cache_load(c, "foofile1");
	printf("Duplicate Foo loaded.\n");
	printf("There are %d entries in the cache.\n", Cache_get_num_entries(c));
	printf("Cache bucket utilization: %0.4f.\n", Cache_bucket_utilization(c));
	printf("Cache average entry depth: %0.4f.\n\n", Cache_average_entry_depth(c));

	// demonstrate use of scopes
	int scopedfoostart = 50;
#define SCOPEDFOONUM 40
	Foo *scopedfoos[SCOPEDFOONUM];
	for(int i = 0; i < SCOPEDFOONUM; i++){
		char fooname[32] = {};
		snprintf(fooname, 32, "foofile%d", scopedfoostart + i);
		scopedfoos[i] = Foo_cache_load_with_scope(c, fooname, CSCOPE_LEVEL);
	}

	printf("%d Level-Scoped Foos loaded.\n", SCOPEDFOONUM);
	printf("There are %d entries in the cache.\n", Cache_get_num_entries(c));
	printf("Cache bucket utilization: %0.4f.\n", Cache_bucket_utilization(c));
	printf("Cache average entry depth: %0.4f.\n\n", Cache_average_entry_depth(c));

	Cache_clean_with_scope(c, CSCOPE_LEVEL);

	printf("There are %d entries in the cache after Level-Scope clean.\n", Cache_get_num_entries(c));
	printf("Cache bucket utilization: %0.4f.\n", Cache_bucket_utilization(c));
	printf("Cache average entry depth: %0.4f.\n\n", Cache_average_entry_depth(c));

	Foo *tmpfoo = NULL;
	char tmpfoofilename[16] = {};
	for(int i = 100; i < 50000; i++){
		snprintf(tmpfoofilename, 16, "foofile%d", i);
		tmpfoo = Foo_cache_load_with_scope(c, tmpfoofilename, CSCOPE_LEVEL);
	}

	printf("Loaded 49900 more foos.\n");
	printf("There are %d entries in the cache now.\n", Cache_get_num_entries(c));
	printf("Cache bucket utilization: %0.4f.\n", Cache_bucket_utilization(c));
	printf("Cache average entry depth: %0.4f.\n\n", Cache_average_entry_depth(c));

	Cache_destroy(c);
	return EXIT_SUCCESS;
}
