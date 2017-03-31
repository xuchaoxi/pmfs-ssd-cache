/*************************************************************************
	> File Name: global.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Fri 31 Mar 2017 12:40:26 PM CST
 ************************************************************************/

#include "raid-5.h"

long GBLKNUM;  // global block numbers
long RBLKNUM;  // raid block numbers
size_t BLKSIZE = 524288;  //128*4096;  // block size;
size_t PAGESIZE = 4096;  // page size
long rotate_width = 100; // rotate width for stripe

char *ssd_path; 

int data_ssd_id;
int parity_ssd_id;
int block_offset_stripe;
long global_stripe_id;
long global_block_id; 
int page_off;
char *page_buf;

