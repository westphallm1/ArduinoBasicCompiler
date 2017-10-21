#include"parse_set.h"

void parse_set(char ** save){
    printf("Parsing set line.\n");
    char * name_buffr;
    short idx=0;
    short dest_addr,src1_addr,src2_addr;
    char opp;
    while((name_buffr=strtok_r(NULL," =,\n",save))!=NULL){
        //find the first symbol, assert that it's an integer variable
        switch(idx){
            case 0:
                dest_addr = is_int_var(name_buffr);
                if(dest_addr==-1){
                    printf("Invalid Syntax in set instruction.\n");
                }
                //we need to bit-shift 
                //addresses have 0 LSB, literals have 1
                dest_addr<<=1;
                break;
            case 1:
                src1_addr = is_int_var(name_buffr);
                if(src1_addr==-1){
                    //if it's not a variable, it's a literal
                    src1_addr = atoi(name_buffr);
                    src1_addr <<=1;
                    src1_addr |= 1;

                }else{
                    src1_addr <<=1;
                }
                break;
            case 2:
                //second should be the operator
                opp = name_buffr[0];
                break;
            case 3:
                src2_addr = is_int_var(name_buffr);
                if(src2_addr==-1){
                    //if it's not a variable, it's a literal
                    src2_addr = atoi(name_buffr);
                    src2_addr <<=1;
                    src2_addr |= 1;

                }else{
                    src2_addr <<=1;
                }
                break;
        }
        idx+=1;
    }
}
