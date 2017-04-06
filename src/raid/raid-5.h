/*************************************************************************
	> File Name: raid-5.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 30 Mar 2017 04:52:52 PM CST
 ************************************************************************/

#define PAGENUM 128  // how many pages in a block
#define N 4  // ssd num for data

#include<sys/types.h>

void initPageBuffer();
void execute(long pageid);
int writeOrReadPage(int ssd_id, int flag);
void directWrite(long pageid);

extern size_t BLKSIZE;  // block size 128 * 4096
extern size_t PAGESIZE; // page size 4096
extern long rotate_width;  // rotate width for stripeu

extern int data_ssd_id; // ssd id for data
extern int parity_ssd_id; // ssd id for parity
extern int block_offset_stripe; // the block offset in a stripe at global address
extern long global_stripe_id; // the Nth stripe
extern long global_block_id; // the Nth block in the global virtual address
extern size_t page_off; // page offset in a block
extern size_t ssd_page_off; // page offset at Nth ssd
extern long raid_offset; // offset in the raid-5 address

extern char *ssd_path; // ssd path
extern char *page_buf; // page buffer




