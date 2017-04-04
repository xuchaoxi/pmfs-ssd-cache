/*************************************************************************
	> File Name: lru.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 04 Apr 2017 03:32:25 PM CST
 ************************************************************************/

#ifndef _LRU_H
#define _LRU_H

#define DEBUG 0

typedef struct 
{
    long nvm_buf_id; // nvm buffer location in shared buffer
    long next_lru;  // to link used nvm as LRU
    long last_lru;  
} NVMBufferDescForLRU;

typedef struct
{
    long first_lru; // Head of list of LRU
    long last_lru;  // Tail of list of LRU
} NVMBufferControlForLRU;

NVMBufferDescForLRU *nvm_buffer_descriptor_lru;
NVMBufferControlForLRU *nvm_buffer_control_lru;

extern void initSSDBufferForLRU();
extern void *hitInLRUBuffer(NVMBufferDesc *nvm_buf_hdr);
extern NVMBufferDesc *getLRUBuffer();


#endif
