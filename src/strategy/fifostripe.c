/*************************************************************************
	> File Name: fifostripe.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 13 Apr 2017 06:35:21 PM CST
 ************************************************************************/

#include<stdio.h>
#include "nvm_cache.h"
#include "nvm_buf_table.h"
#include "nvm_stripe_table.h"

void initNVMStripeBufferForFIFO()
{

}

NVMBufferDesc *getFIFOStripeBuffer(NVMBufferTag nvm_buf_tag)
{
    NVMBufferDesc *nvm_buf_hdr;
    NVMStripeBufferDesc *nvm_stripe_hdr;
    unsigned long hashcode = nvmStripeTableHashCode(nvm_buf_tag.stripe_id);
    long stripe_buf_id = nvmStripeTableLookup(nvm_buf_tag.stripe_id, hashcode);
    if(nvm_buffer_control->first_freenvm >= 0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
        // miss in stripe buffer
        if(stripe_buf_id < 0)
        {
            nvm_stripe_hdr = &nvm_stripe_descriptors[nvm_stripe_control->first_freebuf];
            nvm_stripe_control->first_freebuf = nvm_stripe_hdr->next_freebuf;
            nvm_stripe_control->n_usedbuf++;
            nvm_stripe_hdr->next_freebuf = -1;
           // nvmStripeTableInsert(nvm_buf_tag.stripe_id, hashcode, nvm_stripe_hdr->stripe_buf_id);
        }
        nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
        nvm_buf_hdr->next_freenvm = -1;
        nvm_buffer_control->n_usednvm++;
    } 
    else {
        nvm_stripe_hdr = &nvm_stripe_descriptors[nvm_stripe_control->last_freebuf];
        flushNVMStripeBuffer(nvm_stripe_hdr);
        unsigned long oldhash = nvmStripeTableHashCode(nvm_stripe_hdr->stripe_id);
        nvmStripeTableDelete(nvm_stripe_hdr->stripe_id, oldhash);
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
    }
    return nvm_buf_hdr;
}

void *hitInFIFOStripeBuffer(NVMBufferDesc *nvm_buf_hdr)
{
    return NULL;
}

