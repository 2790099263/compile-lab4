#define _DAG_H
#ifdef  _DAG_H
#include <stdio.h>
#include <string.h>

extern reg_num;
typedef struct _dag dag;
typedef struct _dag *basic_block;

struct _dag{
    int lab_number  ;   //basic block label number
    char *sresult   ;   //result of code generate in block
    char *jmp       ;   //condition to excute jump instruction
    int true_next   ;   //if true,jump to label <true_next>
    int false_next  ;   //if false,jump to label <false_next>
};

#endif