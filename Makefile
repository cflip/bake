# This is my makefile,
# it has many targets.
#

bake: bake.c
	cc -o bake -Wall -Wextra -pedantic -g bake.c

hello:
	echo "hello, world"
	echo "this line will print if the file hello does not exist."

# Thanks for watching!
