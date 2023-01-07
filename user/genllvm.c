#include "genllvm.h"
int reg_num = 0;
const int MOD = 114493;
int hashString(char *s){
    int len=strlen(s),res=0;
    for(int i=0;i<len;i++){
        res=(res*1333+s[i])%MOD;
    }
    return res%MOD;
}
char* typeJudge(past node){
    if(node->flojud==0)return "i32";
    else return "float";
}
void genRootDot(past cur,char *buffer) {
	node_type type = cur->nodeType;
	switch (type) {
		case COMPOUND_STMT: printf("COMPOUND_STMT"); break;
		case RETURN_STMT: printf("RETURN_STMT"); break;
		case DECL_REF_EXPR:
            printf("\nhash : %d\n",hashString(cur->svalue));
            cur->flojud = typeJud[hashString(cur->svalue)];
            printf("DECL_REF_EXPR %s", cur->svalue); 
            break;

		case CALL_EXPR: printf("CALL_EXPR"); break;
		case INTEGER_LITERAL:
            // typeJud[hashString(cur->svalue)]=0;
            cur->flojud = 0;
            printf("INTEGER_LITERAL %d", cur->ivalue); 
            break;

		case FLOATING_LITERAL:
            // typeJud[hashString(cur->svalue)]=1; 
            printf("\n%s %d\n",cur->svalue,hashString(cur->svalue));
            cur->flojud = 1;
            printf("FLOATING_LITERAL %f", cur->fvalue); 
            break;

		case UNARY_OPERATOR: printf("UNARY_OPERATOR %s", cur->svalue); break;
		case ARRAY_SUBSCRIPT_EXPR: printf("ARRAY_SUBSCRIPT_EXPR"); break;
		case BINARY_OPERATOR:
            if(cur->svalue[0]=='+'||cur->svalue[0]=='-'||cur->svalue[0]=='*'||cur->svalue[0]=='/'||cur->svalue[0]=='%'){
                genArithmeticExpr(cur,buffer+strlen(buffer));
            } 
            printf("BINARY_OPERATOR %s",cur->svalue); 
            break;
		case IF_STMT: printf("IF_STMT"); break;
		case WHILE_STMT: printf("WHILE_STMT"); break;
		case CONTINUE_STMT: printf("CONTINUE_STMT"); break;
		case BREAK_STMT: printf("BREAK_STMT"); break;
		case FUNCTION_DECL: printf("FUNCTION_DECL %s", cur->svalue); break;
		case VAR_DECL: 
            if(cur->left->nodeType == INTEGER_LITERAL){
                typeJud[hashString(cur->svalue)]=0;
            }else{
                typeJud[hashString(cur->svalue)]=1;
            }
            genVarDecl(cur,buffer+strlen(buffer));
            printf("VAR_DECL %s", cur->svalue);
            break;
		case PARM_DECL: printf("PARM_DECL %s", cur->svalue); break;
		case INIT_LIST_EXPR: printf("INIT_LIST_EXPR"); break;
		case DECL_STMT: printf("DECL_STMT"); break;
		case TRANSLATION_UNIT: printf("TRANSLATION_UNIT"); break;
		case NULL_STMT: printf("NULL_STMT"); break;
		default: printf("Unknown node type!"); break;
	}
}
void genRoot(past node, char *buffer,int nest) {
	if(node == NULL) return;
	int i = 0;
	for(i = 0; i < nest; i ++)
		printf("  ");
	//printf("%d\n", node->nodeType);
	
	puts("");
	genRoot(node->left, buffer,nest + 1);
    if(node ->nodeType == IF_STMT){
        int now_label = reg_num;
        int head_label = ++reg_num;
        int nxt_label = ++reg_num;
        sprintf(buffer+strlen(buffer),"br i1 %%%d, label %%%d, label %%%d\n"
            ,now_label,head_label,nxt_label);
        sprintf("%d:\n",head_label);
        genRoot(node->right,buffer,nest);
        // if(node->right->next!=NULL){
        //     sprintf(buffer,"");
        // }
        return ;
    }
	genRoot(node->right,buffer,nest + 1);
	genRoot(node->next,buffer,nest);
    genRootDot(node,buffer);
}
//generate VarDecl part
void genVarDecl(past node,char *result){
    printf("\ngenDecl  %s %d\n",node->svalue,hashString(node->svalue));
    if(node->left->flojud==0){
        sprintf(result+strlen(result),"%%%d = alloca i32, align4\n",++reg_num);
        tab[hashString(node->svalue)]=reg_num;
        sprintf(result+strlen(result),"store i32 %d,ptr %%%d, align4\n"
            ,node->left->ivalue,reg_num);
    }else{
        sprintf(result+strlen(result),"%%%d = alloca float, align4\n",++reg_num);
        tab[hashString(node->svalue)]=reg_num;
        sprintf(result+strlen(result),"store float %.6e,ptr %%%d, align4\n"
            ,node->left->fvalue,reg_num);
    }
}

