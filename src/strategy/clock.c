/*************************************************************************
	> File Name: clock.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sun 16 Apr 2017 12:41:34 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm_cache.h" 
#include "nvm_buf_table.h"
#include "clock.h"

void initNVMBufferForCLOCK()
{
    nvm_buffer_control_clock = (NVMBufferControlForClock*)malloc(sizeof(NVMBufferControlForClock));
    nvm_buffer_control_clock->next_evict = 0;
    
    nvm_buffer_descriptors_clock = (NVMBufferDescForClock*)malloc(sizeof(NVMBufferDescForClock)*NNVMBuffers);
    NVMBufferDescForClock *nvm_buf_hdr_clock = nvm_buffer_descriptors_clock;
    long i;
    for(i = 0;i < NNVMBuffers;++i)
    {
        nvm_buf_hdr_clock->nvm_buf_id = i;
        nvm_buf_hdr_clock->usage_count = 0;
        nvm_buf_hdr_clock++;
    }
}

NVMBufferDesc *getCLOCKBuffer()
{
    NVMBufferDesc *nvm_buf_hdr;
    NVMBufferDescForClock *nvm_buf_hdr_clock;
    if(nvm_buffer_control->first_freenvm >= 0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
        nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
        nvm_buf_hdr->next_freenvm = -1;
        nvm_buffer_control->n_usednvm++;
        return nvm_buf_hdr;
    }
    while(1)
    {
        nvm_buf_hdr_clock = &nvm_buffer_descriptors_clock[nvm_buffer_control_clock->next_evict];
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control_clock->next_evict];
        nvm_buffer_control_clock->next_evict++;
        if(nvm_buffer_control_clock->next_evict >= NNVMBuffers)
        {
            nvm_buffer_control_clock->next_evict = 0;
        }
        if(nvm_buf_hdr_clock->usage_count > 0)
        {
            nvm_buf_hdr_clock->usage_count--;
        }
        else {
            NVMBufferTag old_tag = nvm_buf_hdr->nvm_buf_tag;
            flushNVMBuffer(nvm_buf_hdr);
            unsigned long old_hash = nvmBufferTableHashCode(&old_tag);
            nvmBufferTableDelete(&old_tag, old_hash);
            return nvm_buf_hdr;
        }
    }
    return NULL;
}

void *hitInCLOCKBuffer(NVMBufferDesc *nvm_buf_hdr)
{
    NVMBufferDescForClock *nvm_buf_hdr_clock;
    nvm_buf_hdr_clock = &nvm_buffer_descriptors_clock[nvm_buf_hdr->nvm_buf_id];
    nvm_buf_hdr_clock->usage_count++;
    return NULL;
}
