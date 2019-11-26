//
//  main.c
//  parser2
//
//  Created by 赵旭 on 2019/11/24.
//  Copyright © 2019 Zhaox. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype{
    NUMBER = 258,
    ID = 259,
    STRING = 260,
    VOID = 261,
    IF = 262,
    ELSE = 263,
    WHILE = 264,
    RETURN = 265,
    PRINT = 266,
    SCAN = 267,
    CMP=268,
    ASSIGN=269,
    INT=270,
    STR=271
};

typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
    int ivalue;
    char* nodeType;
    past left;
    past right;
}_ast;

extern int yylex(void);
extern int yylval;
extern char* yytext;
extern FILE* yyin;
extern FILE* yyout;

int tok;
void advance(void);
void program(past node);
void external_declaration(past node);
void type(past node);
void decl_or_stmt(past node);
void declarator(past node);
void declarator_list(past node);
void statement_list(past node);
void initializer(past node);
void expr(past node);
void parameter_list(past node);
void parameter(past node);
void statement(past node);
void expr_list(past node);
void id_list(past node);
void expression_statement(past node);
past cmp_expr(void);
past add_expr(void);
past mul_expr(void);
past primary_expr(void);


void advance()
{
    tok = yylex();
//    printf("tok: %s\n", yytext);
}
void error(){
    printf("Matching Fails!\n");
    exit(0);
}

//新的树结点
past newAstNode(){
    past node = malloc(sizeof(ast));
    if (node == NULL) {
        printf("run out of memory.\n");
        exit(0);
    }
    memset(node, 0, sizeof(ast));
    return node;
}

//program
//: external_declaration
//| program external_declaration
//;
void program(past node){
    node->nodeType = "program";
    node->ivalue = 0;
    node->left=NULL;
    node->right=NULL;
    node->left = newAstNode();
    printf("PROGRAM.\n");
    external_declaration(node->left);
    //......此处待改动........
    while (tok!=EOF) {
        while (node->right != NULL) {
            node = node->right;
        }
    node ->right = newAstNode();
    program(node->right);
    }
}

//external_declaration
//: type declarator decl_or_stmt
//;
void external_declaration(past node){
    node->nodeType = "external_declaration";
    node->ivalue = 0;
    node->left=NULL;
    node->right=NULL;
    node->left=newAstNode();
    
    type(node->left);
    while (node->right!=NULL) {
        node=node->right;
    }
    node->right=newAstNode();
    declarator(node->right);
    
    //decl_or_stmt
    while (node->right!=NULL) {
        node= node->right;
    }
    node->right=newAstNode();
    decl_or_stmt(node->right);
}

//decl_or_stmt
//: '{' statement_list '}'
//| '{' '}'
//| ',' declarator_list ';'
//| ';'
//;
void decl_or_stmt(past node){
    node->nodeType = "decl_or_stmt";
    node->ivalue=0;
    node->left=NULL;
    node->right=NULL;
    advance();
    if (tok=='{') {
        advance();
        //......存疑......
        printf("%s\n",yytext);
        if (tok=='}') {
            advance();
            printf("%s\n",yytext);
        }
        else
        {
            node->left =newAstNode();
            statement_list(node->left);
            //......advance();?
            if (tok=='}') {
                advance();
            }
            else{
                printf("decl_or_stmt");
                error();
            }
        }
    }
    else if(tok==','){
        advance();
        printf("%s\n",yytext);
        node->left=newAstNode();
        declarator_list(node->left);
        if (tok==';') {
            advance();
            printf("%s\n",yytext);
        }
        else
        {
            printf("decl_or_stmt\n");
            error();
        }
    }
    else if (tok==';'){
        advance();
        printf("%s\n",yytext);
    }
    else{
        printf("decl_or_stmt\n");
        error();
    }
}

