/*************************************************************************
	> File Name: fifostripe.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 13 Apr 2017 07:02:36 PM CST
 ************************************************************************/

#ifndef _FIFOSTRIPE_H
#define _FIFOSTRIPE_H
extern void initNVMBufferForFIFO();
extern NVMBufferDesc *getFIFOStripeBuffer(NVMBufferTag nvm_buf_tag);
extern void *hitInFIFOStripeBuffer(NVMBufferDesc *nvm_buf_hdr);
#endif
