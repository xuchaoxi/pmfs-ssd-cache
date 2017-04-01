/*************************************************************************
	> File Name: nvm-cache.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 05:04:26 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

void initNVMBuffer()
{
    nvm_buffer_descriptors = (NVMBufferDesc*)malloc(sizeof(NVMBufferDesc)*NNMVBuffers);
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


