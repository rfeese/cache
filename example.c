/*
 * Copyright 2024 Roger Feese
 */
#include <stdio.h>
#include <stdlib.h>
#include "src/Cache.h"
#include "Foo.h"

int main(int argc, char* argv[]){

	Cache *c = Cache_new(10);

	// demonstrate loading intsances of Foo
	Foo *foo1 = Foo_cache_load(c, "foofile1");
	Foo *foo2 = Foo_cache_load(c, "foofile2");
	Foo *foo3 = Foo_cache_load(c, "foofile3");
	Foo *foo4 = Foo_cache_load(c, "foofile4");
	Foo *foo5 = Foo_cache_load(c, "foofile5");
	Foo *foo6 = Foo_cache_load(c, "foofile6");
	Foo *foo7 = Foo_cache_load(c, "foofile7");
	Foo *foo8 = Foo_cache_load(c, "foofile8");
	Foo *foo9 = Foo_cache_load(c, "foofile9");
	Foo *foo10 = Foo_cache_load(c, "foofile10");
	Foo *foo11 = Foo_cache_load(c, "foofile11");
	Foo *foo12 = Foo_cache_load(c, "foofile12");
	Foo *foo13 = Foo_cache_load(c, "foofile13");
	Foo *foo14 = Foo_cache_load(c, "foofile14");
	Foo *foo15 = Foo_cache_load(c, "foofile15");
	Foo *foo16 = Foo_cache_load(c, "foofile16");
	Foo *foo17 = Foo_cache_load(c, "foofile17");
	Foo *foo18 = Foo_cache_load(c, "foofile18");
	Foo *foo19 = Foo_cache_load(c, "foofile19");
	Foo *foo20 = Foo_cache_load(c, "foofile20");
	Foo *foo21 = Foo_cache_load(c, "foofile21");
	Foo *foo22 = Foo_cache_load(c, "foofile22");
	Foo *foo23 = Foo_cache_load(c, "foofile23");
	Foo *foo24 = Foo_cache_load(c, "foofile24");
	Foo *foo25 = Foo_cache_load(c, "foofile25");
	Foo *foo26 = Foo_cache_load(c, "foofile26");
	Foo *foo27 = Foo_cache_load(c, "foofile27");
	Foo *foo28 = Foo_cache_load(c, "foofile28");
	Foo *foo29 = Foo_cache_load(c, "foofile29");
	Foo *foo30 = Foo_cache_load(c, "foofile30");
	Foo *foo31 = Foo_cache_load(c, "foofile31");
	Foo *foo32 = Foo_cache_load(c, "foofile32");
	Foo *foo33 = Foo_cache_load(c, "foofile33");
	Foo *foo34 = Foo_cache_load(c, "foofile34");
	Foo *foo35 = Foo_cache_load(c, "foofile35");
	Foo *foo36 = Foo_cache_load(c, "foofile36");
	Foo *foo37 = Foo_cache_load(c, "foofile37");
	Foo *foo38 = Foo_cache_load(c, "foofile38");
	Foo *foo39 = Foo_cache_load(c, "foofile39");
	Foo *foo40 = Foo_cache_load(c, "foofile40");
	Foo *foo41 = Foo_cache_load(c, "foofile41");
	Foo *foo42 = Foo_cache_load(c, "foofile42");
	printf("There are %d entries in the cache.\n", Cache_get_num_entries(c));

	// loading a file already in the cache should be faster
	Foo *foo1_copy = Foo_cache_load(c, "foofile1");
	printf("There are %d entries in the cache.\n", Cache_get_num_entries(c));

	// demonstrate use of scopes
	Foo *foo50 = Foo_cache_load_with_scope(c, "foofile50", CSCOPE_LEVEL);
	Foo *foo51 = Foo_cache_load_with_scope(c, "foofile51", CSCOPE_LEVEL);
	Foo *foo52 = Foo_cache_load_with_scope(c, "foofile52", CSCOPE_LEVEL);
	Foo *foo53 = Foo_cache_load_with_scope(c, "foofile53", CSCOPE_LEVEL);
	Foo *foo54 = Foo_cache_load_with_scope(c, "foofile54", CSCOPE_LEVEL);
	Foo *foo55 = Foo_cache_load_with_scope(c, "foofile55", CSCOPE_LEVEL);
	Foo *foo56 = Foo_cache_load_with_scope(c, "foofile56", CSCOPE_LEVEL);
	Foo *foo57 = Foo_cache_load_with_scope(c, "foofile57", CSCOPE_LEVEL);
	Foo *foo58 = Foo_cache_load_with_scope(c, "foofile58", CSCOPE_LEVEL);
	Foo *foo59 = Foo_cache_load_with_scope(c, "foofile59", CSCOPE_LEVEL);
	Foo *foo60 = Foo_cache_load_with_scope(c, "foofile60", CSCOPE_LEVEL);
	Foo *foo61 = Foo_cache_load_with_scope(c, "foofile61", CSCOPE_LEVEL);
	Foo *foo62 = Foo_cache_load_with_scope(c, "foofile62", CSCOPE_LEVEL);
	Foo *foo63 = Foo_cache_load_with_scope(c, "foofile63", CSCOPE_LEVEL);
	Foo *foo64 = Foo_cache_load_with_scope(c, "foofile64", CSCOPE_LEVEL);
	Foo *foo65 = Foo_cache_load_with_scope(c, "foofile65", CSCOPE_LEVEL);
	Foo *foo66 = Foo_cache_load_with_scope(c, "foofile66", CSCOPE_LEVEL);
	Foo *foo67 = Foo_cache_load_with_scope(c, "foofile67", CSCOPE_LEVEL);
	Foo *foo68 = Foo_cache_load_with_scope(c, "foofile68", CSCOPE_LEVEL);
	Foo *foo69 = Foo_cache_load_with_scope(c, "foofile69", CSCOPE_LEVEL);
	Foo *foo70 = Foo_cache_load_with_scope(c, "foofile70", CSCOPE_GLOBAL);
	Foo *foo71 = Foo_cache_load_with_scope(c, "foofile71", CSCOPE_GLOBAL);
	Foo *foo72 = Foo_cache_load_with_scope(c, "foofile72", CSCOPE_GLOBAL);
	Foo *foo73 = Foo_cache_load_with_scope(c, "foofile73", CSCOPE_GLOBAL);
	Foo *foo74 = Foo_cache_load_with_scope(c, "foofile74", CSCOPE_GLOBAL);
	Foo *foo75 = Foo_cache_load_with_scope(c, "foofile75", CSCOPE_GLOBAL);
	Foo *foo76 = Foo_cache_load_with_scope(c, "foofile76", CSCOPE_GLOBAL);
	Foo *foo77 = Foo_cache_load_with_scope(c, "foofile77", CSCOPE_GLOBAL);
	Foo *foo78 = Foo_cache_load_with_scope(c, "foofile78", CSCOPE_GLOBAL);
	Foo *foo79 = Foo_cache_load_with_scope(c, "foofile79", CSCOPE_GLOBAL);
	Foo *foo80 = Foo_cache_load_with_scope(c, "foofile80", CSCOPE_LEVEL);
	Foo *foo81 = Foo_cache_load_with_scope(c, "foofile81", CSCOPE_LEVEL);
	Foo *foo82 = Foo_cache_load_with_scope(c, "foofile82", CSCOPE_LEVEL);
	Foo *foo83 = Foo_cache_load_with_scope(c, "foofile83", CSCOPE_LEVEL);
	Foo *foo84 = Foo_cache_load_with_scope(c, "foofile84", CSCOPE_LEVEL);
	Foo *foo85 = Foo_cache_load_with_scope(c, "foofile85", CSCOPE_LEVEL);
	Foo *foo86 = Foo_cache_load_with_scope(c, "foofile86", CSCOPE_LEVEL);
	Foo *foo87 = Foo_cache_load_with_scope(c, "foofile87", CSCOPE_LEVEL);
	Foo *foo88 = Foo_cache_load_with_scope(c, "foofile88", CSCOPE_LEVEL);
	Foo *foo89 = Foo_cache_load_with_scope(c, "foofile89", CSCOPE_LEVEL);

	printf("There are %d entries in the cache.\n", Cache_get_num_entries(c));

	Cache_clean_with_scope(c, CSCOPE_LEVEL);
	
	printf("There are %d entries in the cache.\n", Cache_get_num_entries(c));

	Cache_destroy(c);
	return EXIT_SUCCESS;
}
