/*************************************************************************
	> File Name: clockstripe.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sun 16 Apr 2017 10:17:44 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm_stripe_table.h"
#include "nvm_cache.h"
#include "clockstripe.h"

extern void initNVMStripeBuffer();
static NVMStripeBufferDesc *getCLOCKStripe();
static void *evictCLOCKStripeBuffer();

void initNVMStripeBufferForClock()
{
    initNVMStripeBuffer();
    nvm_stripe_control_clock = (NVMStripeControlForClock*)malloc(sizeof(NVMStripeControlForClock));
    nvm_stripe_control_clock->next_evict = 0;
    nvm_stripe_descriptors_clock = (NVMStripeBufferDescForClock*)malloc(sizeof(NVMStripeBufferDescForClock)*STRIPES);

    NVMStripeBufferDescForClock *nvm_stripe_hdr_clock = nvm_stripe_descriptors_clock;
    long i;
    for(i = 0;i < STRIPES;++i)
    {
        nvm_stripe_hdr_clock->stripe_buf_id = i;
        nvm_stripe_hdr_clock->usage_count = 0;
        nvm_stripe_hdr_clock->flag = 0;
        nvm_stripe_hdr_clock++;
    }
}
   
NVMBufferDesc *getCLOCKStripeBuffer(NVMBufferTag nvm_buf_tag)
{
    NVMBufferDesc *nvm_buf_hdr;
    NVMStripeBufferDesc *nvm_stripe_hdr;
    NVMStripeBufferDescForClock *nvm_stripe_hdr_clock;
    unsigned long hashcode = nvmStripeTableHashCode(nvm_buf_tag.stripe_id);
    long stripe_buf_id = nvmStripeTableLookup(nvm_buf_tag.stripe_id, hashcode);

    if(nvm_buffer_control->first_freenvm < 0)
        evictCLOCKStripeBuffer();

    nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
    nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
    nvm_buf_hdr->next_freenvm = -1;
    nvm_buffer_control->n_usednvm++;

    // note the sequence 
    // evict then insert
    if(stripe_buf_id >= 0)
    {
        nvm_stripe_hdr_clock = &nvm_stripe_descriptors_clock[stripe_buf_id];
        nvm_stripe_hdr_clock->usage_count++;
        hit_stripe++;
    }
    else {
        nvm_stripe_hdr = getCLOCKStripe();
        nvmStripeTableInsert(nvm_buf_tag.stripe_id, hashcode, nvm_stripe_hdr->stripe_buf_id);
        nvm_stripe_hdr->stripe_id = nvm_buf_tag.stripe_id;
        nvm_stripe_hdr_clock = &nvm_stripe_descriptors_clock[nvm_stripe_hdr->stripe_buf_id];
        nvm_stripe_hdr_clock->flag = 1;
    }

    return nvm_buf_hdr;
}

static NVMStripeBufferDesc *getCLOCKStripe()
{
    NVMStripeBufferDesc *nvm_stripe_hdr;
    if(nvm_stripe_control->first_freebuf < 0)
        evictCLOCKStripeBuffer();

    nvm_stripe_hdr = &nvm_stripe_descriptors[nvm_stripe_control->first_freebuf];
    nvm_stripe_control->first_freebuf = nvm_stripe_hdr->next_freebuf;
    nvm_stripe_hdr->next_freebuf = -1;
    nvm_stripe_control->n_usedbuf++;

    return nvm_stripe_hdr;
}

static void *evictCLOCKStripeBuffer()
{
    NVMStripeBufferDescForClock *nvm_stripe_hdr_clock;
    NVMStripeBufferDesc *nvm_stripe_hdr;
    while(1)
    {
        nvm_stripe_hdr_clock = &nvm_stripe_descriptors_clock[nvm_stripe_control_clock->next_evict];
        nvm_stripe_hdr = &nvm_stripe_descriptors[nvm_stripe_control_clock->next_evict];

        nvm_stripe_control_clock->next_evict++;
        if(nvm_stripe_control_clock->next_evict >= STRIPES)
        {
            nvm_stripe_control_clock->next_evict = 0;
        }

        if(nvm_stripe_hdr_clock->usage_count > 0)
        {
            nvm_stripe_hdr_clock->usage_count--;
        }
        else if(nvm_stripe_hdr_clock->flag==1)
        {
            nvm_stripe_hdr_clock->flag = 0;
            flushNVMStripeBuffer(nvm_stripe_hdr);
            unsigned long oldhash = nvmStripeTableHashCode(nvm_stripe_hdr->stripe_id);
            nvmStripeTableDelete(nvm_stripe_hdr->stripe_id, oldhash);

            nvm_stripe_hdr->next_freebuf = nvm_stripe_control->first_freebuf;
            nvm_stripe_control->first_freebuf = nvm_stripe_hdr->stripe_buf_id;
            nvm_stripe_control->n_usedbuf--;
            break;
        }
    }
    return NULL;
}


void *hitInCLOCKStripeBuffer(NVMBufferDesc *nvm_buf_hdr)
{
    NVMStripeBufferDescForClock *nvm_stripe_hdr_clock;
    long stripe_id = nvm_buf_hdr->nvm_buf_tag.stripe_id;
    unsigned long hashcode = nvmStripeTableHashCode(stripe_id);
    long stripe_buf_id = nvmStripeTableLookup(stripe_id, hashcode);
    if(stripe_buf_id >= 0)
    {
        hit_stripe++;
        nvm_stripe_hdr_clock = &nvm_stripe_descriptors_clock[stripe_buf_id];
        nvm_stripe_hdr_clock->usage_count++;
    }
    return NULL;
}
