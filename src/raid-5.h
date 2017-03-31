/*************************************************************************
	> File Name: raid-5.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 30 Mar 2017 04:52:52 PM CST
 ************************************************************************/

#ifndef _RAID-5_H
#define _RAID-5_H

#define PAGESIZE 4096  // page size
#define PAGENUM 128  // how many pages in a block


void initGlobalVirtualAddr();

void initRaidVirtualAddr();

void execute();


extern long GBLKNUM;  // Global block numbers
extern long RBLKNUM;  // Raid block numbers
extern long BLKSIZE;  // block size
extern long rotate_width;  // rotate width for stripe

#endif