//declarator_list
//: declarator
//| declarator_list ',' declarator
//;
void declarator_list(past node){
    node->nodeType = "declarator_list";
    node->ivalue=0;
    node->left=NULL;
    node->right=NULL;
    
    node->left=newAstNode();
    declarator(node->left);
    if (tok==',') {
        advance();
        printf("%s\n",yytext);
        node->right=newAstNode();
        declarator_list(node->right);
    }
}

//intstr_list
//: initializer
//| intstr_list ',' initializer
//;
void intstr_list(past node){
    node->nodeType="intstr_list";
    node->ivalue=0;
    node->left=NULL;
    node->right=NULL;
    
    node->left=newAstNode();
    initializer(node->left);
    if (tok==',') {
        advance();
        printf("%s\n",yytext);
        node->right=newAstNode();
        intstr_list(node->right);
    }
}

//initializer
// : NUMBER
// | STRING
// ;
void initializer(past node){
    node->left=NULL;
    node->right=NULL;
    if (tok==NUMBER) {
        node->nodeType = "NUMBER";
        node->ivalue=258;
        advance();
        printf("%s\n",yytext);
    }
    else if (tok==STRING){
        node->nodeType = "STRING";
        node->ivalue=260;
        advance();
        printf("%s\n",yytext);
    }
    else
    {
        printf("initializer\n");
        error();
    }
}

//declarator
//: ID
//| ID '=' expr
//| ID '(' parameter_list ')'
//| ID '(' ')'
//| ID '[' expr ']'
//| ID '[' ']'
//| ID '[' expr ']' '=' '{' intstr_list '}'
//| ID '[' ']' '=' '{' intstr_list '}'
//;
void declarator(past node){
    if (tok!=ID) {
        printf("declarator\n");
        error();
    }
    else{
        node->nodeType="declarator";
        node->ivalue=0;
        node->left=NULL;
        node->right=NULL;
        advance();
        printf("%s\n",yytext);
        
        if (tok=='=') {
            node->left=newAstNode();
            node=node->left;
            node->nodeType="=";
            node->ivalue=128;       //= -> 128
            node->left=NULL;
            node->right=NULL;
            
            node->left = newAstNode();
            node->left->nodeType="ID";
            node->left->ivalue=259;
            node->left->left=NULL;
            node->left->right=NULL;
            advance();
            printf("%s\n",yytext);
            node->right=newAstNode();
            expr(node->right);
        }
        else if(tok=='('){
            advance();
            printf("%s\n",yytext);
            if (tok==')') {
                node->left=newAstNode();
                //...
                node->left->nodeType="ID";
                node->left->ivalue=259;
                
                node->left->left=NULL;
                node->left->right=NULL;
                advance();
                printf("%s\n",yytext);
            }
            else{
                node->left=newAstNode();
                node->left->nodeType="ID";
                node->left->ivalue=259;
                
                node->left->left=NULL;
                node->left->right=NULL;
                
                node->right=newAstNode();
                parameter_list(node->right);
                if (tok==')') {
                    advance();
                    printf("%s\n",yytext);
                }
                else{
                    printf("declarator\n");
                    error();
                }
            }
        }
        else if (tok=='['){
            advance();
            printf("%s\n",yytext);
            if (tok==']') {
                advance();
                printf("%s\n",yytext);
                if (tok=='=') {
                    node->left=newAstNode();
                    node=node->left;
                    node->nodeType="=";
                    node->ivalue=128;
                    node->left=NULL;
                    node->right=NULL;
                    
                    node->left=newAstNode();
                    node->left->nodeType="ID";
                    node->left->ivalue=259;

                    node->left->left=NULL;
                    node->left->right=NULL;
                    advance();
                    printf("%s\n",yytext);
                    
                    if (tok!='{') {
                        printf("declarator\n");
                        error();
                    }
                    else{
                        advance();
                        printf("%s\n",yytext);
                        node->right=newAstNode();
                        intstr_list(node->right);
                        
                        if (tok!='}') {
                            printf("declarator\n");
                            error();
                        }
                        else{
                            advance();
                            printf("%s\n",yytext);
                        }
                    }
                }
                else{
                    node->left=newAstNode();
                    node->left->nodeType="ID";
                    node->left->ivalue=259;
                    node->left->left=NULL;
                    node->left->right=NULL;
                }
            }
            else{
                node->left=newAstNode();
                node=node->left;
                node->nodeType="=";
                node->ivalue=128;
                node->left=NULL;
                node->right=NULL;
                
                node->left=newAstNode();
                expr(node->left);
                if (tok!=']') {
                    printf("declarator\n");
                    error();
                }
                else{
                    advance();
                    printf("%s\n",yytext);
                    if (tok=='=') {
                        advance();
                        printf("%s\n",yytext);
                        if (tok!='{') {
                            printf("declarator\n");
                            error();
                        }
                        else{
                            advance();
                            printf("%s\n",yytext);
                            
                            node->right=newAstNode();
                            intstr_list(node->right);
                            if (tok!='}') {
                                printf("declarator\n");
                                error();
                            }
                            else{
                                advance();
                                printf("%s\n",yytext);
                            }
                        }
                    }
                }
            }
        }
    }
}

