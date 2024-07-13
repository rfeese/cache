SHELL=/bin/sh
CC=$(CROSS)gcc
PKG_CONFIG=$(CROSS)pkg-config
CFLAGS=-g -Wall
UNITY=../../Unity/src/unity.c

.PHONY: all test clean

# default - run tests
all test:  test_cache
	-./test_cache


# build a test
test_cache: $(UNITY) test_cache.c ../src/Cache.h ../src/Cache.c
	$(CC) $(CFLAGS) $(UNITY) test_cache.c ../src/Cache.c -o test_cache

# delete compiled binaries
clean test_clean:
	- rm test_cache
