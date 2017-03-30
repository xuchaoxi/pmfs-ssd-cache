/*************************************************************************
	> File Name: raid-5.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 30 Mar 2017 04:52:52 PM CST
 ************************************************************************/

#ifndef _RAID-5_H
#define _RAID-5_H


typedef struct {
    long block_id;   // block no
    long stripe_id;   // block belong to strip
    int ssd_id;         // block belong to ssd
    int page_num;    // pages in a block, 128
    int status;       //  block status
    bool isparity;   // data block or parity block
} Block;  // block

typedef struct {
    long stripe_id;   // stripe no
    int block_num;   // blocks in a stripe // num = 5
} Stripe;


typedef struct {
    int ssd_id;  // ssd no
    long block_num;  //  how many blocks in a ssd
} SSD;   // 5 SSDs  SSD[5] 

typedef struct {
    long block_id;  // mark block in global address
    off_t offset;   // specify where the block
    Global_Virtual_Addr *next;  // next block
} Global_Virtual_Addr;

typedef struct  {
    long block_id;   // mark block in raid address
    off_t offset;   // specify where the block 
    Raid_Virtual_Addr *next;  // next block;
} Raid_Virtual_Addr;




#endif
