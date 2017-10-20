/*The Arduino BASIC Compiler v0.1 */
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

const char * command_names [] = {
    "int","string","set","cat","print","input","exit",
    "if","endif","for","endfor"
};

struct _int_name_node{
    char name[10];
    int val;
    struct _int_name_node * next;
};

struct _string_name_node{
    char name[10];
    char * init; //we need to malloc it
    int len;
    struct _string_name_node * next;
};

struct _int_name_node int_names;
struct _int_name_node * int_tail = &int_names;
struct _string_name_node string_names;
struct _string_name_node * string_tail= &string_names;
char BUFFR[80];
//code starts after data
int DATA_SIZE=0;
//string data starts after integer data
int STR_OFFSET=0;
int PROG_CTR;


void print_my_ints();
void print_my_strings();
void direct_line();
void compute_data_size();
int is_int_var(char * name);
int is_string_var(char * name);

void find_variables(char ** save);
//these go in the function table
void declare_integers(char ** save);
void declare_strings(char ** save);
void parse_set(char **save);
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
    int i=0;
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

/* checks if a symbol is in the integer variable table
 * if it is, return its offset from the start of the table.
 * otherwise, return -1
 */
int is_int_var(char * name){
    struct _int_name_node * curr_i=int_names.next;
    int offset = 0;
    while(curr_i){
        if(!strcmp(curr_i->name,name)){
            return offset;
        }
        curr_i=curr_i->next;
        offset+=2;
    }
    return -1;
        
}

/* list is_int_var, but for strings */
int is_string_var(char * name){
    struct _string_name_node* curr_s=string_names.next;
    int offset = 0;
    while(curr_s){
        if(!strcmp(curr_s->name,name)){
            return offset;
        }
        offset += curr_s->len;
        curr_s=curr_s->next;
    }
    return -1;
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

/*
 * Find all the integer variables on the line and add them to the symbol table
 */
void declare_integers(char ** save){
    char * name_buffr;
    struct _int_name_node * new_node=NULL;
    printf("Tokenizing Integers\n"); 
    while((name_buffr=strtok_r(NULL," ,\n",save))!=NULL){
        if(*name_buffr=='='){
            //we've found an '=' symbol
            //initialize our variable to the atoi'd next token
            name_buffr=strtok_r(NULL," ,",save);
            new_node->val=atoi(name_buffr);
            continue;
        }
        //append the new node to the end of the list
        if(new_node){
            int_tail->next=new_node;
            int_tail=new_node;
        }
        
        //setup the node for the next variable
        new_node = malloc(sizeof(struct _int_name_node));
        new_node->val=0;
        new_node->next=NULL;
        strcpy(new_node->name,name_buffr);
        int len = strlen(name_buffr);
    }
    if(new_node){
        int_tail->next=new_node;
        int_tail=new_node;
    }
    printf("\n");
};

/*
 * Find all the string variables on the line and add them to the symbol table
 */
void declare_strings(char ** save){
    printf("Tokenizing Strings\n"); 
    int i = 0;
    char * name_buffr;
    struct _string_name_node * new_node=NULL;
    while((name_buffr=strtok_r(NULL," (),\n",save))!=NULL){
        if(*name_buffr=='='){
            name_buffr=strtok_r(NULL,"\"",save);
            new_node->init=malloc(new_node->len);
            strcpy(new_node->init,name_buffr);
            continue;
        }
        if(i%2==0 && *name_buffr!='\n'){
            //stick the current declared string at the end of the stack
            //and make a new one
            if(new_node){
                string_tail->next=new_node;
                string_tail=new_node;
            }
            //setup the new end of the list
            new_node = malloc(sizeof(struct _string_name_node));
            new_node->init=NULL;
            new_node->next=NULL;
            strcpy(new_node->name,name_buffr);
            int len = strlen(name_buffr);
            //if(name_buffr[len-1]=='\n')
            //    new_node->name[len-1]='\0';

        }else if (i%2==1){
            new_node->len = atoi(name_buffr);
        }
        i++;
    }
    if(new_node){
        string_tail->next=new_node;
        string_tail=new_node;
    }
    printf("\n");
};

void parse_set(char ** save){
    printf("Parsing set line.\n");
    find_variables(save);
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
