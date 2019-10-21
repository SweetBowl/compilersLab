//
//  main.cpp
//  man_lex
//
//  Created by 赵旭 on 2019/10/16.
//  Copyright © 2019 Zhaox. All rights reserved.
//

//#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define numChar 10000
#define numKey 50
#define nummono 12
#define numspe 12
#define numbino 22
#define constSyn 99
#define liberSyn 100

using namespace std;

/**种别码
 1.关键字
 种别码从1开始，依次往后顺延至50
 2.非prefix的单目符
 种别码从51开始，依次往后顺延至62
 3.特殊单目符
 种别码从63开始编号，依次往后顺延至74
 4.多目符
 种别码从75开始编号，依次往后顺延至96
 5.常数
 种别码为99
 6.标识符
 种别码为100
 */
char keyword[50][13]={"abstract","assert","boolean","break","byte","case","catch","char",
    "class","const","continue","default","do","double","else","enum","extends","final",
    "finally","float","for","if","goto","implements","import","instanceof","int","interface",
    "long","native","new","package","private","protected","public","return","short","static",
    "strictfp","super","switch","synchronized","this","throw","throws","transient","try",
    "void","volatile","while"};

const char monoOperator[12]={'(',')','[',']','{','}',';',',','.','?',':','~'};

const char speOperator[12]={'=','<','>','+','-','!','*','%','/','&','|','^'};

const char binocularOperator[22][5]={"==","<=","<<=","<<",">=",">>",">>>",">>=",">>>=","+=","++","--","-=","!=","*=",
    "%=","/=","&=","&&","|=","||","^=",};

FILE* fileSource=NULL;

