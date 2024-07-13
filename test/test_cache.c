#include <string.h>
#include <stdlib.h>
#include "../../Unity/src/unity.h"
#include "../src/Cache.h"

typedef struct myitem {
	CacheEntry *centry;
	int value1;
} myitem;

// cache functions
int _create_called = 0;
void *_create(CacheEntry *entry){
	_create_called = 1;
	myitem *mi = malloc(sizeof(myitem));
	mi->centry = entry;
	return mi;
}

int _load_from_file_called = 0;
int _load_from_file(Cache *cache, void *item, const char *filename, int scope){
	myitem *myitem1 = item;
	myitem1->value1 = 1;
	_load_from_file_called = 1;
	return 1;
}

int _update_refs_called = 0;
void _update_refs(Cache *cache, void *item, int change, int scope){
	_update_refs_called = 1;
}

int _destroy_called = 0;
void _destroy(Cache *cache, void *item, int update_subitem_refs){
	if(update_subitem_refs){
		_update_refs(cache, item, -1, CSCOPE_UNSPECIFIED);
	}
	free(item);
	_destroy_called = 1;
}

// runs before each test
void setUp(void){
	_create_called = 0;
	_load_from_file_called = 0;
	_destroy_called = 0;
	_update_refs_called = 0;
}

//runs after each test
void tearDown(void){
}

void test_Cache_new(){
	Cache *c = Cache_new(10);
	TEST_ASSERT_NOT_NULL_MESSAGE(c, "Cache_new failed");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, Cache_get_num_entries(c), "num entries should be 0");
	Cache_destroy(c);
}

