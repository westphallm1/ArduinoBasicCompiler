#include "parse_print.h"


/* Multiple print commands can be encoded on a single line
 */
void parse_print(char **save){
    char * name_buffr;
    short dest_val;
    while((name_buffr=strtok_r(NULL," =,\n",save))!=NULL){
        //figure out what kind of item we're going to be printing
        if((dest_val=is_int_var(name_buffr))!=-1){
            printf("printd (@ %#04x), ",dest_val);
        } else if ((dest_val=is_string_var(name_buffr))!=-1) {
            printf("prints (@ %#04x), ",dest_val);
        } else {
            //if it's not a variable it's a literal
            printf("printl %s, ",name_buffr);
        };
    } 
    printf("\n");
};
