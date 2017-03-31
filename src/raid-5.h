/*************************************************************************
	> File Name: raid-5.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 30 Mar 2017 04:52:52 PM CST
 ************************************************************************/

#define PAGESIZE 4096  // page size
#define PAGENUM 128  // how many pages in a block


void initGlobalVirtualAddr();

void initRaidVirtualAddr();

void execute(long id);



extern long GBLKNUM;  // Global block numbers
extern long RBLKNUM;  // Raid block numbers
extern long BLKSIZE;  // block size
extern long rotate_width;  // rotate width for stripeu

extern int data_ssd_id; // ssd id for data
extern int parity_ssd_id; // ssd id for parity
extern int block_offset_stripe; // the block offset in a stripe at global address
extern long global_stripe_id; // the Nth stripe
extern long global_block_id; // the Nth block in the global virtual address

extern char *ssd_path; // ssd path
extern char *block_buf; // block
