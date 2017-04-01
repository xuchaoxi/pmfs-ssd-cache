/*************************************************************************
	> File Name: nvm-cache.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 04:52:02 PM CST
 ************************************************************************/

#ifndef _NVM-CACHE_H
#define _NVM-CACHE_H

typedef struct
{
    off_t nvm_buf_tag;  // nvm bufffer offset
    long nvm_buf_id;  
    unsigned nvm_buf_flag;  // ?
    long next_freenvm;     // to link next free nvm
} NVMBufferDesc;


typedef struct NVMBufferHashBucket
{
    off_t hash_key;
    long nvm_buf_id;
    struct NVMBufferHashBucket *next_item;
}

typedef struct
{
    long n_usednvm;  // to evict
    long first_freenvm;  // head of free nvm list
    long last_freenvm;  // tail of free nvm list
}NVMBufferControl;

extern NVMBufferDesc *nvm_buffer_descriptors;  // describe nvm addr
extern NVMBufferHashBucket *nvm_buffer_hashtable;
extern NVMBufferControl *nvm_buffer_control;
extern unsigned long hit_num;
extern unsigned long flush_nvm_blocks;




#endif
