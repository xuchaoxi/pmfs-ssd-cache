/*************************************************************************
	> File Name: global.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Fri 31 Mar 2017 12:40:26 PM CST
 ************************************************************************/

#include "raid/raid_5.h"
#include "nvm_cache.h"
#include "main.h"

size_t BLKSIZE = 524288;  //128*4096;  // block size;
size_t PAGESIZE = 4096;  // page size
long rotate_width = 1280; // rotate width for stripe
size_t NVM_BUFFER_SIZE = 4096;

char *ssd_path; 

int data_ssd_id;
int parity_ssd_id;
off_t ssd_page_off; 
off_t data_raid_offset;
off_t parity_raid_offset;
long global_stripe_id;

char *page_buf;


unsigned long NNVMBuffers = 5000;
unsigned long NNVMBufferTables = 5000;
unsigned long STRIPES = 10;

NVMEvictionStrategy EvictStrategy = LRU;

int nvm_fd;
int ssd0_fd;
int ssd1_fd;
int ssd2_fd;
int ssd3_fd;
int ssd4_fd;

char nvm_device[] = "/mnt/pmfs/nvm";
char ssd0_device[] = "/dev/sdb1";
char ssd1_device[] = "/dev/sdb2";
char ssd2_device[] = "/dev/sdb3";
char ssd3_device[] = "/dev/sdb5";
char ssd4_device[] = "/dev/sdb6";



unsigned long hit_num;
unsigned long hit_parity;
unsigned long hit_data;
unsigned long hit_stripe;
unsigned long write_blocks;
unsigned long flush_blocks;
unsigned long flush_data;
unsigned long flush_parity;
unsigned long flush_stripe;
double pm_time, flush_time;

NVMBufferDesc *nvm_buffer_descriptors;
NVMBufferControl *nvm_buffer_control;
NVMBufferHashBucket *nvm_buffer_hashtable;

NVMStripeBufferDesc *nvm_stripe_descriptors;
NVMStripeBufferControl *nvm_stripe_control;
NVMStripeBufferBucket *nvm_stripe_table;

