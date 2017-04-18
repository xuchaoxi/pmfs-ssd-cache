/*************************************************************************
	> File Name: scan.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 18 Apr 2017 07:34:16 PM CST
 ************************************************************************/

#ifndef _SCAN_H
#define _SCAN_H

typedef struct 
{
    long nvm_buf_id;
    long next_scan;
    long last_scan;
} NVMBufferDescForSCAN;

typedef struct
{
    long start;
    long scan_ptr;
} NVMBufferControlForSCAN;

NVMBufferDescForSCAN *nvm_buffer_descriptors_scan;
NVMBufferControlForSCAN *nvm_buffer_control_scan;

extern void initNVMBufferForSCAN();
extern NVMBufferDesc *getSCANBuffer();
extern void *hitInSCANBuffer(NVMBufferDesc *);
extern void insertSCAN();

#endif
