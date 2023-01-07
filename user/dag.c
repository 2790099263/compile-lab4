#include "dag.h"

basic_block head;

basic_block newBasicBlock(){
    basic_block res = malloc(sizeof(basic_block));
    if(res == NULL){
        printf("No Memory!\n");
        exit(0);
    }
    res->lab_number = ++reg_num;

    
}