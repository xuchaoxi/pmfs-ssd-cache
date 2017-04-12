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

typedef struct 
{
    long stripe_id;
    long strip_buf_id;
    long next_freelru;
} NVMStripeBufferDesc;

typedef struct 
{
    long n_usedlru;
    long first_freelru;
    long last_freelru;
} NVMStripeBufferControl;

typedef struct NVMStripeBufferBucket
{
    long stripe_id;
    long lru_buf_id;
    struct NVMStripeBufferBucket *next;
} NVMStripeBufferBucket;


NVMStripeBufferDescForLRU *nvm_stripe_descriptors_lru;
NVMStripeBufferControlForLRU *nvm_stripe_control_lru;
NVMStripeBufferBucket *nvm_stripe_table;

NVMStripeBufferControl *nvm_stripe_control;
NVMStripeBufferDesc *nvm_stripe_descriptors;

#define GetNVMStripeBufferBucket(hashcode) ((NVMStripeBufferBucket*)(nvm_stripe_table+(unsigned long)hashcode))

extern void initNVMStripeBufferForLRU();
/*
extern void *hitInLRUStripeBuffer(NVMBufferDesc *nvm_buf_hdr_stripe);
extern NVMBufferDescStripe *getLRUStripeBuffer(NVMBufferTag nvm_buf_tag);
*/
long STRIPES = 50000;




#endif
