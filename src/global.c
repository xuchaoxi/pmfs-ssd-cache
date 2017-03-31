/*************************************************************************
	> File Name: global.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Fri 31 Mar 2017 12:40:26 PM CST
 ************************************************************************/

#include "raid-5.h"

long GBLKNUM;  // global block numbers
long RBLKNUM;  // raid block numbers
long BLKSIZE = 128*4096;  // block size;
long rotate_width; // rotate width for stripe


