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
    long nvm_buf_stripe_id;  // nvm buffer stripe id
    long next_lru;  // 
    long last_lru; 
} NVMBufferDescForLRUStripe;

typedef struct
{
    long first_lru;  // Head of list of LRU
    long last_lru;  // Tail of list of LRU
} NVMBufferControlForLRUStripe;

typedef struct NVMBufferStripeBucket
{
    long stripe_id;
    long lru_buf_id
    struct NVMBufferStripeBucket *next;
} NVMBufferStripeBucket;


NVMBufferDescForLRUStripe *nvm_buf_descriptors_lru_stripe;
NVMBufferControlForLRUStripe *nvm_buf_control_lru_stripe;
NVMBufferStripeBucket *nvm_buf_stripe_table;

#define GETNVMBufferStripeBucket(hashcode) ((NVMBufferStripeBucket*)(nvm_buf_stripe_table+(unsigned long)hashcode))

extern void initNVMBufferForLRUStripe();
extern void *hitInLRUBuffer(NVMBufferDesc *nvm_buf_hdr_stripe);
extern NVMBufferDesc *getLRUStripeBuffer();

long STRIPE = 50000;




#endif
