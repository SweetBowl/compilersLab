//
//  main.c
//  parser1
//
//  Created by 赵旭 on 2019/11/16.
//  Copyright © 2019 Zhaox. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype{
    NUMBER = 258,
//    EOL = 259,
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

extern int yylex(void);
extern int yylval;
extern char* yytext;
extern FILE* yyin;
extern FILE* yyout;

int tok;
int program(void);
int external_declaration(void);
int decl_or_stmt(void);
int declarator_list(void);
int intstr_list(void);
int initializer(void);
int declarator(void);
int parameter_list(void);
int parameter(void);
int type(void);
int statement(void);
int statement_list(void);
int expression_statement(void);
int expr(void);
int cmp_expr(void);
int add_expr(void);
int mul_expr(void);
int primary_expr(void);
int expr_list(void);
int id_list(void);

void advance()
{
    tok = yylex();
    printf("tok: %s\n",yytext);
}

//program
//: external_declaration
//| program external_declaration
//;

int program(){
    int result=0;
    do{
        if (external_declaration()) {
            result=1;
        }
        else result=0;
    }while(tok == EOF);
    return result;
}

//external_declaration
//: type declarator decl_or_stmt
//;
int external_declaration(){
    int result = 0;
    if (type()) {
        if (declarator()) {
            if (decl_or_stmt()) {
                result = 1;
            }
        }
    }
    else result=0;
    return result;
}

//decl_or_stmt
//: '{' statement_list '}'
//| '{' '}'
//| ',' declarator_list ';'
//| ';'
//;
int decl_or_stmt(){
    int result = 0;
    if (tok =='{') {
        advance();
        if (statement_list()) {
            result = 1;
        }
        else if (tok=='}'){
            advance();
            result=1;
        }
    }
    else if (tok==','){
        advance();
        if (declarator_list()) {
            if (tok ==';') {
                result=1;
                advance();
            }
        }
    }
    else if (tok==';'){
        advance();
        result = 1;
    }
    return result;
}

//declarator_list
//: declarator
//| declarator_list ',' declarator
//;
int declarator_list(){
    int result = 0;
    int i=0;
    do {
        if (i!=0) {
            advance();
        }
        if (declarator()) {
            i++;
            result=1;
        }
        else{
            result=0;
        }
    } while (tok==',');
    return result;
}

//intstr_list
//: initializer
//| intstr_list ',' initializer
//;
int intstr_list(){
    int result=0;
    int i=0;
    do {
        if(i!=0)
        {
            advance();
        }
        if(initializer()){
            i++;
            result=1;
        }
        else{
            result=0;
        }
    } while (tok==',');
    return result;
}

