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
    global_block_addr = (Global_Virtual_Addr*)malloc(sizeof(Global_Virtaul_Addr));
    block_id = 0;
    offset = BLKSIZE;
    next = NULL;
}

void initRaidVirtualAddr()
{
    raid_virtual_addr = (Raid_Virtual_Addr*)malloc(sizeof(Raid_Virtual_Addr));
    block_id = 0;
    offset = BLKSIZE;
    next = NULL;
}


