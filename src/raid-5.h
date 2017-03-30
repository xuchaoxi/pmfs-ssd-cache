/*************************************************************************
	> File Name: raid-5.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 30 Mar 2017 04:52:52 PM CST
 ************************************************************************/

#ifndef _RAID-5_H
#define _RAID-5_H
#endif

typedef struct {
    long block_id;
    off_t offset;
    char ssd_id;
    long page_num;
} Global_Virtual_Addr;

typedef struct  {
    long block_id;
    off_t offset;
    char ssd_no;
    long page_num;
    bool isparity;
}Raid_Virtual_Addr;



