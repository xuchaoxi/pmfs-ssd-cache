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

int main(int argc, char *argv[])
{
    char *trace_file_path[]={ "res/web_0.csv.req","res/trace/web_2.csv.req",
        "res/trace/hm_0.csv.req","res/trace/mds_1.csv.req",
        "res/trace/prn_0.csv.req","res/trace/prn_1.csv.req",
        "res/trace/proj_0.csv.req","res/trace/proj_1.csv.req",
        "res/trace/src1_0.csv.req","res/trace/src1_1.csv.req", 
        "res/trace/usr_0.csv.req","res/trace/usr_1.csv.req",
    };
    if(argc==4)
    {
        NNVMBuffers = atoi(argv[1]);
        NNVMBufferTables = atoi(argv[1]);
        STRIPES = atoi(argv[1]);
        if(atoi(argv[2])==0)
            EvictStrategy = CLOCK;
        if(atoi(argv[2])==1)
            EvictStrategy = CLOCKSTRIPE;
        if(atoi(argv[2])==2)
            EvictStrategy = LRU;
        if(atoi(argv[2])==3)
            EvictStrategy = LRUSTRIPE;
        if(atoi(argv[2])==4)
            EvictStrategy = FIFO;
        if(atoi(argv[2])==5)
            EvictStrategy = FIFOSTRIPE;
    } 
    else {
        printf("paramteters are wrong \n");
        exit(0);
    }

            
    initNVMBuffer();
//    initNVMStripeBuffer();
/*
 *
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
    */
    trace_to_iocall(trace_file_path[atoi(argv[3])]);
/*
 *
    close(nvm_fd);
    close(ssd0_fd);
    close(ssd1_fd);
    close(ssd2_fd);
    close(ssd3_fd);
    close(ssd4_fd);*/
    return 0;
}
