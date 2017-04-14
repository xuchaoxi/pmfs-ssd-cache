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
static volatile void *addToFIFOTail(NVMStripeBufferDescForFIFO *nvm_buf_hdr_fifo);
static volatile void *deleteFromFIFO(NVMStripeBufferDescForFIFO *nvm_buf_hdr_fifo);
static volatile void *moveToFIFOTail(NVMStripeBufferDescForFIFO *nvm_buf_hdr_fifo);

void initNVMStripeBufferForFIFO()
{
    nvm_stripe_control_fifo = (NVMStripeBufferControlForFIFO*)malloc(sizeof(NVMStripeBufferControlForFIFO));
    nvm_stripe_control_fifo->first_fifo = -1;
    nvm_stripe_control_fifo->last_fifo = -1;
    
    nvm_stripe_descriptors_fifo = (NVMStripeBufferDescForFIFO*)malloc(sizeof(NVMStripeBufferDescForFIFO)*STRIPES);
    NVMStripeBufferDescForFIFO *nvm_buf_hdr_fifo = nvm_stripe_descriptors_fifo;
    long i ;
    for(i = 0; i < STRIPES; ++i)
    {
        nvm_buf_hdr_fifo->stripe_buf_id = i;
        nvm_buf_hdr_fifo->next_fifo = -1;
        nvm_buf_hdr_fifo->last_fifo = -1;
        nvm_buf_hdr_fifo++;
    }
}

static volatile void *addToFIFOTail(NVMStripeBufferDescForFIFO *nvm_buf_hdr_fifo)
{
    nvm_stripe_descriptors_fifo[nvm_stripe_control_fifo->last_fifo].next_fifo = nvm_buf_hdr_fifo->stripe_buf_id;
    nvm_buf_hdr_fifo->last_fifo = nvm_stripe_descriptors_fifo[nvm_stripe_control_fifo->last_fifo].stripe_buf_id;
    nvm_stripe_control_fifo->last_fifo = nvm_buf_hdr_fifo->stripe_buf_id;
    return NULL;
}

static volatile void *deleteFromFIFO(NVMStripeBufferDescForFIFO *nvm_buf_hdr_fifo)
{
    nvm_stripe_control_fifo->first_fifo = nvm_buf_hdr_fifo->next_fifo;
    nvm_buf_hdr_fifo->next_fifo = -1;
    nvm_stripe_descriptors_fifo[nvm_stripe_control_fifo->first_fifo].last_fifo = -1;
    return NULL;
}

static volatile void *moveToFIFOTail(NVMStripeBufferDescForFIFO *nvm_buf_hdr_fifo)
{
    deleteFromFIFO(nvm_buf_hdr_fifo);
    addToFIFOTail(nvm_buf_hdr_fifo);
    return NULL;
}

NVMBufferDesc *getFIFOStripeBuffer(NVMBufferTag nvm_buf_tag)
{
    NVMBufferDesc *nvm_buf_hdr;
    NVMStripeBufferDesc *nvm_stripe_hdr;
    NVMStripeBufferDescForFIFO *nvm_stripe_hdr_fifo;
    unsigned long hashcode = nvmStripeTableHashCode(nvm_buf_tag.stripe_id);
    long stripe_buf_id = nvmStripeTableLookup(nvm_buf_tag.stripe_id, hashcode);
    if(nvm_buffer_control->first_freenvm >= 0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
        // miss in stripe buffer
        if(stripe_buf_id < 0)
        {
            nvm_stripe_hdr = getFIFOStripe();
            nvmStripeTableInsert(nvm_buf_tag.stripe_id, hashcode, nvm_stripe_hdr->stripe_buf_id);
        }
        nvm_buf_hdr->next_freenvm = -1;
        nvm_buffer_control->n_usednvm++;
    } 
    else {
        nvm_stripe_hdr = &nvm_stripe_descriptors[nvm_stripe_control_fifo->first_fifo];
        nvm_stripe_hdr_fifo = &nvm_stripe_descriptors_fifo[nvm_stripe_control_fifo->first_fifo];
        
        flushNVMStripeBuffer(nvm_stripe_hdr);
        unsigned long oldhash = nvmStripeTableHashCode(nvm_stripe_hdr->stripe_id);
        nvmStripeTableDelete(nvm_stripe_hdr->stripe_id, oldhash);
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
    }
    return nvm_buf_hdr;
}

static NVMStripeBufferDesc *getFIFOStripe()
{
    NVMStripeBufferDesc *nvm_stripe_hdr;
    NVMStripeBufferDescForFIFO *nvm_stripe_hdr_fifo;
    if(nvm_stripe_control->first_freebuf >= 0)
    {
        nvm_stripe_hdr = &nvm_stripe_descriptors[nvm_stripe_control->first_freebuf];
        nvm_stripe_hdr_fifo = &nvm_stripe_descriptors_fifo[nvm_stripe_control->first_freebuf];
        nvm_stripe_control->first_freebuf = nvm_stripe_hdr->next_freebuf;
        nvm_stripe_hdr->next_freebuf = -1;
        addToFIFOTail(nvm_stripe_hdr_fifo);
        nvm_stripe_control->n_usedbuf++;
    }
    else {
        nvm_stripe_hdr = &nvm_stripe_descriptors[nvm_stripe_control_fifo->first_fifo];
        nvm_stripe_hdr_fifo = &nvm_stripe_descriptors[nvm_stripe_control_fifo->first_fifo];
        moveToFIFOTail(nvm_stripe_hdr_fifo);
        flushNVMStripeBuffer(nvm_stripe_hdr);
        unsigned long hashcode = nvmStripeTableHashCode(nvm_stripe_hdr->stripe_id);
        nvmStripeTableDelete(nvm_stripe_hdr->stripe_id, hashcode);
    }
    return nvm_stripe_hdr;
}

void *hitInFIFOStripeBuffer(NVMBufferDesc *nvm_buf_hdr)
{
    return NULL;
}

