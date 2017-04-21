/*************************************************************************
	> File Name: fifo.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 13 Apr 2017 01:34:38 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm_cache.h"
#include "nvm_buf_table.h"
#include "fifo.h"

static volatile void *addToFIFOTail(NVMBufferDescForFIFO *nvm_buf_hdr_fifo);
static volatile void *deleteFromFIFO(NVMBufferDescForFIFO *nvm_buf_hdr_fifo);
static volatile void *moveToFIFOTail(NVMBufferDescForFIFO *nvm_buf_hdr_fifo);

void initNVMBufferForFIFO()
{
    head = 0;
    tail = 0;
    /*
    nvm_buffer_control_fifo = (NVMBufferControlForFIFO*)malloc(sizeof(NVMBufferControlForFIFO));
    nvm_buffer_control_fifo->first_fifo = -1;
    nvm_buffer_control_fifo->last_fifo = -1;
    
    nvm_buffer_descriptors_fifo = (NVMBufferDescForFIFO*)malloc(sizeof(NVMBufferDescForFIFO)*NNVMBuffers);
    NVMBufferDescForFIFO *nvm_buf_hdr_fifo = nvm_buffer_descriptors_fifo;
    long i ;
    for(i = 0; i < NNVMBuffers; ++i)
    {
        nvm_buf_hdr_fifo->nvm_buf_id = i;
        nvm_buf_hdr_fifo->next_fifo = -1;
        nvm_buf_hdr_fifo++;
    }
    */
}

static volatile void *addToFIFOTail(NVMBufferDescForFIFO *nvm_buf_hdr_fifo)
{
    if(nvm_buffer_control->n_usednvm==0)
    {
        nvm_buffer_control_fifo->first_fifo = nvm_buf_hdr_fifo->nvm_buf_id;
        nvm_buffer_control_fifo->last_fifo = nvm_buf_hdr_fifo->nvm_buf_id;
    }
    else {
        nvm_buffer_descriptors_fifo[nvm_buffer_control_fifo->last_fifo].next_fifo = nvm_buf_hdr_fifo->nvm_buf_id;
        nvm_buf_hdr_fifo->next_fifo = -1;
        nvm_buffer_control_fifo->last_fifo = nvm_buf_hdr_fifo->nvm_buf_id;
    }
    return NULL;
}

static volatile void *deleteFromFIFO(NVMBufferDescForFIFO *nvm_buf_hdr_fifo)
{
    
    if(nvm_buf_hdr_fifo->next_fifo >= 0)
    {
        nvm_buffer_control_fifo->first_fifo = nvm_buf_hdr_fifo->next_fifo;
        nvm_buf_hdr_fifo->next_fifo = -1;
    }
    else {
        nvm_buffer_control_fifo->first_fifo = -1;
        nvm_buffer_control_fifo->last_fifo = -1;
    }
    return NULL;
}

static volatile void *moveToFIFOTail(NVMBufferDescForFIFO *nvm_buf_hdr_fifo)
{
    if(nvm_buffer_control->n_usednvm > 1)
    {
        deleteFromFIFO(nvm_buf_hdr_fifo);
        addToFIFOTail(nvm_buf_hdr_fifo);
    }
    return NULL;
}

NVMBufferDesc *getFIFOBuffer()
{
    NVMBufferDesc *nvm_buf_hdr;
    if(nvm_buffer_control->first_freenvm >= 0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
        tail = (tail + 1)%NNVMBuffers;
        nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
        nvm_buf_hdr->next_freenvm = -1;
        nvm_buffer_control->n_usednvm++;
        return nvm_buf_hdr;
    }
    nvm_buf_hdr = &nvm_buffer_descriptors[head];
    head = (head+1)%NNVMBuffers;
    flushNVMBuffer(nvm_buf_hdr);
    NVMBufferTag old_tag = nvm_buf_hdr->nvm_buf_tag; 
    unsigned long old_hash = nvmBufferTableHashCode(&old_tag);
    nvmBufferTableDelete(&old_tag, old_hash);
    return nvm_buf_hdr;
}

void *hitInFIFOBuffer(NVMBufferDesc *nvm_buf_hdr)
{

    return NULL;
}

