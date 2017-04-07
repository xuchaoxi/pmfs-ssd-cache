/*************************************************************************
	> File Name: main.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Fri 31 Mar 2017 11:15:59 AM CST
 ************************************************************************/

#define _GNU_SOURCE 1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h> 
#include<fcntl.h>
#include<unistd.h>
#include "raid/raid_5.h"
#include "nvm_cache.h"
#include "main.h"
#include "trace2call.h"

int main(int arc, char *argv[])
{
    initNVMBuffer();
    nvm_fd = open(nvm_device, O_RDWR | O_DIRECT);
    ssd0_fd = open(ssd0_device, O_RDWR | O_DIRECT);
    ssd1_fd = open(ssd1_device, O_RDWR | O_DIRECT);
    ssd2_fd = open(ssd2_device, O_RDWR | O_DIRECT);
    ssd3_fd = open(ssd3_device, O_RDWR | O_DIRECT);
    ssd4_fd = open(ssd4_device, O_RDWR | O_DIRECT);
    
    if(nvm_fd < 0)
    {
        perror("[ERROR]:nvm_fd ");
        exit(0);
    }
    if(ssd0_fd < 0)
    {
        perror("[ERROR]:ssd0_fd ");
        exit(0);
    }
    if(ssd1_fd < 0)
    {
        perror("[ERROR]:ssd1_fd ");
        exit(0);
    }

    if(ssd2_fd < 0)
    {
        perror("[ERROR]:ssd2_fd ");
        exit(0);
    }
    if(ssd3_fd < 0)
    {
        perror("[ERROR]:ssd3_fd ");
        exit(0);
    }
    if(ssd4_fd < 0)
    {
        perror("[ERROR]:ssd4_fd ");
        exit(0);
    }
    trace_to_iocall("trace");

    close(nvm_fd);
    close(ssd0_fd);
    close(ssd1_fd);
    close(ssd2_fd);
    close(ssd3_fd);
    close(ssd4_fd);
    return 0;
/*
    struct timeval tv_begin, tv_end;
    double total_time;
    initPageBuffer(); 
    gettimeofday(&tv_begin, NULL);
    while(scanf("%d %d %ld", &con, &rw, &id)!=EOF)
    {
      //  if(i%1000000==0)
        {
            printf("%d %d %ld\n", con, rw, id);
            i = 0;
        }
        ++i;
        directWrite(id);

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

    }
    gettimeofday(&tv_end, NULL);
    total_time = (tv_end.tv_usec - tv_begin.tv_usec)/1000000.0+(tv_end.tv_sec - tv_begin.tv_sec);
    printf("total time = %lf\n",total_time);
    return 0;
    */
}
