# Example of a makefile with multiple steps to build the final target

bake: bake.o
	cc -o bake bake.o

bake.o: bake.c
	cc -o bake.o -c bake.c -Wall -Wextra -pedantic -g

# Test case with stuff that doesn't exist
bad: things.c stuff.c crap.c
	cc -o bad things.c stuff.c crap.c
