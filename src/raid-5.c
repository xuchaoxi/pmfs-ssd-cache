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

void initGlobalVirtualAddr()
{
}

void initRaidVirtualAddr()
{
}

void initBLKBuffer()
{
    int i;
    block_buf = (char*)memalign(512, BLKSIZE);
    if(!block_buf)
    {
        perror("[ERROR]:Fail to allocate blcok buffer");
        exit(0);
    }
    for(i = 0;i < BLKSIZE;++i)
        block_buf[i] = '.';
}

// param : id the Nth 4KB
void execute(long page_id)
{   
    // the Nth block in the global virtual address
    global_block_id = (long)( page_id / PAGENUM);  
    
    // the Nth stripe
    global_stripe_id = (long)(global_block_id / 4); 
    
    // the block offset in a stripe at global address
    block_offset_stripe = global_block_id - global_stripe_id*4;  
    
    // the parity block in Nth ssd
    parity_ssd_id = (global_stripe_id / rotate_width) % 5; 
    
    // the data block in Nth ssd
    data_ssd_id = (block_offset_stripe >= parity_ssd_id) ? block_offset_stripe+1 : block_offset_stripe;
    printf("parity_ssd_id = %d\tblock_ssd_id = %d\tstripe_id = %d\n\n", parity_ssd_id, data_ssd_id, global_stripe_id);
    
}

void writeBlock()
{
    //ssd_path[7] = data_ssd_id+'0';
    ssd_path[8] = data_ssd_id+'1';
    int ssdfd = open(ssd_path, O_RDWR | O_DIRECT);
    if(ssdfd < 0)
    {
        perror("[ERROR]:Fail to open ssd device");
        exit(0);
    }
    initBLKBuffer();
    off_t offset = global_stripe_id;
 //   int code = pwrite(ssdfd, block_buf, BLKSIZE, offset);   
//    int code = write(ssdfd, block_buf, 512);
    int code = 1;
    if(code < 0)
    {
        perror("[ERROR]:Fail to write block buffer");
        exit(0);
    }
    close(ssdfd);
}


