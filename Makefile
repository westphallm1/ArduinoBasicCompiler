.DEFAULT_GOAL = abc
var_types.o: var_types.c var_types.h
	gcc -c var_types.h var_types.c

parse_set.o: parse_set.c parse_set.h
	gcc -c parse_set.c parse_set.h

parse_print.o: parse_print.c parse_print.h
	gcc -c parse_print.c parse_print.h

abc: abc.c parse_set.o var_types.o parse_print.o
	gcc -o abc abc.c parse_set.o var_types.o parse_print.o

debug: abc.c parse_set.o var_types.o parse_print.o
	gcc -g -o abc abc.c parse_set.o var_types.o parse_print.o && gdb ./abc

