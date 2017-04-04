/*************************************************************************
	> File Name: nvm-cache.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 04:52:02 PM CST
 ************************************************************************/

#ifndef _NVM-CACHE_H
#define _NVM-CACHE_H

#define DEBUG 0

#define NVM_BUF_VALID 0x01
#define NVM_BUF_DIRTY 0x02

#define bool unsigned char

typedef struct
{
    off_t offset;  // offset at global ssd
  //  int ssd_id;  // at which ssd
} NVMBufferTag;

typedef struct
{
    NVMBufferTag nvm_buf_tag;  // 
    long nvm_buf_id;       // buffer id
    unsigned nvm_buf_flag;  // ?
    long next_freenvm;     // to link next free nvm
} NVMBufferDesc;

typedef enum {
    ClOCK = 0,
    LRU
}NVMEvictionStrategy;

typedef struct NVMBufferHashBucket
{
    NVMBufferTag hash_key;  //
    long nvm_buf_id;  // id at nvm 
  //  int ssd_id;  // belong to which ssd
    struct NVMBufferHashBucket *next;
} NVMBufferHashBucket;

typedef struct
{
    long n_usednvm;  // to evict
    long first_freenvm;  // head of free nvm list
    long last_freenvm;  // tail of free nvm list
} NVMBufferControl;

extern NVMBufferDesc *nvm_buffer_descriptors;  // describe nvm addr
extern NVMBufferHashBucket *nvm_buffer_hashtable;
extern NVMBufferControl *nvm_buffer_control;

extern unsigned long hit_num;
extern unsigned long flush_nvm_blocks;

extern unsigned long NNVMBuffers;   // 50000
extern unsigned long NNVMBufTables;  // 50000
extern size_t NVM_BUFFER_SIZE; // 4096(4KB)

#define GetNVMBufHashBucket(hash_code) ((NVMBufferHashBucket*) (nvm_buffer_hashtable + (unsigned)hash_code) 

extern void initNVMBuffer();
extern void read_block(off_t offset, char* nvm_buffer);
extern void write_block(off_t offset, char* nvm_buffer);
extern void *flushNVMBuffer(NVMBufferDesc *nvm_buf_hdr);


extern NVMEvictionStrategy EvictStrategy;

extern int nvm_fd;




#endif
