/*************************************************************************
	> File Name: sortstripe.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 30 May 2017 07:40:44 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm_cache.h"
#include "nvm_stripe_table.h"
#include "sortstripe.h"

void initNVMStripeBufferForSort()
{
    initNVMStripeBuffer();
    nvm_stripe_control_sort = (NVMStripeBufferControlForSort*)malloc(sizeof(NVMStripeBufferControlForSort));
    nvm_stripe_control_sort->first_sort = -1;
    nvm_stripe_control_sort->last_sort = -1;

    nvm_stripe_descriptors_sort = (NVMStripeBufferDescForSort*)malloc(sizeof(NVMStripeBufferDescForSort)*STRIPES);
    NVMStripeBufferDescForSort *nvm_buf_hdr_sort;
    long i;
    nvm_buf_hdr_sort = nvm_stripe_descriptors_sort;
    for(i = 0;i < STRIPES;++i)
    {
        nvm_buf_hdr_sort->stripe_buf_id = i;
        num_buf_hdr_sort->hit_num = 0;
        num_buf_hdr_sort->hit_avg = 0.0;
        nvm_buf_hdr_sort->next_sort = -1;
        nvm_buf_hdr_sort->last_sort = -1;
        ++nvm_buf_hdr_sort;
    }
}

static volatile void *addToSortStripe(NVMStripeBufferDescForSort *nvm_buf_hdr_sort)
{
    if(nvm_stripe_control->n_usedbuf==0)
    {
        nvm_stripe_control_sort->first_sort = nvm_buf_hdr_sort->stripe_buf_id;
        nvm_stripe_control_sort->last_sort = nvm_buf_hdr_sort->stripe_buf_id;
    }
    else {
        nvm_buf_hdr_sort->next_sort = nvm_stripe_descriptors_sort[].stripe_buf_id;
        nvm_buf_hdr_sort->last_sort = nvm_stripe_descriptors_sort[].stripe_buf_id;
    }
}

static volatile void *deleteFromSortStripe(NVMStripeBufferDescForSort *nvm_buf_hdr_sort)
{
    if(nvm_buf_hdr_sort->last_sort >= 0)
    {
        nvm_stripe_descriptors_sort[nvm_buf_hdr_sort->last_sort].next_sort = nvm_buf_hdr_sort->next_sort;
    }
    else {
        nvm_stripe_control_sort->first_sort = nvm_buf_hdr_sort->next_sort;
    }
    if(nvm_buf_hdr_sort->next_sort >= 0)
    {
        nvm_stripe_descriptors_sort[nvm_buf_hdr_sort->next_sort].last_sort = nvm_buf_hdr_sort->last_sort;
    }
    else {
        nvm_stripe_control_sort->last_sort = nvm_buf_hdr_sort->last_sort;
    }
    return NULL;
}

static volatile void *moveToSortStripe(NVMStripeBufferDescForSort *nvm_buf_hdr_sort)
{
    if(nvm_stripe_control->n_usedbuf > 1)
    {
        deleteFromSortStripe(nvm_buf_hdr_sort);
        addToSortStripe(nvm_buf_hdr_sort);
    }
    return NULL;
}

NVMBufferDesc *getSortStripeBuffer(NVMBufferTag nvm_buf_tag)
{
    NVMBufferDesc *nvm_buf_hdr;
    NVMStripeBufferDesc *nvm_stripe_hdr;
    NVMStripeBufferDescForSort *nvm_stripe_hdr_sort;
    unsigned long hashcode = nvmStripeTableHashCode(nvm_buf_tag.stripe_id);
    long stripe_buf_id = nvmStripeTableLookup(nvm_buf_tag.stripe_id, hashcode);

    if(stripe_buf_id >= 0)
    {
        hit_stripe++;
        nvm_stripe_hdr_sort = &nvm_stripe_descriptors_sort[stripe_buf_id];
        nvm_stripe_hdr_sort->hit_num++;
        moveToSortStripe(nvm_stripe_hdr_sort);
    }
    else {
        nvm_stripe_hdr = getSortStripe();
        nvmStripeTableInsert(nvm_buf_tag.stripe_id, hashcode, nvm_stripe_hdr->stripe_buf_id);
        nvm_stripe_hdr->stripe_id = nvm_buf_tag.stripe_id;
    }

    if(nvm_buffer_control->first_freenvm >= 0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
    }
    else {
        nvm_stripe_hdr = &nvm_stripe_descriptors[nvm_stripe_control_sort->last_sort];
        nvm_stripe_hdr_sort = &nvm_stripe_descriptors_sort[nvm_stripe_control_sort->last_sort];
        flushNVMStripeBuffer(nvm_stripe_hdr);
        unsigned long oldhash = nvmStripeTableHashCode(nvm_stripe_hdr->stripe_id);
        nvmStripeTableDelete(nvm_stripe_hdr->stripe_id, oldhash);

        nvm_stripe_control_sort->last_sort = nvm_stripe_hdr_sort->last_sort;
        deleteFromSortStripe(nvm_stripe_hdr_sort);
        nvm_stripe_descriptors[nvm_stripe_control->last_freebuf].next_freebuf = nvm_stripe_hdr->stripe_buf_id;
        nvm_stripe_control->last_freebuf = nvm_stripe_hdr->stripe_buf_id;
        nvm_stripe_control->n_usedbuf--;

        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
    }
    nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
    nvm_buf_hdr->next_freenvm = -1;
    nvm_buffer_control->n_usednvm++;
    return nvm_buf_hdr;
}

void *hitInSortStripeBuffer(NVMBufferDesc *nvm_buf_hdr)
{
    NVMStripeBufferDesc *nvm_stripe_hdr;
    NVMStripeBufferDescForSort *nvm_stripe_hdr_sort;
    long stripe_id = nvm_buf_hdr->nvm_buf_tag.stripe_id;
    unsigned long hashcode = nvmStripeTableHashCode(stripe_id);
    long stripe_buf_id = nvmStripeTableLookup(stripe_id, hashcode);
    if(stripe_buf_id >= 0)
    {
        hit_stripe++;
    }
    return NULL;
}
