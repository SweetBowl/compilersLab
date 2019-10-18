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

/*检查程序文件是否存在或是否为空
 
 在最新的Xcode Version 11.0 (11A420a)下，此函数不能运行
 
bool checkNullFile(char const *fileName)
{
    FILE *fp = fopen(fileName, "rb");
    printf("文件存在\n");
    if (!fp) {
        printf("程序文件不存在,将为您创建一个！\n");
        FILE *fp=fopen(fileName, "wb");
        fclose(fp);
        return false;
    }
    else{
        char tmp;
        int res = fscanf(fp, "%c",&tmp);
        fclose(fp);
        if (res<=0) {
            return false;
        }
        else
            return true;
    }
}

//读取程序文件内容//
bool infoInit(char const *fileName,char resourceProj[numChar])
{
    FILE *fp;
    int cur = 0;
    bool res=checkNullFile(fileName);

    //初始化数组
    for (int j=0; j<numChar; j++) {
        resourceProj[j]='\0';
    }
    
    if ((fp=fopen(fileName, "rb"))==NULL) {
        if ((fp=fopen(fileName, "wb"))==NULL) {
            printf("错误，无法创建程序文件\n");
        }
    }
    
    else{
        while (res&&feof(fp)) {
            fscanf(fp,"%c", &resourceProj[cur]);
            cur++;
        }
        resourceProj[cur]='\0';
    }
    fclose(fp);
    printf("已读取文件内容到数组中\n");
    return true;
}
*/

/*读取程序文件内容到数组中**/
bool infoInit(char resourceProj[numChar]){
    
    FILE *fp;
    int cur=0;
    
    if ((fp=fopen("/Users/zhaoxu/Hej.java", "r"))==NULL) {
        printf("无法打开文件\n");
        return false;
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
    
    return true;
}

int main(int argc, const char * argv[]) {
    
    char resourceProj[numChar];
    int i=0;
    
    infoInit(resourceProj);
   
    //测试读取到的内容
    while (resourceProj[i]!='\0') {
        printf("%c ",resourceProj[i]);
        i++;
    }
    
    return 0;
}
