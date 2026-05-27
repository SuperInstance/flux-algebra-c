CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
AR = ar

SRC = src/flux_algebra.c
OBJ = $(SRC:.c=.o)
LIB = libflux_algebra.a
HDR = include/flux_algebra.h

.PHONY: all lib test clean

all: lib

lib: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

src/%.o: src/%.c $(HDR)
	$(CC) $(CFLAGS) -Iinclude -c -o $@ $<

test: $(LIB) tests/test_flux_algebra.c $(HDR)
	$(CC) $(CFLAGS) -Iinclude -o test_flux_algebra tests/test_flux_algebra.c $(LIB) -lm
	./test_flux_algebra

clean:
	rm -f src/*.o $(LIB) test_flux_algebra
