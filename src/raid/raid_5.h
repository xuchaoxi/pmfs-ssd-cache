/*************************************************************************
	> File Name: raid-5.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 30 Mar 2017 04:52:52 PM CST
 ************************************************************************/

#define PAGENUM 64  // how many pages in a block
#define N 4  // ssd num for data

#include<sys/types.h>

void initPageBuffer();
void execute(long pageid);
int writeOrReadPage(int ssd_id, off_t offset, char *buffer, int flag);
void directWrite(long pageid);

extern size_t BLKSIZE;  // block size 128 * 4096
extern size_t PAGESIZE; // page size 4096
extern long rotate_width;  // rotate width for stripeu

extern int data_ssd_id; // ssd id for data
extern int parity_ssd_id; // ssd id for parity
int block_offset_stripe; // the block offset in a stripe at global address
long global_stripe_id; // the Nth stripe
long global_block_id; // the Nth block in the global virtual address
size_t page_off; // page offset in a block
extern off_t ssd_page_off; // page offset at Nth ssd
extern off_t data_raid_offset; // data offset in the raid-5 address
extern off_t parity_raid_offset; // parity offset in the raid-5 address

extern char *ssd_path; // ssd path
extern char *page_buf; // page buffer

extern char ssd0_device[];
extern char ssd1_device[];
extern char ssd2_device[];
extern char ssd3_device[];
extern char ssd4_device[];