//initializer
//: NUMBER
//| STRING
//;
int initializer(){
    int result = 0;
    //不知道是否需要advance()
    advance();
    if (tok==NUMBER) {
        result=1;
    }
    else if (tok==STRING) {
        result=1;
    }
    return result;
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
int declarator(){
    int result=0;
    if (tok==ID) {
        advance();
        if (tok=='=') {
            advance();
            if (expr()) {
                result=1;
            }
        }
        else if (tok=='('){
            advance();
            if (parameter_list()) {
                if (tok==')') {
                    advance();
                    result=1;
                }
            }
            else if(tok==')'){
                advance();
                result=1;
            }
        }
        else if(tok=='['){
            advance();
            if (expr()) {
                if (tok==']') {
                    advance();
                    if (tok=='=') {
                        advance();
                        if (tok=='{') {
                            advance();
                            if (intstr_list()) {
                                if (tok=='}') {
                                    advance();
                                    result=1;
                                }
                            }
                        }
                    }
                    else result=1;
                }
            }
            else if(tok==']'){
                advance();
                if (tok=='=') {
                    advance();
                    if (tok=='{') {
                        advance();
                        if (intstr_list()) {
                            if (tok=='}') {
                                advance();
                                result=1;
                            }
                        }
                    }
                }
                else result=1;
            }
        }
        else result=1;//不知道是1还是0
    }
    return result;
}

//parameter_list
//: parameter
//| parameter_list ',' parameter
//;
int parameter_list(){
    int result=0;
    int i=0;
    do {
        if (i!=0) {
            advance();
        }
        if (parameter()) {
            i++;
            result=1;
        }
        else{
            result=0;
            return result;
        }
    } while (tok==',');
    return result;
}

//parameter
//: type ID
//;
int parameter(){
    int result=0;
    if (type()) {
        if (tok==ID) {
            advance();
            result=1;
        }
    }
    return result;
}

//type
//: INT
//| STR
//| VOID
//;
int type(){
    int result=0;
    //不知道是否需要advance()
    advance();
    if (tok==INT) {
        result=1;
    }
    else if (tok==STR) {
        result=1;
    }
    else if (tok==VOID) {
        result=1;
    }
    return result;
}

//statement
//: type declarator_list ';'
//| '{' statement_list '}'
//| expression_statement
//| IF '(' expr ')' statement
//| IF '(' expr ')' statement ELSE statement
//| WHILE '(' expr ')' statement
//| RETURN ';'
//| RETURN expr ';'
//| PRINT ';
//| PRINT expr_list ';'
//| SCAN id_list ';'
//;
int statement(){
    int result=0;
    if (type()) {
        if (declarator_list()) {
            if (tok==';') {
                advance();
                result=1;
            }
        }
    }
    else if(tok=='{'){
        advance();
        if (statement_list()) {
            if (tok=='}') {
                advance();
                result=1;
            }
        }
    }
    else if(expression_statement()){
        result=1;
    }
    else if(tok==IF){
        advance();
        if (tok=='(') {
            advance();
            if (expr()) {
                if (tok==')') {
                    advance();
                    if (statement()) {
                        if (tok==ELSE)
                        {
                            advance();
                            if (statement()) {
                                result=1;
                            }
                        }
                        else result=1;
                    }
                }
            }
        }
    }
    else if(tok==WHILE)
    {
        advance();
        if (tok==('(')) {
            advance();
            if (expr()) {
                if (tok==')') {
                    advance();
                    if (statement()) {
                        result=1;
                    }
                }
            }
        }
    }
    else if(tok==RETURN)
    {
        advance();
        if (tok==';') {
            advance();
            result=1;
        }
        else if(expr())
        {
            if (tok==';') {
                advance();
                result=1;
            }
        }
    }
    else if(tok==PRINT)
    {
        advance();
        if (tok==';') {
            advance();
            result=1;
        }
        else if(expr_list())
        {
            if (tok==';') {
                advance();
                result=1;
            }
        }
    }
    else if(tok==SCAN){
        advance();
        if(id_list())
        {
            if(tok==';')
            {
                advance();
                result=1;
            }
        }
    }
    return result;
}

//statement_list
//: statement
//| statement_list statement
//;
int statement_list(){
    int result=0;
    do {
        if (statement()) {
            result=1;
        }
        else
        {
            result=0;
            return result;
        }
    } while (tok!=EOF);
    return result;
}

//expression_statement
//: ';'
//| expr ';'
//;
int expression_statement(){
    int result=0;
    if (tok==';') {
        advance();
        result=1;
    }
    else if (expr())
    {
        if (tok==';') {
            advance();
            result=1;
        }
    }
    return result;
}

//expr
//: cmp_expr
//;
int expr(){
    int result=0;
    if (cmp_expr()) {
        result=1;
    }
    return result;
}

//cmp_expr
//: add_expr
//| cmp_expr CMP add_expr
//;
int cmp_expr(){
    int result=0;
    int i=0;
    do {
        if (i!=0) {
            advance();
        }
        if (add_expr()) {
            result=1;
            i++;
        }
        else{
            result=0;
            return result;
        }
    } while (tok==CMP);
    return result;
}

//add_expr
//: mul_expr
//| add_expr '+' mul_expr
//| add_expr '-' mul_expr
//;
int add_expr(){
    int result=0;
    int i=0;
    do {
        if (i!=0) {
            advance();
        }
        if (mul_expr()) {
            result=1;
            i++;
        }
        else{
            result=0;
            return result;
        }
    } while (tok=='+'|tok=='-');
    return result;
}

//mul_expr
//: primary_expr
//| mul_expr '*' primary_expr
//| mul_expr '/' primary_expr
//| mul_expr '%' primary_expr
//| '-' primary_expr
//;
int mul_expr(){
    int result=0;
    int i=0;
    if (tok=='-') {
        advance();
        do {
            if (i!=0) {
                advance();
            }
            if (primary_expr()) {
                result=1;
                i++;
            }
            else{
                result=0;
                return result;
            }
        } while (tok=='*'|tok=='/'|tok=='%');
    }
    else
    {
        do {
            if (i!=0) {
                advance();
            }
            if (primary_expr()) {
                result=1;
                i++;
            }
            else{
                result=0;
                return result;
            }
        } while (tok=='*'|tok=='/'|tok=='%');
    }
    return result;
}

//primary_expr
//: ID '(' expr_list ')'
//| ID '(' ')'
//| '(' expr ')'
//| ID
//| NUMBER
//| STRING
//| ID ASSIGN expr
//| ID '=' expr
//| ID '[' expr ']'
//| ID '[' expr ']' '=' expr
//;
int primary_expr(){
    int result=0;
    if (tok==ID) {
        advance();
        if (tok=='(') {
            advance();
            if (expr_list()) {
                if (tok==')') {
                    result=1;
                    advance();
                }
            }
            else if (tok==')')
            {
                advance();
                result=1;
            }
        }
        else if (tok=='['){
            advance();
            if (expr()) {
                if (tok==']') {
                    advance();
                    if (tok=='=') {
                        advance();
                        if (expr()) {
                            result=1;
//                            advance();
                        }
                    }
                    else result=1;
                }
            }
        }
        else if (tok=='='){
            advance();
            if (expr()) {
                result=1;
            }
        }
        else if (tok==ASSIGN)
        {
            advance();
            if (expr()) {
                result=1;
            }
        }
        else result=1;
    }
    else if (tok==NUMBER)
    {
        result=1;
        advance();
    }
    else if (tok==STRING)
    {
        result=1;
        advance();
    }
    else if (tok=='(')
    {
        advance();
        if (expr()) {
            if (tok==')') {
                advance();
                result=1;
            }
        }
    }
    return result;
}

//expr_list
//: expr
//| expr_list ',' expr
//;
int expr_list(){
    int result=0;
    int i=0;
    do {
        if (i!=0) {
            advance();
        }
        if (expr()) {
            result=1;
            i++;
        }
        else
        {
            result=0;
            return result;
        }
    } while (tok==',');
    
    return result;
}

//id_list
//: ID
//| id_list ',' ID
//;
int id_list(){
    int result=0;
    int i=0;
    do {
        if (i!=0) {
            advance();
        }
        if (tok==ID) {
            advance();
            result=1;
            i++;
        }
        else{
            result=0;
            return result;
        }
    } while (tok==',');
    return result;
}

int main(int argc, const char * argv[]) {
//    FILE *fp;
    if((yyin=fopen("/Users/zhaoxu/Downloads/编译_lab2/expr.txt", "r"))==NULL){
        printf("无法打开文件\n");
        return 0;
    };
    advance();
    if (program()) {
        printf("Success!\n");
    }
    else
        printf("Fail!\n");

    return 0;
}
