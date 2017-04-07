/*************************************************************************
	> File Name: global.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Fri 31 Mar 2017 12:40:26 PM CST
 ************************************************************************/

#include "raid/raid-5.h"
#include "nvm-cache.h"
#include "main.h"

size_t BLKSIZE = 524288;  //128*4096;  // block size;
size_t PAGESIZE = 4096;  // page size
long rotate_width = 100; // rotate width for stripe

char *ssd_path; 

int data_ssd_id;
int parity_ssd_id;
off_t ssd_page_off; 
off_t data_raid_offset;
off_t prity_raid_offset;

char *page_buf;


unsigned long NNVMBuffers = 50000;
unsigned long NNVMBufferTables = 50000;

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



unsigned long flush_fifo_times;

