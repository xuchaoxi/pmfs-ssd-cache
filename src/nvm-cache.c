/*************************************************************************
	> File Name: nvm-cache.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 05:04:26 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm-cache.h"
#include "nvm-buf-table.h"

void initNVMBuffer()
{
    nvm_buffer_descriptors = (NVMBufferDesc*)malloc(sizeof(NVMBufferDesc)*NNVMBuffers);
    NVMBufferDesc *nvm_buf_hdr;
    long i;
    nvm_buf_hdr = nvm_buffer_descriptors;
    for(i = 0;i < NNVMBuffers;++i)
    {
        nvm_buf_hdr->nvm_buf_id = i;
        nvm_buf_hdr->nvm_buf_flag = 0;
        nvm_buf_hdr->next_freenvm = i+1;
        nvm_buf_hdr++;
    }
    nvm_buf_hdr[NNVMBuffers-1].next_freenvm = -1;

    hit_num = 0;
    flush_nvm_blocks = 0;
}

NVMBufferDesc *NVMBufferAlloc(NVMBufferTag nvm_buf_tag, bool *found)
{
    NVMBufferDesc *nvm_buf_hdr;
    unsigned long nvm_buf_hash = nvmBufferTableHashCode(&nvm_buf_tag);
    long ssd_buf_id = nvmBufferTableLookup(&nvm_buf_tag, nvm_buf_hash);
    
}
    

void read_block(off_t offset, char* nvm_buffer)
{
    void *nvm_buf_block;
    NVMBufferDesc *nvm_buf_hdr;

    bool found = 0;
    int ret;
    if(DEBUG)
        printf("[INFO]:read() ------ offset=%lu\n",offset);
//    nvm_buf_hdr = NVMBufferAlloc(, &found);
}



