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
    int *jmp       ;   //register number of jump operation
    basic_block true_next   ;   //if true,jump to label <true_next>
    basic_block false_next  ;   //if false,jump to label <false_next>
};
basic_block newBasicBlock();
basic_block addBlock(int tfJudge);
void addEdge(int tfJudge,basic_block fromBlock,basic_block toBlock);
void dagInit();
void printNode(basic_block node);

#endif