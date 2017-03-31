/*************************************************************************
	> File Name: main.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Fri 31 Mar 2017 11:15:59 AM CST
 ************************************************************************/

#define _GNU_SOURCE 1

#include<stdio.h>
#include<stdlib.h>
#include"raid-5.h"

int main()
{
    int con, rw;
    long id;

    struct timeval tv_begin, tv_end;
    double total_time;
    
    gettimeofday(&tv_begin, NULL);
    while(scanf("%d %d %ld", &con, &rw, &id)!=EOF)
    {
        execute(id);
        writePage();
    }
    gettimeofday(&tv_end, NULL);
    total_time = (tv_end.tv_usec - tv_begin.tv_usec)/1000000.0+(tv_end.tv_sec - tv_begin.tv_sec);
    printf("total time = %lf\n",total_time);
    return 0;
}
