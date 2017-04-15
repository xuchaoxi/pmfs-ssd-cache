/*************************************************************************
	> File Name: fifo.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 13 Apr 2017 01:57:12 PM CST
 ************************************************************************/

#ifndef _FIFO_H
#define _FIFO_H

typedef struct 
{
    long nvm_buf_id;
    long next_fifo;
} NVMBufferDescForFIFO;

typedef struct
{
    long first_fifo;
    long last_fifo;
} NVMBufferControlForFIFO;

NVMBufferDescForFIFO *nvm_buffer_descriptors_fifo;
NVMBufferControlForFIFO *nvm_buffer_control_fifo;

long head, tail;

extern void initNVMBufferForFIFO();
extern NVMBufferDesc *getFIFOBuffer();
extern void *hitInFIFOBuffer(NVMBufferDesc *nvm_buf_hdr);
#endif
