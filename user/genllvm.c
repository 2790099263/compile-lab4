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
void genRootDot(past cur,char *buffer) {
	node_type type = cur->nodeType;
	switch (type) {
		case COMPOUND_STMT: printf("COMPOUND_STMT"); break;
		case RETURN_STMT: printf("RETURN_STMT"); break;
		case DECL_REF_EXPR: printf("DECL_REF_EXPR %s", cur->svalue); break;
		case CALL_EXPR: printf("CALL_EXPR"); break;
		case INTEGER_LITERAL: printf("INTEGER_LITERAL %d", cur->ivalue); break;
		case FLOATING_LITERAL: printf("FLOATING_LITERAL %f", cur->fvalue); break;
		case UNARY_OPERATOR: printf("UNARY_OPERATOR %s", cur->svalue); break;
		case ARRAY_SUBSCRIPT_EXPR: printf("ARRAY_SUBSCRIPT_EXPR"); break;
		case BINARY_OPERATOR:
            if(cur->svalue[0]=='+'||cur->svalue[0]=='-'||cur->svalue[0]=='*'||cur->svalue[0]=='/'){
                genArithmeticExpr(cur,buffer+strlen(buffer));
            } 
            printf("BINARY_OPERATOR %s",cur->svalue); 
            break;
		case IF_STMT: printf("IF_STMT"); break;
		case WHILE_STMT: printf("WHILE_STMT"); break;
		case CONTINUE_STMT: printf("CONTINUE_STMT"); break;
		case BREAK_STMT: printf("BREAK_STMT"); break;
		case FUNCTION_DECL: printf("FUNCTION_DECL %s", cur->svalue); break;
		case VAR_DECL: printf("VAR_DECL %s", cur->svalue); break;
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
	genRoot(node->right,buffer,nest + 1);
	genRoot(node->next,buffer,nest);
    genRootDot(node,buffer);
}
//return the register number of result
void genArithmeticExpr(past node,char *result){
    int a=0,b=0;
    if (node->left->nodeType == DECL_REF_EXPR)
    {
        sprintf(result+strlen(result),"%%%d = load i32, ptr %%%d, align 4\n"
            ,++reg_num,tab[hashString(node->left->svalue)]);
        a=reg_num;
    }else{
        a=node->left->regnum;
    }
    if (node->right->nodeType == DECL_REF_EXPR)
    {
        sprintf(result+strlen(result),"%%%d = load i32, ptr %%%d, align 4\n"
            ,++reg_num,tab[hashString(node->right->svalue)]);
        b=reg_num;
    }else{
        b=node->right->regnum;
    }
    if(node->svalue[0]=='+'){
        sprintf(result+strlen(result),"%%%d = add nsw i32 %%%d, %%%d\n",++reg_num,a,b);
        node->regnum = reg_num;
    }
    else if(node->svalue[0]=='-'){
        sprintf(result+strlen(result),"%%%d = sub nsw i32 %%%d, %%%d\n",++reg_num,a,b);
        node->regnum = reg_num;
    }
    else if(node->svalue[0]=='*'){
        sprintf(result+strlen(result),"%%%d = mul nsw i32 %%%d, %%%d\n",++reg_num,a,b);
        node->regnum = reg_num;
    }
    else if(node->svalue[0]=='/'){
        sprintf(result+strlen(result),"%%%d = sdiv i32 %%%d, %%%d\n",++reg_num,a,b);
        node->regnum = reg_num;
    }
    return ;
}