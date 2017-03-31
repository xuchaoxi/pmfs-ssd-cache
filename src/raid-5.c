/*************************************************************************
	> File Name: raid-5.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 30 Mar 2017 04:52:18 PM CST
 ************************************************************************/

#define _GNU_SOURCE 1

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/file.h>
#include "raid-5.h"

void initPageBuffer()
{
    int i;
//    page_buf = (char*)memalign(2, PAGESIZE);
    page_buf = (char*)valloc(PAGESIZE);
    if(!page_buf)
    {
        perror("[ERROR]:Fail to allocate page buffer");
        exit(0);
    }
    for(i = 0;i < PAGESIZE;++i)
        page_buf[i] = '.';
}

// param : page_id is the Nth 4KB
void execute(long page_id)
{   
    // the Nth block in the global virtual address
    global_block_id = (long)( page_id / PAGENUM);  

    // page offset in a block
    page_off = page_id % PAGENUM;
    
    // the Nth stripe
    global_stripe_id = (long)(global_block_id / N); 
    
    // the block offset in a stripe at global address
    block_offset_stripe = global_block_id - global_stripe_id*N;  
    
    // the parity block in Nth ssd
    parity_ssd_id = (global_stripe_id / rotate_width) % (N+1); 
    
    // the data block in Nth ssd
    data_ssd_id = (block_offset_stripe >= parity_ssd_id) ? block_offset_stripe+1 : block_offset_stripe;
//    printf("parity_ssd_id = %d data_ssd_id = %d stripe_id = %d\n", parity_ssd_id, data_ssd_id, global_stripe_id);
    
}

void writePage()
{
    switch(data_ssd_id)
    {
        case 0 : 
            {
                ssd_path = "/dev/sdb1";
                break;
            }
        case 1 : 
            {
                ssd_path = "/dev/sdb2";
                break;
            }
        case 2 : 
            {
                ssd_path = "/dev/sdb3";
                break;
            }
        case 3 : 
            {
                ssd_path = "/dev/sdb5";
                break;
            }
        case 4 : 
            {
                ssd_path = "/dev/sdb6";
                break;
            }
    }
    int ssdfd = open(ssd_path, O_RDWR | O_DIRECT);
    if(ssdfd < 0)
    {
        perror("[ERROR]:Fail to open ssd device");
        exit(0);
    }
//    initPageBuffer();
    off_t write_off = global_stripe_id*PAGENUM +  page_off;
    int code = pwrite(ssdfd, page_buf, PAGESIZE, write_off*4096);   
    if(code < 0)
    {
        perror("[ERROR]:Fail to write block buffer");
        exit(0);
    }
    close(ssdfd);
}


