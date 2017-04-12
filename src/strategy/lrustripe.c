/*************************************************************************
	> File Name: lrustripe.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 11 Apr 2017 05:14:08 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "../nvm_cache.h"
#include "nvm_stripe_table.h"
#include "lrustripe.h"

static volatile void *addToLRUStripeHead(NVMStripeBufferDescForLRU *nvm_buf_hdr_lru);
static volatile void *deleteFromLRUStripe(NVMStripeBufferDescForLRU *nvm_buf_hdr_lru);
static volatile void *moveToLRUStripeHead(NVMStripeBufferDescForLRU *nvm_buf_hdr_lru);
NVMStripeBufferDesc *getLRUStripe(NVMBufferTag nvm_buf_tag);

void initNVMStripeBufferForLRU()
{
    nvm_stripe_control_lru = (NVMStripeBufferControlForLRU*)malloc(sizeof(NVMStripeBufferControlForLRU));
    nvm_stripe_control_lru->first_lru = -1;
    nvm_stripe_control_lru->last_lru = -1;

    nvm_stripe_descriptors_lru = (NVMStripeBufferDescForLRU*)malloc(sizeof(NVMStripeBufferDescForLRU)*STRIPES);
    NVMStripeBufferDescForLRU *nvm_buf_hdr_lru;
    long i;
    nvm_buf_hdr_lru = nvm_stripe_descriptors_lru;
    for(i = 0; i < STRIPES; ++i)
    {
        nvm_buf_hdr_lru->stripe_buf_id = i;
        nvm_buf_hdr_lru->next_lru = -1;
        nvm_buf_hdr_lru->last_lru = -1;
        ++nvm_buf_hdr_lru;
    }
}

void initNVMStripeBuffer()
{
    initNVMStripeBufferForLRU();
    initNVMStripeTable(STRIPES);

    nvm_stripe_control = (NVMStripeBufferControl*)malloc(sizeof(NVMStripeBufferControl));
    nvm_stripe_control->n_usedlru = 0;
    nvm_stripe_control->first_freelru = 0;
    nvm_stripe_control->last_freelru = STRIPES-1;

    nvm_stripe_descriptors = (NVMStripeBufferDesc*)malloc(sizeof(NVMStripeBufferDesc)*STRIPES);
    NVMStripeBufferDesc *nvm_stripe_hdr = nvm_stripe_descriptors;
    long i;
    for(i = 0;i < STRIPES;++i)
    {
        nvm_stripe_hdr->stripe_buf_id = i;
        nvm_stripe_hdr->next_freelru = i+1;
        nvm_stripe_hdr++;
    }
    nvm_stripe_descriptors[STRIPES-1].next_freelru = -1;
}

static volatile void *addToLRUStripeHead(NVMStripeBufferDescForLRU *nvm_buf_hdr_lru)
{
    if(nvm_stripe_control->n_usedlru==0)
    {
        nvm_stripe_control_lru->first_lru = nvm_buf_hdr_lru->stripe_buf_id;
        nvm_stripe_control_lru->last_lru = nvm_buf_hdr_lru->stripe_buf_id;
    }
    else {
        nvm_buf_hdr_lru->next_lru = nvm_stripe_descriptors_lru[nvm_stripe_control_lru->first_lru].stripe_buf_id;
        nvm_buf_hdr_lru->last_lru = -1;
        nvm_stripe_descriptors_lru[nvm_stripe_control_lru->first_lru].last_lru = nvm_buf_hdr_lru->stripe_buf_id;
        nvm_stripe_control_lru->first_lru = nvm_buf_hdr_lru->stripe_buf_id;
    }
    return NULL;   
}

static volatile void *deleteFromLRUStripe(NVMStripeBufferDescForLRU *nvm_buf_hdr_lru)
{
    if(nvm_buf_hdr_lru->last_lru >= 0)
    {
        nvm_stripe_descriptors_lru[nvm_buf_hdr_lru->last_lru].next_lru = nvm_buf_hdr_lru->next_lru;
    }
    else {
        nvm_stripe_control_lru->first_lru = nvm_buf_hdr_lru->next_lru;
    }
    if(nvm_buf_hdr_lru->next_lru >= 0)
    {
        nvm_stripe_descriptors_lru[nvm_buf_hdr_lru->next_lru].last_lru = nvm_buf_hdr_lru->last_lru;
    }
    else {
        nvm_stripe_control_lru->last_lru = nvm_buf_hdr_lru->last_lru;
    long nvm_buf_stripe_id;  // nvm buffer stripe id
    }
    return NULL;
}

static volatile void *moveToLRUStripeHead(NVMStripeBufferDescForLRU *nvm_buf_hdr_lru)
{
    deleteFromLRUStripe(nvm_buf_hdr_lru);
    addToLRUHead(nvm_buf_hdr_lru);
    return NULL;
}

NVMBufferDesc *getLRUStripeBuffer(NVMBufferTag nvm_buf_tag)
{
    NVMBufferDesc *nvm_buf_hdr;
    NVMStripeBufferDesc *nvm_stripe_hdr;
    NVMStripeBufferDescForLRU *nvm_stripe_hdr_lru;
    unsigned long hashcode = nvmStripeTableHashCode(nvm_buf_tag.stripe_id);
    long stripe_buf_id = nvmStripeTableLookup(nvm_buf_tag.stripe_id, hashcode);
    if(nvm_buffer_control->first_freenvm >= 0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
        if(stripe_buf_id >= 0)
        {
            nvm_stripe_hdr = &nvm_stripe_descriptors[stripe_buf_id];
            nvm_stripe_hdr_lru = &nvm_stripe_descriptors_lru[stripe_buf_id];
            moveToLRUStripeHead(nvm_stripe_hdr_lru);
        }
        else {
            nvm_stripe_hdr = getLRUStripe(nvm_buf_tag);
            nvmStripeTableInsert(nvm_buf_tag.stripe_id, hashcode, nvm_stripe_hdr->stripe_buf_id);
        }
        nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
        nvm_buf_hdr->next_freenvm = -1;
        nvm_buffer_control->n_usednvm++;
        return nvm_buf_hdr;
    }
    else {
   //     nvm_stripe_hdr_lru = nvm_stripe_descriptors_lru[nvm_stripe_control_lru->last_lru];
 //       flushStripe(nvm_stripe_lru);
    } 

}

NVMStripeBufferDesc *getLRUStripe(NVMBufferTag nvm_buf_tag)
{
    NVMStripeBufferDescForLRU *nvm_stripe_hdr_lru;
    NVMStripeBufferDesc *nvm_stripe_hdr;
    if(nvm_stripe_control->first_freelru >= 0)
    {
        nvm_stripe_hdr = &nvm_stripe_descriptors[nvm_stripe_control->first_freelru];
        nvm_stripe_hdr_lru = &nvm_stripe_descriptors_lru[nvm_stripe_control->first_freelru];
        nvm_stripe_control->first_freelru = nvm_stripe_hdr->next_freelru;
        nvm_stripe_hdr->next_freelru = -1;
        moveToLRUStripeHead(nvm_stripe_hdr_lru);
    }
    return nvm_stripe_hdr;
}


