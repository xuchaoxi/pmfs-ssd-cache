/*************************************************************************
	> File Name: lrustripe.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 11 Apr 2017 05:14:08 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm_cache.h"
#include "nvm_buf_table.h"
#include "lrustripe.h"

static volatile void *addToLRUStripeHead(NVMBufferDescForLRUStripe *nvm_buf_hdr_lru);
static volatile void *deleteFromLRUStripe(NVMBufferDescForLRUStripe *nvm_buf_hdr_lru);
static volatile void *moveToLRUStripeHead(NVMBufferDescForLRUStripe *nvm_buf_hdr_lru);

void initNVMBufferForLRUStripe()
{
    nvm_buf_control_lru_stripe = (NVMBufferControlForLRUStripe*)malloc(sizeof(NVMBufferControlForLRUStripe));
    nvm_buf_control_lru_stripe->first_lru = -1;
    nvm_buf_control_lru_stripe->last_lru = -1;

    nvm_buf_descriptors_lru_stripe = (NVMBufferDescForLRUStripe*)malloc(sizeof(NVMBufferDescForLRUStripe)*STRIPES);
    NVMBufferDescForLRUStripe *nvm_buf_hdr_lru;
    long i;
    nvm_buf_hdr_lru = nvm_buf_descriptors_lru_stripe;
    for(i = 0; i < STRIPES; ++i)
    {
        nvm_buf_hdr_lru->nvm_buf_stripe_id = i;
        nvm_buf_hdr_lru->next_lru = -1;
        nvm_buf_hdr_lru->last_lru = -1;
        ++nvm_buf_hdr_lru;
    }
    flush_fifo_times = 0;
}

static volatile void *addToLRUStripeHead(NVMBufferDescForLRUStripe *nvm_buf_hdr_lru)
{
    if(nvm_buffer_control->n_usednvm==0)
    {
        nvm_buf_control_lru_stripe->first_lru = nvm_buf_hdr_lru->nvm_buf_stripe_id;
        nvm_buf_control_lru_stripe->last_lru = nvm_buf_hdr_lru->nvm_buf_stripe_id;
    }
    else {
        nvm_buf_hdr_lru->next_lru = nvm_buf_descriptors_lru_stripe[nvm_buf_control_lru_stripe->first_lru].nvm_buf_stripe_id;
        nvm_buf_hdr_lru_stripe->last_lru = -1;
        nvm_buf_descriptors_lru_stripe[nvm_buf_control_lru_stripe->first_lru].last_lru = nvm_buf_hdr_lru->nvm_buf_stripe_id;
        nvm_buf_control_lru_stripe->first_lru = nvm_buf_hdr_lru->nvm_buf_stripe_id;
    }
    return NULL;   
}

static volatile void *deleteFromLRUStripe(NVMBufferDescForLRUStripe *nvm_buf_hdr_lru)
{
    if(nvm_buf_hdr_lru->last_lru >= 0)
    {
        nvm_buf_descriptors_lru[nvm_buf_hdr_lru->last_lru].next_lru = nvm_buf_hdr_lru->next_lru;
    }
    else {
        nvm_buf_control_lru_stripe->first_lru = nvm_buf_hdr_lru->next_lru;
    }
    if(nvm_buf_hdr_lru->next_lru >= 0)
    {
        nvm_buf_descriptors_lru[nvm_buf_hdr_lru->next_lru].last_lru = nvm_buf_hdr_lru->last_lru;
    }
    else {
        nvm_buf_control_lru_stripe->last_lru = nvm_buf_hdr_lru->last_lru;
    }
    return NULL;
}

static volatile void *moveToLRUStripeHead(NVMBufferDescForLRUStripe *nvm_buf_hdr_lru)
{
    deleteFromLRUStripe(nvm_buf_hdr_lru);
    addToLRUHead(nvm_buf_hdr_lru);
    return NULL;
}

NVMBufferDescStripe *getLRUStripeBuffer(NVMBufferTag nvm_buf_tag)
{
    NVMBufferDesc *nvm_buf_hdr;
    NVMBufferDescForLRUStripe *nvm_buf_hdr_lru;
    if(nvm_buffer_control->first_freenvm >= 0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buf_control->first_freenvm];

    
        
        nvm_buf_hdr_lru = &nvm_buf_descriptors_lru_stripe[nvm_buf_tag.stripe_id];  // !!!

        nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
        nvm_buf_hdr->next_freenvm = -1;
        addToLRUStripeHead(nvm_buf_hdr_lru);
        nvm_buffer_control->n_usednvm++;
        return nvm_buf_hdr;
    }
    else {
        flush_fifo_times++;
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control_lru->last_lru];

}

NVMBufferDescStripe *getLRUStripe(NVMBufferTag nvm_buf_tag)
{
    long i;
    for(i = 0;i < STRIPE;++i)
    {
        if(

        
