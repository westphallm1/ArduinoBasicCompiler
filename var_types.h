#ifndef VAR_TYPES_H
#define VAR_TYPES_H
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
struct _int_name_node{
    char name[10];
    short val;
    struct _int_name_node * next;
};

struct _string_name_node{
    char name[10];
    char * init; //we need to malloc it
    short len;
    struct _string_name_node * next;
};

struct _int_name_node int_names;
struct _int_name_node * int_tail = &int_names;
struct _string_name_node string_names;
struct _string_name_node * string_tail= &string_names;

short is_int_var(char * name);
short is_string_var(char * name);
void declare_integers(char ** save);
void declare_strings(char ** save);
#endif