void test_Cache_load_with_scope(){
	Cache *c = Cache_new(10);
	myitem *myitem1 = Cache_load_with_scope(c, "myitem1", CSCOPE_LEVEL, _create, _load_from_file, _destroy, _update_refs);
	TEST_ASSERT_EQUAL_INT_MESSAGE(CSCOPE_LEVEL, myitem1->centry->scope, "entry scope should have been set.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, Cache_get_num_entries(c), "num entries should be 1");
	myitem *myitem1_2 = Cache_load_with_scope(c, "myitem1", CSCOPE_GLOBAL, _create, _load_from_file, _destroy, _update_refs);
	TEST_ASSERT_EQUAL_INT_MESSAGE(CSCOPE_GLOBAL, myitem1->centry->scope, "entry scope should have been updated.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, Cache_get_num_entries(c), "num entries should be 1");
	Cache_destroy(c);
}

void test_Cache_load(){
	Cache *c = Cache_new(10);

	_update_refs_called = 0;
	myitem *myitem1 = Cache_load(c, "myitem1", _create, _load_from_file, _destroy, _update_refs);
	TEST_ASSERT_NOT_NULL_MESSAGE(myitem1, "myitem1 should not be null.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _create_called, "_create should have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _load_from_file_called, "_load_from_file should have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, myitem1->value1, "myitem1->value1 should be 1.");
	TEST_ASSERT_NOT_NULL_MESSAGE(myitem1->centry, "cache entry should not be null.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, myitem1->centry->refs, "cache entry refs should be 1.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, _update_refs_called, "_update_refs should NOT have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, Cache_get_num_entries(c), "num entries should be 1");

	_create_called = 0;
	_load_from_file_called = 0;
	_update_refs_called = 0;
	myitem *myitem1_2 = Cache_load(c, "myitem1", _create, _load_from_file, _destroy, _update_refs);
	TEST_ASSERT_EQUAL_PTR_MESSAGE(myitem1, myitem1_2, "myitem1_2 should reference the same item as myitem1.");
	TEST_ASSERT_NOT_NULL_MESSAGE(myitem1_2, "myitem1_2 should not be null.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, _create_called, "_create should NOT have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, _load_from_file_called, "_load_from_file should NOT have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, myitem1_2->value1, "myitem1_2->value1 should be 1.");
	TEST_ASSERT_NOT_NULL_MESSAGE(myitem1_2->centry, "cache entry should not be null.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(2, myitem1_2->centry->refs, "cache entry refs should be 2.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _update_refs_called, "_update_refs should have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, Cache_get_num_entries(c), "num entries should be 1");

	myitem *myitem2 = Cache_load(c, "myitem2", _create, _load_from_file, _destroy, _update_refs);
	TEST_ASSERT_NOT_NULL_MESSAGE(myitem2, "myitem2 should not be null.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _create_called, "_create should have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _load_from_file_called, "_load_from_file should have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, myitem2->value1, "myitem1_2->value1 should be 1.");
	TEST_ASSERT_NOT_NULL_MESSAGE(myitem2->centry, "cache entry should not be null.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, myitem2->centry->refs, "cache entry refs should be 1.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(2, Cache_get_num_entries(c), "num entries should be 2");
	Cache_destroy(c);
}

void test_Cache_destroy(){
	Cache *c = Cache_new(10);

	myitem *myitem1 = Cache_load(c, "myitem1", _create, _load_from_file, _destroy, _update_refs);
	myitem *myitem2 = Cache_load(c, "myitem2", _create, _load_from_file, _destroy, _update_refs);
	Cache_destroy(c);
}

void test_Cache_remove(){
	Cache *c = Cache_new(10);
	myitem *myitem1 = Cache_load(c, "myitem1", _create, _load_from_file, _destroy, _update_refs);

	// remove non-existant	
	Cache_remove(c, "myitem5");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, _destroy_called, "_destroy should NOT have been called.");

	// remove existant
	Cache_remove(c, "myitem1");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _destroy_called, "_destroy should have been called.");

	// full load should be required after item was previously removed
	_create_called = 0;
	_load_from_file_called = 0;
	myitem1 = Cache_load(c, "myitem1", _create, _load_from_file, _destroy, _update_refs);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _create_called, "_create should have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _load_from_file_called, "_load_from_file should have been called.");
	Cache_destroy(c);
}

void test_Cache_entry_update_refs(){
	Cache *c = Cache_new(10);
	myitem *myitem1 = Cache_load_with_scope(c, "myitem1", CSCOPE_LEVEL, _create, _load_from_file, _destroy, _update_refs);
	Cache_entry_update_refs(c, myitem1->centry, 1, CSCOPE_UNSPECIFIED);
	TEST_ASSERT_EQUAL_INT_MESSAGE(2, myitem1->centry->refs, "refs should be 2.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(CSCOPE_LEVEL, myitem1->centry->scope, "scope should still be CSCOPE_LEVEL");
	Cache_entry_update_refs(c, myitem1->centry, -1, CSCOPE_GLOBAL);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, myitem1->centry->refs, "refs should be 1.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(CSCOPE_GLOBAL, myitem1->centry->scope, "scope should still be CSCOPE_GLOBAL");
	Cache_entry_update_refs(c, myitem1->centry, -1, CSCOPE_UNSPECIFIED);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _destroy_called, "_destroy should have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, Cache_get_num_entries(c), "num entries should be 0");
	Cache_destroy(c);
}

void test_Cache_purge(){
	Cache *c = Cache_new(10);
	myitem *myitem1 = Cache_load(c, "myitem1", _create, _load_from_file, _destroy, _update_refs);
	myitem *myitem2 = Cache_load(c, "myitem2", _create, _load_from_file, _destroy, _update_refs);
	myitem *myitem3 = Cache_load(c, "myitem3", _create, _load_from_file, _destroy, _update_refs);
	Cache_purge(c);
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, Cache_get_num_entries(c), "num entries should be 0");
	Cache_destroy(c);
}

void test_Cache_clean_with_scope(){
	Cache *c = Cache_new(10);
	myitem *myitem1 = Cache_load_with_scope(c, "myitem1", CSCOPE_LEVEL, _create, _load_from_file, _destroy, _update_refs);
	myitem *myitem2 = Cache_load_with_scope(c, "myitem2", CSCOPE_GLOBAL, _create, _load_from_file, _destroy, _update_refs);
	myitem *myitem3 = Cache_load_with_scope(c, "myitem3", CSCOPE_UNSPECIFIED, _create, _load_from_file, _destroy, _update_refs);
	myitem *myitem4 = Cache_load_with_scope(c, "myitem4", CSCOPE_GLOBAL, _create, _load_from_file, _destroy, _update_refs);
	myitem *myitem5 = Cache_load_with_scope(c, "myitem5", CSCOPE_LEVEL, _create, _load_from_file, _destroy, _update_refs);
	Cache_clean_with_scope(c, CSCOPE_LEVEL);
	TEST_ASSERT_EQUAL_INT_MESSAGE(3, Cache_get_num_entries(c), "num entries should be 3");
	Cache_destroy(c);
}

int main(){
	UNITY_BEGIN();
	RUN_TEST(test_Cache_new);
	RUN_TEST(test_Cache_load_with_scope);
	RUN_TEST(test_Cache_load);
	RUN_TEST(test_Cache_destroy);
	RUN_TEST(test_Cache_remove);
	RUN_TEST(test_Cache_entry_update_refs);
	RUN_TEST(test_Cache_purge);
	RUN_TEST(test_Cache_clean_with_scope);

	return UNITY_END();
}