//parameter_list
//: parameter
//| parameter_list ',' parameter
//;
void parameter_list(past node){
    node->nodeType="parameter_list";
    node->ivalue=0;
    node->left=NULL;
    node->right=NULL;
    node->left=newAstNode();
    parameter(node->left);
    
    if (tok==',') {
        advance();
        printf("%s\n",yytext);
        node->right=newAstNode();
        parameter_list(node->right);
    }
}

//parameter
//: type ID
//;
void parameter(past node){
    node->nodeType = "parameter";
    node->ivalue=0;
    node->left=NULL;
    node->right=NULL;
    
    node->left=newAstNode();
    type(node->left);
    if (tok!=ID) {
        printf("parameter\n");
        error();
    }
    else{
        node->right =newAstNode();
        node->right->nodeType="ID";
        node->right->ivalue=259;
        node->right->left=NULL;
        node->right->right=NULL;
        
        advance();
        printf("%s\n",yytext);
    }
}

//type
//: INT
//| STR
//| VOID
//;
void type(past node){
//    if (tok!=ID) {
//        printf("type\n");
//        error();
//    }
 //   else{
        if (strcmp(yytext,"int")==0) {
            node->nodeType = "INT";
            node->ivalue=270;
            node->left=NULL;
            node->right=NULL;
            advance();
            printf("%s\n",yytext);
        }
        else if (strcmp(yytext,"string")==0){
            node->nodeType="STR";
            node->ivalue=271;
            node->left=NULL;
            node->right=NULL;
            advance();
            printf("%s\n",yytext);
        }
        else if(strcmp(yytext,"void")==0){
            node->nodeType="VOID";
            node->ivalue=261;
            node->left=NULL;
            node->right=NULL;
            advance();
            printf("%s\n",yytext);
        }
        else{
            printf("type\n");
            error();
        }
    }
//}

