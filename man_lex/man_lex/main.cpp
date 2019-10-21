//
//  main.cpp
//  man_lex
//
//  Created by 赵旭 on 2019/10/16.
//  Copyright © 2019 Zhaox. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define numChar 10000

using namespace std;

const char keyword[50][13]={"abstract","assert","boolean","break","byte","case","catch","char",
    "class","const","continue","default","do","double","else","enum","extends","final",
    "finally","float","for","if","goto","implements","import","instanceof","int","interface",
    "long","native","new","package","private","protected","public","return","short","static",
    "strictfp","super","switch","synchronized","this","throw","throws","transient","try",
    "void","volatile","while"};

const char monocularOperator[15]={'+','-','*','/','=','>','<','!','?',':','&','|','^','%','~'};

const char binocularOperator[22][5]={"==","<=",">=","!=","&&","||","++","--","<<",">>",">>>","+=","-=","*=",
    "%=","/=","&=","|=","^=","<<=",">>=",">>>="};

const char Delimiter[9]={'(',')','[',']','{','}',';',',','.'};

FILE* fileSource=NULL;

/*判断是否为字母**/
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

/*判断是否为数字**/
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

/*查找关键字**/
int searchKey(char keyword[][13],char s[])
{
    for(int i=0;i<50;i++){
        if (strcmp(keyword[i], s)==0) {
            return i+1;                 //成功查找返回种别码
        }
    }
    return -1;                          //查找不成功返回-1
}

/*读取程序文件内容到数组中**/
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

/*编译预处理**/
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
