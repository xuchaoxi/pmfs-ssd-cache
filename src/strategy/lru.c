/*************************************************************************
	> File Name: lru.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 04 Apr 2017 08:36:44 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "../nvm-cache.h"
#include "lru.h"

static volatile void *addToLRUHead(NVMBufferDescForLRU *nvm_buf_hdr_lru);
static volatile void *deleteFromLRU(NVMBufferDescForLRU *nvm_buf_hdr_lru);
static volatile void *moveToLRUHead(NVMBufferDescForLRU *nvm_buf_hdr_lru);

void initSSDBufferForLRU()
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
}

static volatile void *deleteFromLRU(NVMBufferDescForLRU *nvm_buf_hdr_lru)
{
}

static volatile void *moveToLRUHead(NVMBufferDescForLRU *nvm_buf_hdr_lru)
{
    deleteFromLRU(nvm_buf_hdr_lru);
    addToLRUHead(nvm_buf_hdr_lru);
    return NULL;
}

NVMBufferDesc *getLRUBuffer()
{
}

void *hitInLRUBuffer(NVMBufferDesc *nvm_buf_hdr)
{
    moveToLRUHead(&nvm_buffer_descriptors_lru[nvm_buf_hdr->nvm_buf_id]);
    return NULL;
}