//statement
//    : type declarator_list ';'
//    | '{' statement_list '}'
//    | expr_statement
//    | IF '(' expr ')' statement
//    | IF '(' expr ')' statement ELSE statement
//    | WHILE '(' expr ')' statement
//    | RETURN ';'
//    | RETURN expr ';'
//    | PRINT ';
//    | PRINT expr_list ';'
//    | SCAN id_list ';'
//    ;
void statement(past node){
    node->nodeType="statement";
    node->ivalue=0;
    node->left=NULL;
    node->right=NULL;
    
    //......
    if (tok==INT) {
        node->left=newAstNode();
        type(node->left);
        
        node->right=newAstNode();
        declarator_list(node->right);
        
        advance();
        if (tok!=';') {
            printf("statement\n");
            error();
        }
        else{
            advance();
            printf("%s\n",yytext);
        }
    }
    else if (tok==STR) {
        node->left=newAstNode();
        type(node->left);
        
        node->right=newAstNode();
        declarator_list(node->right);
        
        if (tok!=';') {
            printf("statement\n");
            error();
        }
        else{
            advance();
            printf("%s\n",yytext);
        }
    }
    else if (tok==VOID) {
        node->left=newAstNode();
        type(node->left);
        
        node->right=newAstNode();
        declarator_list(node->right);
        
        if (tok!=';') {
            printf("statement\n");
            error();
        }
        else{
            advance();
            printf("%s\n",yytext);
        }
    }
    else if (tok=='{'){
        advance();
        printf("%s\n",yytext);
        statement_list(node->left);
        if (tok=='}') {
            advance();
            printf("%s\n",yytext);
        }
        else{
            printf("statement\n");
            error();
        }
    }
    else if (tok==WHILE){
        node->left=newAstNode();
        node=node->left;
        node->nodeType="WHILE";
        node->ivalue=264;
        node->left=NULL;
        node->right=NULL;
        advance();
        printf("%s\n",yytext);
        if (tok!='(') {
            printf("statement\n");
            error();
        }
        else{
            advance();
            printf("%s\n",yytext);
            node->left=newAstNode();
            expr(node->left);
            if (tok!=')') {
                printf("statement\n");
                error();
            }
            else{
                advance();
                printf("%s\n",yytext);
                node->right=newAstNode();
                statement(node->right);
            }
        }
    }
    else if (tok==RETURN){
        node->left=newAstNode();
        node=node->left;
        node->nodeType="RETURN";
        node->ivalue=265;
        node->left=NULL;
        node->right=NULL;
        advance();
        printf("%s\n",yytext);
        if (tok==';') {
            advance();
            printf("%s\n",yytext);
        }
        else{
            node->left=newAstNode();
            expr(node->left);
            if(tok==';'){
                advance();
                printf("%s\n",yytext);
            }
            else{
                printf("statement\n");
                error();
            }
        }
    }
    else if(tok==IF){
        node->left = newAstNode();
        node->left->nodeType="IF";
        node->left->ivalue=262;
        node->left->left=NULL;
        node->left->right=NULL;
        advance();
        printf("%s\n",yytext);
        if (tok!='(') {
            printf("statement\n");
            error();
        }
        else{
            advance();
            printf("%s\n",yytext);
            node->left->left=newAstNode();
            expr(node->left->left);
            
            if (tok!=')') {
                printf("statement\n");
                error();
            }
            else{
                advance();
                printf("%s\n",yytext);
                node->left->right=newAstNode();
                statement(node->left->right);
                
                if (tok!=ELSE) {
                    
                }
                else{
                    node->right=newAstNode();
                    node->right->nodeType="ELSE";
                    node->right->ivalue=263;
                    node->right->left=NULL;
                    node->right->right=NULL;
                    
                    advance();
                    printf("%s\n",yytext);
                    node->right->left=newAstNode();
                    statement(node->right->left);
                }
            }
        }
    }
    else if (tok==PRINT){
        node->left=newAstNode();
        node=node->left;
        node->nodeType="PRINT";
        node->ivalue=266;
        node->left=NULL;
        node->right=NULL;
        
        advance();
        printf("%s\n",yytext);
        node->left=newAstNode();
        expr_list(node->left);
        if (tok!=';') {
            printf("statement\n");
            error();
        }
        else{
            advance();
            printf("%s\n",yytext);
        }
    }
    else if (tok==SCAN){
        node->left=newAstNode();
        node=node->left;
        node->nodeType="SCAN";
        node->ivalue=267;
        node->left=NULL;
        node->right=NULL;
        
        advance();
        printf("%s\n",yytext);
        node->left=newAstNode();
        id_list(node->left);
        if (tok!=';') {
            printf("statement\n");
            error();
        }
        else{
            advance();
            printf("%s\n",yytext);
        }
    }
    else{
        node->left=newAstNode();
        expression_statement(node->left);
    }
}

