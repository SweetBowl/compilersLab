%{
enum yytokentype{
	NUMBER = 258,
    ID = 269,
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

int yylval;

%}

PRINT			print
IF				if
ELSE			else
WHILE			while
RETURN			return
SCAN			scan
INT				int
STR				str
VOID			void

DOT				[.]
COMMA			[,]
NOT				[!]
SEM				[;]
LP				\(
RP				\)
LB				\[
RB				\]
LC				\{
RC				\}
PLUS			[+]
SUB				[-]
MUL				[*]
DIV				[/]
MOD				[%]
EQUAL			[=]

CMP				[>]|[<]|[>][=]|[<][=]|[=][=]|[!][=]
ASSIGN			[=]|[*][=]|[/][=]|[+][=]|[-][=]|[%][=]

NUMBER			(-?[1-9][0-9]*|0)
STRING			\".*\"
ID				[a-zA-Z_$][a-zA-Z_$]*

%%

{COMMA}			|
{DOT}			|
{NOT}			|
{SEM}			|
{LP}			|
{RP}			|
{LB}			|
{RB}			|
{LC}            |
{RC}			|
{PLUS}			|
{SUB}			|
{MUL}			|
{DIV}			|
{MOD}			|
{EQUAL}			{
					printf("tok:%s\n",yytext);return yytext;
				}
				
{NUMBER}		{	printf("tok:%s\n",yytext);return 258;}
{ID}			{	printf("tok:%s\n",yytext);return 259;}
{STRING}		{	printf("tok:%s\n",yytext);return 260;}
{VOID}			{	printf("tok:%s\n",yytext);return 261;}
{IF}			{	printf("tok:%s\n",yytext);return 262;}
{ELSE}			{	printf("tok:%s\n",yytext);return 263;}
{WHILE}			{	printf("tok:%s\n",yytext);return 264;}
{RETURN}		{	printf("tok:%s\n",yytext);return 265;}
{PRINT}			{	printf("tok:%s\n",yytext);return 266;}
{SCAN}			{	printf("tok:%s\n",yytext);return 267;}
{CMP}			{	printf("tok:%s\n",yytext);return 268;}
{ASSIGN}		{	printf("tok:%s\n",yytext);return 269;}
{INT}			{	printf("tok:%s\n",yytext);return 270;}
{STR}			{	printf("tok:%s\n",yytext);return 271;}

[\n\t]			{ /*ignore whitespace*/ }
%%

int yyerror(char *s)
{
	fprintf(stderr,"error: %s\n",s);
	return 0;
}

int yywrap()
{
	return 1;	
}
