/*************************************************************************
	> File Name: clock.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sun 16 Apr 2017 12:29:18 AM CST
 ************************************************************************/

#ifndef _CLOCK_H
#define _CLOCK_H
typedef struct 
{
    long nvm_buf_id;
    unsigned long usage_count;
} NVMBufferDescForClock;

typedef struct 
{
    long next_evict;
} NVMBufferControlForClock;

NVMBufferDescForClock *nvm_buffer_descriptors_clock;
NVMBufferControlForClock *nvm_buffer_control_clock;

extern void initNVMBufferForCLOCK();
extern NVMBufferDesc *getCLOCKBuffer();
extern void *hitInCLOCKBuffer(NVMBufferDesc *);


#endif