/*
    generate ArithmeticExpr part
    no return thing. 
*/
void genArithmeticExpr(past node,char *result){
    int a=0,b=0;
    /*
        judge if the left part or right part in register,
        if in ,go on
        else push it into a new register
    */
    if (node->left->nodeType == DECL_REF_EXPR)
    {
        sprintf(result+strlen(result),"%%%d = load %s, ptr %%%d, align 4\n"
            ,++reg_num,typeJudge(node->left),tab[hashString(node->left->svalue)]);
        a=reg_num;
    }else{
        a=node->left->regnum;
    }
    if (node->right->nodeType == DECL_REF_EXPR)
    {
        sprintf(result+strlen(result),"%%%d = load %s, ptr %%%d, align 4\n"
            ,++reg_num,typeJudge(node->right),tab[hashString(node->right->svalue)]);
        b=reg_num;
    }else{
        b=node->right->regnum;
    }
    // this node 's flojud is equal left part | right part flojud;
    node->flojud = node->left->flojud | node->right->flojud;
    /*
        if the left part and right part is not the same flojud
        transform the int type to float type.
    */
    if((node->left->flojud^node->right->flojud==1)&&(node->left->flojud==0)){
        sprintf(result+strlen(result),"%%%d = sitofp i32 %%%d to float\n"
            ,++reg_num,node->left->regnum);
        node->left->regnum = reg_num;
    }else if((node->left->flojud^node->right->flojud==1)&&(node->right->flojud==0)){
        sprintf(result+strlen(result),"%%%d = sitofp i32 %%%d to float\n"
            ,++reg_num,node->right->regnum);
        node->left->regnum = reg_num;
    }
    /*
        in five way to solve the question
        judge with the first word of node->svalue.
    */
    if(node->svalue[0]=='+'){
        sprintf(result+strlen(result),"%%%d = %sadd nsw %s %%%d, %%%d\n"
            ,++reg_num,node->flojud==1?"f":"",typeJudge(node),a,b);
        node->regnum = reg_num;
    }
    else if(node->svalue[0]=='-'){
        sprintf(result+strlen(result),"%%%d = %ssub nsw %s %%%d, %%%d\n"
            ,++reg_num,node->flojud==1?"f":"",typeJudge(node),a,b);
        node->regnum = reg_num;
    }
    else if(node->svalue[0]=='*'){
        sprintf(result+strlen(result),"%%%d = %smul nsw %s %%%d, %%%d\n"
            ,++reg_num,node->flojud==1?"f":"",typeJudge(node),a,b);
        node->regnum = reg_num;
    }
    else if(node->svalue[0]=='/'){
        sprintf(result+strlen(result),"%%%d = %sdiv %s %%%d, %%%d\n",
            ++reg_num,node->flojud==1?"f":"s",typeJudge(node),a,b);
        node->regnum = reg_num;
    }
    else if(node->svalue[0]=='%'){
        sprintf(result+strlen(result),"%%%d = srem %s %%%d, %%%d\n"
            ,++reg_num,typeJudge(node),a,b);
        node->regnum = reg_num;
    }
    return ;
}