//statement_list
//: statement
//| statement_list statement
//;
void statement_list(past node){
    node->nodeType="statement_list";
    node->ivalue=0;
    node->left=NULL;
    node->right=NULL;
    
    node->left=newAstNode();
    statement(node->left);
    if (tok!='}') {
        node->right=newAstNode();
        statement_list(node->right);
    }
}

//expression_statement
//: ';'
//| expr ';'
//;
void expression_statement(past node){
    node->nodeType="expression_statement";
    node->ivalue=0;
    node->left=NULL; node->right=NULL;

    if(tok==';'){
        advance();
        printf("%s\n",yytext);
    }
    else{
        node->left=newAstNode();
        expr(node->left);
        if(tok !=';'){
            printf("expression_statement\n");
            error();
        }
        else{
            advance();printf("%s\n",yytext);
        }
    }
}

//expr_list
//: expr
//| expr_list ',' expr
//;
void expr_list(past node){
    node->nodeType="expr_list";
    node->ivalue=0;
    node->left=NULL;
    node->right=NULL;

    node->left=newAstNode();
    expr(node->left);
    if(tok==','){
        advance();printf("%s\n",yytext);

        node->right=newAstNode();
        expr_list(node->right);
    }
}

//expr
//: cmp_expr
//;
void expr(past node){
    node->nodeType="expr";
    node->ivalue=0;
    node->left=NULL;
    node->right=NULL;
    node->left=cmp_expr();
}

//cmp_expr
//: add_expr
//| cmp_expr CMP add_expr
//;
past cmp_expr(){
    past node=newAstNode();
    node->nodeType="cmp_expr";
    node->ivalue=0;
    node->left=NULL;
    node->right=NULL;

    past temp=add_expr();

    if(strcmp(yytext, "==")==0||strcmp(yytext, "!=")||strcmp(yytext, "<=")||strcmp(yytext, ">=")||strcmp(yytext, "<")||strcmp(yytext, ">")){
        node->left=newAstNode();
        
        node->left->nodeType=malloc(sizeof(char) * 128);
        strcpy(node->left->nodeType,yytext);
        node->left->ivalue=tok;
        node->left->left=NULL; node->left->right=NULL;

        node->left->left=temp;

        advance();printf("%s\n",yytext);
        node->left->right=cmp_expr();
    }
    else node->left=temp;

    return node;
}

past add_expr(){
    past node=newAstNode();
    node->nodeType="add_expr";
    node->ivalue=0;
    node->left=NULL; node->right=NULL;

    past temp=mul_expr();
    if((tok=='+')||(tok=='-')){
        node->left=newAstNode();
        node->left->nodeType=malloc(sizeof(char) * 128);
        strcpy(node->left->nodeType,yytext);
        node->left->ivalue=tok;
        node->left->left=NULL; node->left->right=NULL;

        node->left->left=temp;

        advance();printf("%s\n",yytext);
        node->left->right=add_expr();
    }
    else node->left=temp;

    return node;
}

past mul_expr(){
    past node=newAstNode();
    node->nodeType="mul_expr";
    node->ivalue=0;
    node->left=NULL; node->right=NULL;

    if(tok=='-'){
        node->left=newAstNode();
        node->left->nodeType="-";
        node->left->ivalue='-';
        node->left->left=NULL; node->left->right=NULL;

        advance();printf("%s\n",yytext);
        node->right=primary_expr();
    }
    else{
        past temp=primary_expr();
        if((tok=='*')||(tok=='/')||(tok=='%')){
            node->left=newAstNode();
            node->left->nodeType=malloc(sizeof(char) * 128);
            strcpy(node->left->nodeType,yytext);
            node->left->ivalue=tok;
            node->left->left=NULL; node->left->right=NULL;

            node->left->left=temp;

            advance();printf("%s\n",yytext);
            node->left->right=mul_expr();
        }
        else node->left=temp;
    }

    return node;
}

