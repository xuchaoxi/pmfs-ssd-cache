/*************************************************************************
	> File Name: fifo.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 13 Apr 2017 01:57:12 PM CST
 ************************************************************************/

#ifndef _FIFO_H
#define _FIFO_H
extern void initNVMBufferForFIFO();
extern NVMBufferDesc *getFIFOBuffer();
extern void *hitInFIFOBuffer(NVMBufferDesc *nvm_buf_hdr);
#endif
