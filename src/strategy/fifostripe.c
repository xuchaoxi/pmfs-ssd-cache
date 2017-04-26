/*************************************************************************
	> File Name: fifostripe.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 13 Apr 2017 06:35:21 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm_cache.h"
#include "nvm_stripe_table.h"
#include "fifostripe.h"

static NVMStripeBufferDesc *getFIFOStripe();

extern void initNVMStripeBuffer();

void initNVMStripeBufferForFIFO()
{
    initNVMStripeBuffer();
    head = 0;
    tail = 0;
}

NVMBufferDesc *getFIFOStripeBuffer(NVMBufferTag nvm_buf_tag)
{
    NVMBufferDesc *nvm_buf_hdr;
    NVMStripeBufferDesc *nvm_stripe_hdr;
    unsigned long hashcode = nvmStripeTableHashCode(nvm_buf_tag.stripe_id);
    long stripe_buf_id = nvmStripeTableLookup(nvm_buf_tag.stripe_id, hashcode);

    if(stripe_buf_id < 0)
    {
        nvm_stripe_hdr = getFIFOStripe();
        nvmStripeTableInsert(nvm_buf_tag.stripe_id, hashcode, nvm_stripe_hdr->stripe_buf_id);
        nvm_stripe_hdr->stripe_id = nvm_buf_tag.stripe_id;
    }
    else 
        hit_stripe++;

    if(nvm_buffer_control->first_freenvm >= 0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
        // miss in stripe buffer
    } 
    else {
        nvm_stripe_hdr = &nvm_stripe_descriptors[head];
        head = (head+1)%STRIPES;
        flushNVMStripeBuffer(nvm_stripe_hdr);
        unsigned long oldhash = nvmStripeTableHashCode(nvm_stripe_hdr->stripe_id);
        nvmStripeTableDelete(nvm_stripe_hdr->stripe_id, oldhash);	
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
    }
    nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
    nvm_buf_hdr->next_freenvm = -1;
    nvm_buffer_control->n_usednvm++;
 
    return nvm_buf_hdr;
}

static NVMStripeBufferDesc *getFIFOStripe()
{
    NVMStripeBufferDesc *nvm_stripe_hdr;
    if((tail+1)%STRIPES!=head)
    {
        nvm_stripe_hdr = &nvm_stripe_descriptors[tail];
        tail = (tail + 1)%STRIPES;
        nvm_stripe_control->n_usedbuf++;
    }
    else {
        nvm_stripe_hdr = &nvm_stripe_descriptors[head];
        head = (head+1)%STRIPES;
        flushNVMStripeBuffer(nvm_stripe_hdr);
        unsigned long hashcode = nvmStripeTableHashCode(nvm_stripe_hdr->stripe_id);
        nvmStripeTableDelete(nvm_stripe_hdr->stripe_id, hashcode);
    }
    return nvm_stripe_hdr;
}

void *hitInFIFOStripeBuffer(NVMBufferDesc *nvm_buf_hdr)
{
    hit_stripe++;
    return NULL;
}

