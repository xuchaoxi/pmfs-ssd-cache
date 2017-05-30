/*************************************************************************
	> File Name: sortstripe.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 30 May 2017 07:35:54 PM CST
 ************************************************************************/

#ifndef _SORTSTRIPE_H
#define _SORTSTRIPE_H

typedef struct 
{
    long stripe_buf_id;
    long next_lru;
    long last_lru;
} NVMStripeBufferDescForSort;

typedef struct 
{
    long first_lru;
    long last_lru;
} NVMStripeBufferControlForSort;

NVMStripeBufferDescForSort *nvm_stripe_descriptors_sort;
NVMStripeBufferControlForSort *nvm_stripe_control_sort;

extern void initNVMStripeBufferForSort();
extern NVMBufferDesc *getSortStripeBuffer(NVMBufferTag nvm_buf_tag);
extern void *hitInSortStripeBuffer(NVMBufferDesc *);
#endif
