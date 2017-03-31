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
    long i = 0;

    struct timeval tv_begin, tv_end;
    double total_time;
    initPageBuffer(); 
    gettimeofday(&tv_begin, NULL);
    while(scanf("%d %d %ld", &con, &rw, &id)!=EOF)
    {
        if(i%1000000==0)
        {
            printf("%d %d %ld\n", con, rw, id);
            i = 0;
        }
        ++i;
        directWrite(id);
/*
        execute(id);
        if(rw==0)
        {
            writeOrReadPage(data_ssd_id, 0);
            writeOrReadPage(parity_ssd_id, 0);
        }
        else if(rw==1)
        {
            writeOrReadPage(data_ssd_id, 1);
        }
*/
    }
    gettimeofday(&tv_end, NULL);
    total_time = (tv_end.tv_usec - tv_begin.tv_usec)/1000000.0+(tv_end.tv_sec - tv_begin.tv_sec);
    printf("total time = %lf\n",total_time);
    return 0;
}
