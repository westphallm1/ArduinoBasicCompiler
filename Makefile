var_types.o: var_types.c var_types.h
	gcc -c var_types.h var_types.c

parse_set.o: parse_set.c parse_set.h
	gcc -c parse_set.c parse_set.h


abc: abc.c parse_set.h var_types.h parse_set.o var_types.o
	gcc -o abc parse_set.h var_types.h abc.c parse_set.o var_types.o
