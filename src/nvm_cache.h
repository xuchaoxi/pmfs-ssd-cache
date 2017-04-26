/*************************************************************************
	> File Name: nvm-cache.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 04:52:02 PM CST
 ************************************************************************/

#ifndef _NVM_CACHE_H
#define _NVM_CACHE_H

#define DEBUG 0

#define NVM_BUF_VALID 0x01
#define NVM_BUF_DIRTY 0x02

#define bool unsigned char

typedef struct
{
    off_t offset;  // offset at global ssd
    int data_ssd_id;  // data at which ssd
    int parity_ssd_id;  // parity at which ssd
    long stripe_id;  // stripe id
    off_t ssd_offset;  // page offset at ssd
    int flag;  // tag data or parity, data : 1 | parity : 0
} NVMBufferTag;

typedef struct
{
    NVMBufferTag nvm_buf_tag;  // 
    long nvm_buf_id;       // buffer id
//    unsigned nvm_buf_flag;   
    long next_freenvm;     // to link next free nvm
} NVMBufferDesc;

typedef enum {
    CLOCK = 0,
    CLOCKSTRIPE,
    LRU,
    LRUSTRIPE,
    FIFO,
    FIFOSTRIPE,
    SCAN
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

typedef struct 
{
    long stripe_id;
    long stripe_buf_id;  //  buffer id
    long next_freebuf;  
} NVMStripeBufferDesc;

typedef struct NVMStripeBufferBucket
{
    long stripe_id;
    long stripe_buf_id;
    struct NVMStripeBufferBucket *next;
} NVMStripeBufferBucket;

typedef struct 
{
    long n_usedbuf;  // to evict
    long first_freebuf; // head of free buffer
    long last_freebuf;  // tail of free buffer
} NVMStripeBufferControl;

extern NVMBufferDesc *nvm_buffer_descriptors;  // describe nvm addr
extern NVMBufferHashBucket *nvm_buffer_hashtable;
extern NVMBufferControl *nvm_buffer_control;

extern NVMStripeBufferDesc *nvm_stripe_descriptors; 
extern NVMStripeBufferControl *nvm_stripe_control;
extern NVMStripeBufferBucket *nvm_stripe_table;

extern unsigned long hit_num;
extern unsigned long hit_parity;
extern unsigned long hit_data;
extern unsigned long hit_stripe;
extern unsigned long write_blocks;
extern unsigned long flush_blocks;
extern unsigned long flush_data;
extern unsigned long flush_parity;
extern unsigned long flush_stripe;

extern unsigned long NNVMBuffers;   // 50000
extern unsigned long NNVMBufferTables;  // 50000
extern size_t NVM_BUFFER_SIZE; // 4096(4KB)
extern unsigned long STRIPES;  // 50000

#define GetNVMBufferHashBucket(hash_code) ((NVMBufferHashBucket*) (nvm_buffer_hashtable + (unsigned long)hash_code))
#define GetNVMStripeBufferBucket(hashcode) ((NVMStripeBufferBucket*)(nvm_stripe_table + (unsigned long)hashcode))

extern void initNVMBuffer();
extern void read_block(off_t offset, char* nvm_buffer);
extern void write_block(off_t offset, char* nvm_buffer, int flag);
extern void *flushNVMBuffer(NVMBufferDesc *nvm_buf_hdr);
extern void *flushNVMStripeBuffer(NVMStripeBufferDesc *nvm_buf_hdr);


extern NVMEvictionStrategy EvictStrategy;

extern int nvm_fd;

#endif
