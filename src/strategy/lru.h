/*************************************************************************
	> File Name: lru.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 04 Apr 2017 03:32:25 PM CST
 ************************************************************************/

#ifndef _LRU_H
#define _LRU_H

extern void *hitInLRUBuffer(NVMBufferDesc *nvm_buf_hdr);
extern NVMBufferDesc *getLRUBuffer();


#endif
