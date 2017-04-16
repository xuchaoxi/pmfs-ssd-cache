/*************************************************************************
	> File Name: clockstripe.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sun 16 Apr 2017 10:06:46 AM CST
 ************************************************************************/

#ifndef _CLOCKSTRIPE_H
#define _CLOCKSTRIPE_H

typedef struct 
{
    long stripe_buf_id;
    unsigned long usage_count;
    int flag;  // 0 for not used
} NVMStripeBufferDescForClock;

typedef struct
{
    long next_evict;
} NVMStripeControlForClock;

NVMStripeBufferDescForClock *nvm_stripe_descriptors_clock;
NVMStripeControlForClock *nvm_stripe_control_clock;

extern void initNVMStripeBufferForClock();
extern NVMBufferDesc *getCLOCKStripeBuffer(NVMBufferTag nvm_buf_tag);
extern void *hitInCLOCKStripeBuffer();

#endif
