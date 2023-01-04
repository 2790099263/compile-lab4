#define _GENLLVM_H
#ifdef  _GENLLVM_H
#include <string.h>
#include <stdlib.h>
#include "node_type.h"
#include "ast.h"
int tab[120005];
int hashString(char *s);
void genRootDot(past cur,char *buffer);
void genRoot(past node,char *buffer,int nest);
void genArithmeticExpr(past node,char *result);
#endif