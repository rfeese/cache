.PHONY: install clean

libcache.a: cache.o
	ar cr $@ cache.o

install: libcache.a cache.h
	cp libcache.a $(DESTDIR)/lib/
	cp cache.h $(DESTDIR)/include/

clean:
	- rm *.a
	- rm *.o
