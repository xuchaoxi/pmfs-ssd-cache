/*************************************************************************
	> File Name: fifostripe.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 13 Apr 2017 07:02:36 PM CST
 ************************************************************************/

#ifndef _FIFOSTRIPE_H
#define _FIFOSTRIPE_H

typedef struct 
{
    long stripe_buf_id;
    long next_fifo;
    long last_fifo;
} NVMStripeBufferDescForFIFO;

typedef struct
{
    long first_fifo;
    long last_fifo;
} NVMStripeBufferControlForFIFO;

NVMStripeBufferDescForFIFO *nvm_stripe_descriptors_fifo;
NVMStripeBufferControlForFIFO *nvm_stripe_control_fifo;

extern void initNVMBufferForFIFO();
extern NVMBufferDesc *getFIFOStripeBuffer(NVMBufferTag nvm_buf_tag);
extern void *hitInFIFOStripeBuffer(NVMBufferDesc *nvm_buf_hdr);
#endif
