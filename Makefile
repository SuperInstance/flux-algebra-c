CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -Iinclude

.PHONY: all test clean

all: build/flux_algebra.a

build:
	mkdir -p build

build/flux_algebra.o: src/flux_algebra.c include/flux_algebra.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build/flux_algebra.a: build/flux_algebra.o
	ar rcs $@ $<

build/test_flux_algebra: build/flux_algebra.o tests/test_flux_algebra.c
	$(CC) $(CFLAGS) build/flux_algebra.o tests/test_flux_algebra.c -lm -o $@

test: build/test_flux_algebra
	./build/test_flux_algebra

clean:
	rm -rf build