/**判断是否为字母*/
bool isLetter(char letter)
{
    if((letter >='a' && letter<='z') || (letter>='A'&&letter<='Z'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**判断是否为数字*/
bool isDigit(char digit)
{
    if(digit>='0'&&digit<='9')
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**查找关键字*/
int searchKey(char keyword[][13],char s[])
{
    for(int i=0;i<50;i++){
        if (strcmp(keyword[i], s)==0) {
            return i+1;                 //成功查找返回种别码
        }
    }
    return -1;                          //查找不成功返回-1
}

/**读取程序文件内容到数组中*/
int infoInit(char resourceProj[numChar]){
    
    FILE *fp;
    int cur=0;
    
    if ((fp=fopen("/Users/zhaoxu/Hej.java", "r"))==NULL) {
        printf("无法打开文件\n");
        return 0;
    }
    
    //初始化数组
     for (int j=0; j<numChar; j++) {
         resourceProj[j]='\0';
     }
    
    //在未到达文件尾的情况下将文件内容读取到数组中
    resourceProj[cur] = fgetc(fp);
    while (!feof(fp)) {
        cur++;
        resourceProj[cur] = fgetc(fp);
    }
    resourceProj[++cur]='\0';
    
    return cur;
}

/**编译预处理*/
void filterPro(char ch[],int lenResource)
{
    char temString[numChar];
    int count=0;
    
    for (int i=0; i<=lenResource; i++) {
//        单行注释'//'，去除注释后面的东西，直到换行
        if (ch[i] =='/' && ch[i+1]=='/') {
            while (ch[i]!='\n') {
                i++;
            }
        }
        if (ch[i]=='/'&&ch[i+1]=='*') {
//         多行注释'/*'，去除注释后面的东西，直到遇到'*/'
            i += 2;
            while (ch[i]!='*'||ch[i+1]!='/') {
                i++;
            }
            i += 2;
            
        }
        if (ch[i] !='\n' && ch[i]!='\t' && ch[i]!='\r') {
//            过滤掉无用字符，存到数组中
            temString[count++]=ch[i];
        }
    }
    
    temString[count] ='\0';
    strcpy(ch, temString);
}

/**判断是否为标识符或关键字*/
int checkLetter(char resourceProj[],char token[],int &cur,int &count){
    int syn = -1;
    if (isLetter(resourceProj[cur])|| resourceProj[cur] =='_' || resourceProj[cur]=='$') {
        token[count++] = resourceProj[cur];
        cur++;
//        读取后面的字母或数字或特殊字符
        while (isLetter(resourceProj[cur]) || isDigit(resourceProj[cur]) ||resourceProj[cur] =='_' ||resourceProj[cur] =='$') {
            token[count++] =resourceProj[cur];
            cur++;
        }
        token[count]='\0';
//        判断是否为关键字
        syn = searchKey(keyword, token);
        if (syn == -1) {
//            此单词为标识符
            syn =liberSyn;
        }
    }
    return syn;
}

/**判断是否为常数*/
int checkDigit(char resourceProj[],char token[],int &cur,int &count){
    int syn;
    if (isDigit(resourceProj[cur])) {
            while (isDigit(resourceProj[cur])) {
            token[count++] = resourceProj[cur];
            cur++;
        }
    }
    token[count]='\0';
    syn = constSyn;
    return syn;
}

/**判断不是某bino的prefix的单目符*/
//const char monoOperator[14]={'(',')','[',']','{','}',';',',','.','!','?',':','^','~'};
int checkMono(char resourceProj[],char token[],int &cur,char ch){
    int syn = -1;
    if (ch =='('||ch==')'||ch=='['||ch==']'||ch=='{'||ch=='}'||ch==';'||ch==','||ch=='.'||ch=='!'
        ||ch=='?'||ch==':'||ch=='^'||ch=='~') {
        token[0] = resourceProj[cur];
        token[1] = resourceProj[cur];
        for (int i=0; i<nummono; i++) {
            if (resourceProj[cur] == monoOperator[i]) {
                syn = numKey+1+i;
                break;
            }
        }
        cur++;
    }
    return syn;
}

/**判断是否为多目符或是prefix的单目符*/
//种别码：特殊的单目符从63开始编号，依次往后顺延至74
//种别码：多目符从75开始编号，依次往后顺延至96

//const char speOperator[12]={'=','<','>','+','-','!','*','%','/','&','|','^'};
//const char binocularOperator[22][5]={"==","<=","<<","<<=",">=",">>",">>>",">>=",">>>=","+=","++","--","-=","!=","*=",
//"%=","/=","&=","&&","|=","||","^=",};
int checkBino(char resourceProj[],int &cur){
    int syn=-1;
    if (resourceProj[cur] == '=') {
        //判断是否为 = 或者 ==
        cur++;      //超前搜索
        if (resourceProj[cur]=='=') {
            syn = 75;
        }
        else{
            cur--;  //回退
            syn =63;
        }
        cur++;
        return syn;
    }
    else if (resourceProj[cur]=='<') {
        //判断是否为< 或者 <= 或者 << 或者 <<=
        cur++;
        if (resourceProj[cur]=='=') {
            syn =76;
        }
        else if(resourceProj[cur] =='<'){
            cur++;
            if (resourceProj[cur]=='=') {
                syn=78;
            }
            else
            {
                cur--;
                syn=77;
            }
        }
        else
        {
            cur--;
            syn = 64;
        }
        cur++;
        return syn;
    }
    else if(resourceProj[cur]=='>'){
        //>, >=, >>, >>=, >>>, >>>=
        cur++;
        if (resourceProj[cur]=='=') {
            syn=79;
        }
        else if(resourceProj[cur]=='>')
        {
            cur++;
            if (resourceProj[cur]=='>') {
                cur++;
                if (resourceProj[cur]=='=') {
                    syn=83;
                }
                else
                {
                    cur--;
                    syn=81;
                }
            }
            else if(resourceProj[cur]=='=')
            {
                syn=82;
            }
            else
            {
                cur--;
                syn=80;
            }
        }
        else
        {
            cur--;
            syn=65;
        }
        cur++;
        return syn;
    }
    else if(resourceProj[cur]=='+'){
        cur++;
        if (resourceProj[cur]=='=') {
            syn=84;
        }
        else if(resourceProj[cur]== '+'){
            syn = 85;
        }
        else{
            cur--;
            syn=66;
        }
        cur++;
        return syn;
    }
    else if(resourceProj[cur]=='-'){
        cur++;
        if (resourceProj[cur]=='=') {
            syn=87;
        }
        else if(resourceProj[cur]=='-'){
            syn=96;
        }
        else{
            cur--;
            syn=67;
        }
        cur++;
        return syn;
    }
    else if(resourceProj[cur]=='!'){
        cur++;
        if (resourceProj[cur]=='=') {
            syn=88;
        }
        else{
            cur--;
            syn=68;
        }
        cur++;
        return syn;
    }
    else if(resourceProj[cur]=='*'){
        cur++;
        if (resourceProj[cur]=='=') {
            syn=89;
        }
        else{
            cur--;
            syn=69;
        }
        cur++;
        return syn;
    }
    else if(resourceProj[cur]=='%'){
        cur++;
        if (resourceProj[cur]=='=') {
            syn=90;
        }
        else{
            cur--;
            syn=70;
        }
        cur++;
        return syn;
    }
    else if(resourceProj[cur]=='/'){
        cur++;
        if (resourceProj[cur]=='=') {
            syn=91;
        }
        else{
            cur--;
            syn=71;
        }
        cur++;
        return syn;
    }
    else if(resourceProj[cur]=='&'){
        cur++;
        if (resourceProj[cur]=='=') {
            syn=92;
        }
        else if(resourceProj[cur]=='&'){
            syn=93;
        }
        else{
            cur--;
            syn=72;
        }
        cur++;
        return syn;
    }
    else if(resourceProj[cur]=='|'){
        cur++;
        if (resourceProj[cur]=='=') {
            syn=94;
        }
        else if(resourceProj[cur]=='|'){
            syn=95;
        }
        else{
            cur--;
            syn=73;
        }
        cur++;
        return syn;
    }
    else if(resourceProj[cur]=='^'){
        cur++;
        if (resourceProj[cur]=='=') {
            syn=96;
        }
        else{
            cur--;
            syn=74;
        }
        cur++;
        return syn;
    }
    //上述条件都没有退出本函数时，返回-1
    return syn;
}

/**完整的词法分析主体*/

int main(int argc, const char * argv[]) {
    
    char resourceProj[numChar];
    int i=0,j=0;
    int lenResource=0;
    
    lenResource=infoInit(resourceProj);
   
    //测试读取到的内容
    while (resourceProj[i]!='\0') {
        printf("%c ",resourceProj[i]);
        i++;
    }
    printf("\n");
    filterPro(resourceProj, lenResource);
    while (resourceProj[j]!='\0') {
        printf("%c ",resourceProj[j]);
        j++;
    }
    printf("\n");

    return 0;
}
