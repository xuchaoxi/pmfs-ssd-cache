/*************************************************************************
	> File Name: lrustripe.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 11 Apr 2017 06:50:47 PM CST
 ************************************************************************/

#ifndef _LRUSTRIPE_H
#define _LRUSTRIPE_H

#define DEBUG 0
typedef struct 
{
    long stripe_buf_id;  // lru buffer id
    long next_lru;  // next lru
    long last_lru;  // pre lru
} NVMStripeBufferDescForLRU;

typedef struct
{
    long first_lru;  // Head of list of LRU
    long last_lru;  // Tail of list of LRU
} NVMStripeBufferControlForLRU;


NVMStripeBufferDescForLRU *nvm_stripe_descriptors_lru;
NVMStripeBufferControlForLRU *nvm_stripe_control_lru;

extern unsigned long flush_fifo_times;

extern void initNVMStripeBufferForLRU();
extern NVMBufferDesc *getLRUStripeBuffer(NVMBufferTag nvm_buf_tag);
extern void *hitInLRUStripeBuffer(NVMBufferDesc *);


#endif
