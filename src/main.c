/*************************************************************************
	> File Name: main.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Fri 31 Mar 2017 11:15:59 AM CST
 ************************************************************************/

#define _GNU_SOURCE 1

#include<stdio.h>
#include"raid-5.h"

int main()
{
    int con, rw;
    long id;
    
    while(scanf("%d %d %ld", &con, &rw, &id)!=EOF)
    {
        execute(id);
        writeBlock();
        printf("%s\n", ssd_path);
    }
    return 0;
}
