/*************************************************************************
	> File Name: lru.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 04 Apr 2017 08:36:44 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm_cache.h"
#include "nvm_buf_table.h"
#include "lru.h"

static volatile void *addToLRUHead(NVMBufferDescForLRU *nvm_buf_hdr_lru);
static volatile void *deleteFromLRU(NVMBufferDescForLRU *nvm_buf_hdr_lru);
static volatile void *moveToLRUHead(NVMBufferDescForLRU *nvm_buf_hdr_lru);

void initNVMBufferForLRU()
{
    nvm_buffer_control_lru = (NVMBufferControlForLRU*)malloc(sizeof(NVMBufferControlForLRU));
    nvm_buffer_control_lru->first_lru = -1;
    nvm_buffer_control_lru->last_lru = -1;
    
    nvm_buffer_descriptors_lru = (NVMBufferDescForLRU*)malloc(sizeof(NVMBufferDescForLRU)*NNVMBuffers);
    NVMBufferDescForLRU *nvm_buf_hdr_lru;
    long i;
    nvm_buf_hdr_lru = nvm_buffer_descriptors_lru;
    for(i = 0;i < NNVMBuffers;++i)
    {
        nvm_buf_hdr_lru->nvm_buf_id = i;
        nvm_buf_hdr_lru->next_lru = -1;
        nvm_buf_hdr_lru->last_lru = -1;
        ++nvm_buf_hdr_lru;
    }
    flush_fifo_times = 0;
}

static volatile void *addToLRUHead(NVMBufferDescForLRU *nvm_buf_hdr_lru)
{
    if(nvm_buffer_control->n_usednvm==0)
    {
        nvm_buffer_control_lru->first_lru = nvm_buf_hdr_lru->nvm_buf_id;
        nvm_buffer_control_lru->last_lru = nvm_buf_hdr_lru->nvm_buf_id;
    }
    else {
       nvm_buf_hdr_lru->next_lru = nvm_buffer_descriptors_lru[nvm_buffer_control_lru->first_lru].nvm_buf_id;
       nvm_buf_hdr_lru->last_lru = -1;
       nvm_buffer_descriptors_lru[nvm_buffer_control_lru->first_lru].last_lru = nvm_buf_hdr_lru->nvm_buf_id;
       nvm_buffer_control_lru->first_lru = nvm_buf_hdr_lru->nvm_buf_id;
    }
    return NULL;

}

static volatile void *deleteFromLRU(NVMBufferDescForLRU *nvm_buf_hdr_lru)
{
    if(nvm_buf_hdr_lru->last_lru >= 0)
    {
        nvm_buffer_descriptors_lru[nvm_buf_hdr_lru->last_lru].next_lru = nvm_buf_hdr_lru->next_lru;
    }
    else {
        nvm_buffer_control_lru->first_lru = nvm_buf_hdr_lru->next_lru;
    }
    if(nvm_buf_hdr_lru->next_lru >= 0)
    {
        nvm_buffer_descriptors_lru[nvm_buf_hdr_lru->next_lru].last_lru = nvm_buf_hdr_lru->last_lru;
    }
    else {
        nvm_buffer_control_lru->last_lru = nvm_buf_hdr_lru->last_lru;
    }
    return NULL;
}

static volatile void *moveToLRUHead(NVMBufferDescForLRU *nvm_buf_hdr_lru)
{
    deleteFromLRU(nvm_buf_hdr_lru);
    addToLRUHead(nvm_buf_hdr_lru);
    return NULL;
}

NVMBufferDesc *getLRUBuffer()
{
    NVMBufferDesc *nvm_buf_hdr;
    NVMBufferDescForLRU *nvm_buf_hdr_lru;
    if(nvm_buffer_control->first_freenvm>=0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
        nvm_buf_hdr_lru = &nvm_buffer_descriptors_lru[nvm_buffer_control->first_freenvm];
        nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
        nvm_buf_hdr->next_freenvm = -1;
        addToLRUHead(nvm_buf_hdr_lru);
        nvm_buffer_control->n_usednvm++;
        return nvm_buf_hdr;
    }
    flush_fifo_times++;
    nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control_lru->last_lru];
    nvm_buf_hdr_lru = &nvm_buffer_descriptors_lru[nvm_buffer_control_lru->last_lru];
    moveToLRUHead(nvm_buf_hdr_lru);
    
    NVMBufferTag old_tag = nvm_buf_hdr->nvm_buf_tag;
    flushNVMBuffer(nvm_buf_hdr);
    unsigned long old_hash = nvmBufferTableHashCode(&old_tag);
    nvmBufferTableDelete(&old_tag, old_hash);
    return nvm_buf_hdr;
}


void *hitInLRUBuffer(NVMBufferDesc *nvm_buf_hdr)
{
    moveToLRUHead(&nvm_buffer_descriptors_lru[nvm_buf_hdr->nvm_buf_id]);
    return NULL;
}
