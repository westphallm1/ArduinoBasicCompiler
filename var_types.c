#include"var_types.h"
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
        short len = strlen(name_buffr);
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
    short i = 0;
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

short is_int_var(char * name){
    struct _int_name_node * curr_i=int_names.next;
    short offset = 0;
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
short is_string_var(char * name){
    struct _string_name_node* curr_s=string_names.next;
    short offset = 0;
    while(curr_s){
        if(!strcmp(curr_s->name,name)){
            return offset;
        }
        offset += curr_s->len;
        curr_s=curr_s->next;
    }
    return -1;
}
