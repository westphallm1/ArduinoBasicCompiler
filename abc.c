/*The Arduino BASIC Compiler v0.1 */
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"parse_set.h"
#include"var_types.h"

const char * command_names [] = {
    "int","string","set","cat","print","input","exit",
    "if","endif","for","endfor"
};

char BUFFR[80];
//code starts after data
short DATA_SIZE=0;
//string data starts after shorteger data
short STR_OFFSET=0;
short PROG_CTR;


void print_my_ints();
void print_my_strings();
void direct_line();
void compute_data_size();

void find_variables(char ** save);
//these go in the function table
void parse_cat(char **save);
void parse_print(char **save);
void parse_input(char **save);
void parse_exit(char **save);
void parse_if(char **save);
void parse_endif(char **save);
void parse_for(char **save);
void parse_endfor(char **save);

void (*parser_table[])(char **) = {
     declare_integers,
     declare_strings,
     parse_set,
     parse_cat,
     parse_print,
     parse_input,
     parse_exit,
     parse_if,
     parse_endif,
     parse_for,
     parse_endfor
};

int main(){
    FILE * fp = fopen("foo.py","r");
    short i=0;
    while(fgets(BUFFR,80,fp)!=NULL){
        printf("Tokenizing line %d:\n",++i);
        direct_line(); 
    }
}

void print_my_ints(){
     struct _int_name_node * curr = int_names.next;
     while(curr){
        printf("%s: %d\n",curr->name,curr->val);
        curr=curr->next;
     }
}

void print_my_strings(){
    struct _string_name_node * curr = string_names.next;
    while(curr){
        printf("%s(%d): \"%s\"\n",curr->name,curr->len,
                (curr->init)?curr->init:"Uninitialized");
        curr=curr->next;
    }
}

void direct_line(){
      int i=0;
      //one liner to strtok_r the whole string
      char * save;
      //the first command is always seperated from the rest by a space
      char * first_cmd = strtok_r(BUFFR," ",&save);
      for(i=0;i<12;i++){
        if(!strcmp(first_cmd,command_names[i])){
            parser_table[i](&save);
            break;
        }
        if(i==3 && !DATA_SIZE) compute_data_size();
      }
}


void compute_data_size(){
    if(DATA_SIZE)
        return;//we've already calculated data size
    struct _int_name_node * curr_i=int_names.next;
    int offset = 0;
    while(curr_i){
        offset+=2;
        curr_i=curr_i->next;
    }
    printf("Integer space used: %d\n",offset);
    STR_OFFSET=offset;

    struct _string_name_node* curr_s=string_names.next;
    while(curr_s){
        offset += curr_s->len;
        curr_s=curr_s->next;
    }
    DATA_SIZE=offset;
    PROG_CTR=offset;
    printf("Total data size: %d\n",DATA_SIZE);
}


void find_variables(char ** save){
    char * name_buffr;
    struct _int_name_node * curr_i;
    struct _string_name_node * curr_s; 
    while((name_buffr = strtok_r(NULL," (),\n",save))!=NULL){
        //search through the integer variables
        int addr;
        if((addr=is_int_var(name_buffr))!=-1){
            printf("int %s (@ %#04x), ",name_buffr,addr);
        }else if((addr=is_string_var(name_buffr))!=-1){
            printf("string %s (@ %#04x), ",name_buffr,STR_OFFSET+addr);
        }
    }
    printf("\n");
}



void parse_cat(char **save){
    find_variables(save);
};

void parse_print(char **save){
    find_variables(save);
};

void parse_input(char **save){
    find_variables(save);
};

void parse_exit(char **save){
    find_variables(save);
};

void parse_if(char **save){
    find_variables(save);
};

void parse_endif(char **save){
    find_variables(save);
};
void parse_for(char **save){
    find_variables(save);
};

void parse_endfor(char **save){
    find_variables(save);
};
