/*The Arduino BASIC Compiler v0.1 */
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"parse_set.h"
#include"parse_print.h"
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


void direct_line();
void compute_data_size();

void find_variables(char ** save);
//these go in the function table
void parse_cat(char **save);
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

int main(int argc, char * argv[]){
    FILE * fp = fopen(argv[1],"rb");
    short i=0;
    while(fgets(BUFFR,80,fp)!=NULL){
        printf("%d: ",i++);
        direct_line(); 
    }
}

void direct_line(){
    if(BUFFR[0]=='\0'||BUFFR[0]=='\n'){
        printf("\n");
        return;
    }
    int i=0;
    //one liner to strtok_r the whole string
    char * save = NULL;
    //the first command is always seperated from the rest by a space
    char * first_cmd = strtok_r(BUFFR," \n",&save);
    for(i=0;i<12;i++){
        if(!strcmp(first_cmd,command_names[i])){
            printf("%s\n",command_names[i]);
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

void parse_input(char **save){
    find_variables(save);
};

void parse_exit(char **save){
    printf("exit\n");
};

void parse_if(char **save){
    find_variables(save);
};

void parse_endif(char **save){
};

void parse_for(char **save){
};

void parse_endfor(char **save){
};
