#include<stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "genllvm.h"
#include "dag.h"
extern FILE* yyin;
extern int yyparse(void);
extern past root;
void yyerror(char *s)
{
	printf("%s\n", s);
}
char buffer[5000];

int main(int argc,char* argv[])
{
	if(argc<2){
		printf("input file is needed.\n");
		exit(0);
	}
	setbuf(stdout,NULL);
	yyin = fopen(argv[1], "r");
	if(yyin==NULL){
		printf("can not open file.\n");
		exit(0);
	}
	yyparse();
	dagInit();
	genRoot(root,buffer,0);
	printf("\n");
	printf("%s\n",buffer);
	return 0;
}