past primary_expr(){
    past node=newAstNode();
    node->nodeType="primary_expr";
    node->ivalue=0;
    node->left=NULL; node->right=NULL;

    if(tok=='('){//parentheses (
        advance();printf("%s\n",yytext);
        node->left=newAstNode();
        expr(node->left);
        if(tok!=')'){
            error();
        }
        else{
            advance();printf("%s\n",yytext);
        }
    }
    else if(tok==NUMBER){//digit
        node->left=newAstNode();
        node->left->nodeType="NUMBER";
        node->left->ivalue=NUMBER;
        node->left->left=NULL;
        node->left->right=NULL;
        advance();printf("%s\n",yytext);
    }
    else if(tok=='"'){//string
        node->left=newAstNode();
        node->left->nodeType="STRING";
        node->left->ivalue='"';
        node->left->left=NULL;
        node->left->right=NULL;
        advance();printf("%s\n",yytext);
    }
    else if(tok==ID){//ID
        advance();
        printf("%s\n",yytext);

        if(tok=='='){//=
            node->left=newAstNode();
            node->left->nodeType="=";
            node->left->ivalue=128;
            node->left->left=NULL;
            node->left->right=NULL;

            node->left->left=newAstNode();
            node->left->left->nodeType="ID";
            node->left->left->ivalue=ID;
            node->left->left->left=NULL; node->left->left->right=NULL;

            advance();printf("%s\n",yytext);
            node->left->right=newAstNode();
            expr(node->left->right);
        }
        else if(tok=='('){//parentheses (
            node->left=newAstNode();
            node->left->nodeType="ID";
            node->left->ivalue=ID;
            node->left->left=NULL;
            node->left->right=NULL;

            advance();printf("%s\n",yytext);
            if(tok==')'){
                advance();printf("%s\n",yytext);
            }
            else{
                node->right=newAstNode();
                expr_list(node->right);

                if(tok==')'){
                    advance();printf("%s\n",yytext);
                }
                else{
                    error();
                }
            }
        }
        else if(tok=='['){ // [
            advance();printf("%s\n",yytext);

            past temp=cmp_expr();

            if(tok!=']'){
                error();
            }
            else{
                advance();printf("%s\n",yytext);
                if(tok=='='){// =
                    node->left=newAstNode();
                    node->left->nodeType="=";
                    node->left->ivalue=128;
                    node->left->left=NULL; node->left->right=NULL;

                    node->left->left=temp;

                    advance();
                    printf("%s\n",yytext);
                    node->left->right=newAstNode();
                    expr(node->left->right);
                }
            }
        }
    }
    else{
        error();
    }

    return node;
}


void id_list(past node){
    node->nodeType="id_list";
    node->ivalue=0;
    node->left=NULL; node->right=NULL;

    if(tok!=ID){
        error();
    }
    else{
        node->left=newAstNode();
        node->left->nodeType="ID";
        node->left->ivalue=ID;
        node->left->left=NULL; node->left->right=NULL;

        advance();printf("%s\n",yytext);
        if(tok==','){
            advance();
            printf("%s\n",yytext);

            node->right=newAstNode();
            id_list(node->right);
        }
    }
}

void showAst(past node, int nest)
{
    if(node == NULL)
        return;

    int i = 0;
    for(i = 0; i < nest; i ++)
        printf("  ");

    printf("%s %d\n",node->nodeType,node->ivalue);
    showAst(node->left, nest+1);
    showAst(node->right, nest+1);

}


int main()
{
    int nest=0;
    ///Users/zhaoxu/test.c
    ///Users/zhaoxu/eclipse-workspace/parserTest1/src/expr.txt
    if((yyin=fopen("/Users/zhaoxu/Downloads/编译_lab2/test.c", "r"))==NULL){
        printf("无法打开文件\n");
        return 0;
    };
    printf("Hello Compiler\n");
    advance();
    past node=newAstNode();
    program(node);
    showAst(node,nest);
    printf("\nsuccess!");
    return 0;
}
