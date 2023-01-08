#include "dag.h"
/*
    head is the first block
    now_block point the block we using now
*/
basic_block head,now_block;
/*
    new a basic block
    return the basic_block point
    if there is no momory ,print error message and exit
*/
basic_block newBasicBlock(){
    basic_block res = malloc(sizeof(basic_block));
    if(res == NULL){
        printf("No Memory!\n");
        exit(0);
    }
    res->lab_number = reg_num++;
    res->sresult    = NULL;
    res->jmp        = NULL;
    res->true_next  = NULL;
    res->false_next = NULL;
    return res;
}

/*
    add a new block to now block;
    @param: 
        tfJudge: Judging whether to establish a real edge or a false edge
            0 -> false edge 1 -> true edge
    @return:
        new_block;
*/
basic_block addBlock(int tfJudge){
    basic_block new_block = newBasicBlock();
    if(tfJudge==0)now_block->false_next = new_block;
    else now_block->true_next = new_block;
    now_block = new_block;
    return new_block;
}

/*
    add a new edge from one block to another block
    @param:
        tfJudge: Judging whether to establish a real edge or a false edge
            0 -> false edge 1 -> true edge
        fromBlock: source endpoint of the edge
        toBlock  : edge end point
    @return: none
*/
void addEdge(int tfJudge,basic_block fromBlock,basic_block toBlock){
    if(tfJudge == 0)fromBlock->false_next = toBlock;
    else fromBlock->true_next   = toBlock;
    return ;
}

/*
    initialization
    @param: none;
    @return:none;
*/
void dagInit(){
    head = newBasicBlock();
    now_block = head;
    return ;
}

/*
    print all messages in node of DAG
    @param: node: basic_block need to operate
    @return: none
*/
void printNode(basic_block node){
    printf("%d:\n",node->lab_number);
    printf("%s\n",node->sresult);
    if(node->true_next!=NULL && node->false_next!=NULL){
        printf("br i1 %%%d,label %%%d, label %%%d\n"
            ,node->jmp,node->true_next->lab_number,node->false_next->lab_number);
    }
    else if(node->true_next!=NULL){
        printf("br label %%%d\n",node->true_next->lab_number);
    }
}