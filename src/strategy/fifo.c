/*************************************************************************
	> File Name: fifo.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 13 Apr 2017 01:34:38 PM CST
 ************************************************************************/

#include<stdio.h>
#include "nvm_cache.h"
#include "nvm_buf_table.h"

void initNVMBufferForFIFO()
{

}

NVMBufferDesc *getFIFOBuffer()
{
    NVMBufferDesc *nvm_buf_hdr;
    if(nvm_buffer_control->first_freenvm>=0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
        nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
        nvm_buf_hdr->next_freenvm = -1;
        nvm_buffer_control->n_usednvm++;
    }
    else {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->last_freenvm];
        flushNVMBuffer(nvm_buf_hdr);
        unsigned long hashcode = nvmBufferTableHashCode(&nvm_buf_hdr->nvm_buf_tag);
        nvmBufferTableDelete(&nvm_buf_hdr->nvm_buf_tag, hashcode);
    }
    return nvm_buf_hdr;
}

void *hitInFIFOBuffer(NVMBufferDesc *nvm_buf_hdr)
{

    return NULL;
}

