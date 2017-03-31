/*************************************************************************
	> File Name: raid-5.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 30 Mar 2017 04:52:18 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "raid-5.h"

void initGlobalVirtualAddr()
{
}

void initRaidVirtualAddr()
{
}

// param : id the Nth 4KB
void execute(long id)
{   
    long global_block_id = (long)( id / PAGENUM);  // the Nth block in the global virtual address
    long global_stripe_id = (long)(global_block_id / 4); // the Nth stripe
    int block_offset_stripe = global_block_id - global_stripe_id*4;  // the block offset in a stripe at global address
    int parity_ssd_id = (global_stripe_id / 100) % 5; // the parity in Nth ssd
    int block_ssd_id = (block_offset_stripe >= parity_ssd_id) ? block_offset_stripe+1 : block_offset_stripe;
    printf("parity_ssd_id = %d\tblock_ssd_id = %d\tstripe_id = %d\n\n", parity_ssd_id, block_ssd_id, global_stripe_id);
    
}



    

