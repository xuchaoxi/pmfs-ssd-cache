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
#include "raid_5.h"

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
void execute(off_t offset)
{ 
/*  size = 4KB
 */
    // stripe_id
    global_stripe_id = (long)(offset / N);
    // parity ssd id
    parity_ssd_id = (global_stripe_id / rotate_width) % (N+1);
    // data ssd id
    block_offset_stripe = offset % N;
    data_ssd_id = (block_offset_stripe >= parity_ssd_id) ? block_offset_stripe+1 : block_offset_stripe;
    // offset map in global raid address
    data_raid_offset = (global_stripe_id * (N+1)) + data_ssd_id;
    parity_raid_offset = (global_stripe_id * (N+1)) + parity_ssd_id;
    ssd_page_off = global_stripe_id;

/*  old 
 *  use BLOCKSIZE & PAGENUM
    // the Nth block in the global virtual address
    global_block_id = (long)( offset / PAGENUM);  
    

    // page offset in a block
    page_off = offset % PAGENUM;
    
    // the Nth stripe
    global_stripe_id = (long)(global_block_id / N); 

    // offset at Nth ssd
    ssd_page_off = global_stripe_id * PAGENUM + page_off;
    
    // the block offset in a stripe at global address
    block_offset_stripe = global_block_id % N;  
    
    // the parity block in Nth ssd
    parity_ssd_id = (global_stripe_id / rotate_width) % (N+1); 
    
    // the data block in Nth ssd
    data_ssd_id = (block_offset_stripe >= parity_ssd_id) ? block_offset_stripe+1 : block_offset_stripe;
//    printf("parity_ssd_id = %d data_ssd_id = %d stripe_id = %d\n", parity_ssd_id, data_ssd_id, global_stripe_id);
    // raid-5 offset mapped
    data_raid_offset = (global_stripe_id*(N+1) + data_ssd_id)*PAGENUM + page_off;
    parity_raid_offset = (global_stripe_id*(N+1) + parity_ssd_id)*PAGENUM + page_off; 
*/
}

int writeOrReadPage(int ssd_id, off_t offset, char *buffer, int flag)
{
    switch(ssd_id)
    {
        case 0 : ssd_path = ssd0_device;
                 break;
        case 1 : ssd_path = ssd1_device;
                 break;
        case 2 : ssd_path = ssd2_device;
                 break;
        case 3 : ssd_path = ssd3_device;
                 break;
        case 4 : ssd_path = ssd4_device;
                 break;
    }
/*    int ssdfd = open(ssd_path, O_WRONLY | O_DIRECT);
    if(ssdfd < 0)
    {
        perror("[ERROR]:Fail to open ssd device");
        exit(0);
    }
    */
//    initPageBuffer();
 
    // ssd_page_offset 
 //   off_t offset = global_stripe_id*PAGENUM +  page_off;
    
    int code;
   // if(flag==1)
  //      code = pwrite(ssdfd, buffer, PAGESIZE, offset*PAGESIZE);   
 //   else if(flag==0)
    //    code = pread(ssdfd, buffer, PAGENUM, offset*PAGESIZE);
/*
    if(code < 0)
    {
        perror("[ERROR]:Fail to write or read page buffer");
        exit(0);
    }
*/
    //close(ssdfd);
    return code;
}


void directWrite(long pageid)
{
    ssd_path = "/dev/sdb1";
    int fd = open(ssd_path, O_WRONLY | O_DIRECT);
    if(fd < 0)
    {
        perror("[ERROR]:Fail to open /dev/sdb1");
        exit(0);
    }
    int ret = pwrite(fd, page_buf, PAGESIZE, pageid*PAGESIZE);
    if(ret < 0)
    {
        perror("[ERROR]:Fail to write page buffer in ssd1");
        exit(0);
    }
    close(fd);
}
