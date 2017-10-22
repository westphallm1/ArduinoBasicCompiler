#include"parse_set.h"

/*Format: 7 bytes wide
 * 1       2-3     4-5      6-7
 * Instr   Dest    Src1     Src2
 *
 * Different operations (eg +,-,/,*) are encoded as different
 * intructions
 */
void parse_set(char ** save){
    char * name_buffr;
    short idx=0;
    short dest_addr,src1_addr=0,src2_addr=0;
    char opp='+';
    while((name_buffr=strtok_r(NULL," =,\n",save))!=NULL){
        //find the first symbol, assert that it's an integer variable
        switch(idx){
            case 0:
                dest_addr = is_int_var(name_buffr);
                if(dest_addr==-1){
                    printf("Invalid Syntax in set instruction.\n");
                    exit(1);
                }
                //we need to bit-shift 
                //addresses have 1 LSB, literals have 0
                dest_addr<<=1;
                dest_addr|=1;
                break;
            case 1:
                src1_addr = is_int_var(name_buffr);
                if(src1_addr==-1){
                    //if it's not a variable, it's a literal
                    src1_addr = atoi(name_buffr);
                    src1_addr <<= 1;

                }else{
                    src1_addr <<=1;
                    src1_addr |= 1;
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
                }else{
                    src2_addr <<=1;
                    src2_addr |= 1;
                }
                break;
        }
        idx+=1;
    }
    printf("set");
    switch(opp){
        case '+':
            printf("sum ");
            break;
        case '-':
            printf("diff ");
            break;
        case '*':
            printf("prod ");
            break;
        case '/':
            printf("quot ");
            break;
        case '%':
            printf("mod ");
            break;
        case '&':
            printf("and ");
            break;
        case '^':
            printf("xor ");
            break;
        case '|':
            printf("or ");
            break;
    };
    printf("(@ %#04x), ",dest_addr>>1);
    switch(src1_addr&1){
        case 1:
            //it's a variable
            printf("(@ %#04x), ",src1_addr>>1);
            break;
        case 0:
            //it's a literal
            printf("%d, ",src1_addr>>1);
            break;
    }
    switch(src2_addr&1){
        case 1:
            //it's a variable
            printf("(@ %#04x) ",src2_addr>>1);
            break;
        case 0:
            //it's a literal
            printf("%d ",src2_addr>>1);
            break;
    }
    printf("\n");

}
