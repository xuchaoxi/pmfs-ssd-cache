/*************************************************************************
	> File Name: nvm_cache.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 05:04:26 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<memory.h>
#include "raid/raid_5.h"
#include "nvm_cache.h"
#include "nvm_buf_table.h"
#include "nvm_stripe_table.h"
#include "strategy/clock.h"
#include "strategy/clockstripe.h"
#include "strategy/lru.h"
#include "strategy/lrustripe.h"
#include "strategy/fifo.h"
#include "strategy/fifostripe.h"
#include "strategy/scan.h"

static NVMBufferDesc *NVMBufferAlloc(NVMBufferTag nvm_buf_tag, bool *found);
static void *initStrategyNVMBuffer(NVMEvictionStrategy strategy);
static NVMBufferDesc *getStrategyNVMBuffer(NVMBufferTag nvm_buf_tag, NVMEvictionStrategy strategy);
static void *hitInNVMBuffer(NVMBufferDesc *nvm_buf_hdr, NVMEvictionStrategy strategy);

void initNVMBuffer()
{
    // init buffer strategy
    initStrategyNVMBuffer(EvictStrategy);
    // init hash table
    initNVMBufferTable(NNVMBufferTables);

    nvm_buffer_control = (NVMBufferControl*)malloc(sizeof(NVMBufferControl));
    nvm_buffer_control->n_usednvm = 0;
    nvm_buffer_control->first_freenvm = 0;
    nvm_buffer_control->last_freenvm = NNVMBuffers-1;

    nvm_buffer_descriptors = (NVMBufferDesc*)malloc(sizeof(NVMBufferDesc)*NNVMBuffers);
    NVMBufferDesc *nvm_buf_hdr;
    long i;
    nvm_buf_hdr = nvm_buffer_descriptors;
    for(i = 0;i < NNVMBuffers;++i)
    {
        nvm_buf_hdr->nvm_buf_id = i;
        nvm_buf_hdr->next_freenvm = i+1;
        nvm_buf_hdr++;
    }
    nvm_buffer_descriptors[NNVMBuffers-1].next_freenvm = -1;

    hit_num = 0;
    hit_data = 0;
    hit_parity = 0;
    hit_stripe = 0;
    write_blocks = 0;
    flush_blocks = 0;
    flush_data = 0;
    flush_parity = 0;
    flush_stripe = 0;
}

// stripe buffer
void initNVMStripeBuffer()
{

//    initStrategyNVMBuffer(EvictStrategy);
    initNVMStripeTable(STRIPES);

    nvm_stripe_control = (NVMStripeBufferControl*)malloc(sizeof(NVMStripeBufferControl));
    nvm_stripe_control->n_usedbuf = 0;
    nvm_stripe_control->first_freebuf = 0;
    nvm_stripe_control->last_freebuf = STRIPES-1;

    nvm_stripe_descriptors = (NVMStripeBufferDesc*)malloc(sizeof(NVMStripeBufferDesc)*STRIPES);
    NVMStripeBufferDesc *nvm_stripe_hdr = nvm_stripe_descriptors;
    long i;
    for(i = 0;i < STRIPES;++i)
    {
        nvm_stripe_hdr->stripe_buf_id = i;
        nvm_stripe_hdr->next_freebuf = i+1;
        nvm_stripe_hdr++;
    }
    nvm_stripe_descriptors[STRIPES-1].next_freebuf = -1;
}

static void *initStrategyNVMBuffer(NVMEvictionStrategy strategy)
{
    if(strategy==LRU)
        initNVMBufferForLRU();
    if(strategy==LRUSTRIPE)
        initNVMStripeBufferForLRU();
    if(strategy==FIFO)
        initNVMBufferForFIFO();
    if(strategy==FIFOSTRIPE)
        initNVMStripeBufferForFIFO();
    if(strategy==CLOCK)
        initNVMBufferForCLOCK();
    if(strategy==CLOCKSTRIPE)
        initNVMStripeBufferForClock();
    if(strategy==SCAN)
        initNVMBufferForSCAN();
    return NULL;
}

static NVMBufferDesc *getStrategyNVMBuffer(NVMBufferTag nvm_buf_tag, NVMEvictionStrategy strategy)
{
    if(strategy==CLOCK)
        return getCLOCKBuffer();
    if(strategy==CLOCKSTRIPE)
        return getCLOCKStripeBuffer(nvm_buf_tag);
    if(strategy==LRU)
        return getLRUBuffer();
    if(strategy==LRUSTRIPE)
        return getLRUStripeBuffer(nvm_buf_tag);
    if(strategy==FIFO)
        return getFIFOBuffer();
    if(strategy==FIFOSTRIPE)
        return getFIFOStripeBuffer(nvm_buf_tag);
    if(strategy==SCAN)
        return getSCANBuffer();
    return NULL;
}

static void *hitInNVMBuffer(NVMBufferDesc *nvm_buf_hdr, NVMEvictionStrategy strategy)
{
    if(strategy==LRU)
        hitInLRUBuffer(nvm_buf_hdr);
    if(strategy==LRUSTRIPE)
        hitInLRUStripeBuffer(nvm_buf_hdr);
    if(strategy==FIFO)
        hitInFIFOBuffer(nvm_buf_hdr);
    if(strategy==FIFOSTRIPE)
        hitInFIFOStripeBuffer(nvm_buf_hdr);
    if(strategy==CLOCK)
        hitInCLOCKBuffer(nvm_buf_hdr);
    if(strategy==CLOCKSTRIPE)
        hitInCLOCKStripeBuffer(nvm_buf_hdr);
    if(strategy==SCAN)
        hitInSCANBuffer(nvm_buf_hdr);
    return NULL;
}

static NVMBufferDesc *NVMBufferAlloc(NVMBufferTag nvm_buf_tag, bool *found)
{
    NVMBufferDesc *nvm_buf_hdr;
    unsigned long nvm_buf_hash = nvmBufferTableHashCode(&nvm_buf_tag);
    long nvm_buf_id = nvmBufferTableLookup(&nvm_buf_tag, nvm_buf_hash);
    if(nvm_buf_id >= 0)
    {
        hit_num++;
        if(nvm_buf_tag.data_ssd_id < 0)
            hit_parity++;
        else 
            hit_data++;
        *found = 1;
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buf_id];
        hitInNVMBuffer(nvm_buf_hdr, EvictStrategy);
        // stratege
    }
    else 
    {
        nvm_buf_hdr = getStrategyNVMBuffer(nvm_buf_tag, EvictStrategy);
        // else not hit
        // getstratege
        nvmBufferTableInsert(&nvm_buf_tag, nvm_buf_hash, nvm_buf_hdr->nvm_buf_id);
        nvm_buf_hdr->nvm_buf_tag = nvm_buf_tag;
        *found = 0;
    }
    return nvm_buf_hdr;   
}

void *flushNVMBuffer(NVMBufferDesc *nvm_buf_hdr)
{
    char *nvm_buffer;
    int ret;

    int data_id = nvm_buf_hdr->nvm_buf_tag.data_ssd_id;
    int parity_id = nvm_buf_hdr->nvm_buf_tag.parity_ssd_id;
    off_t ssd_offset = nvm_buf_hdr->nvm_buf_tag.ssd_offset;

    int ssd_id = (data_id==-1 ? parity_id : data_id);
    if(data_id < 0)
        flush_parity++;
    else 
        flush_data++;
    /*
    // page
    ret = posix_memalign(&nvm_buffer, 512, sizeof(char)*PAGESIZE);
    if(ret < 0)
    {
        perror("[ERROR]:flushNVMBuffer()------posix_memalign");
        free(nvm_buffer);
        exit(0);
    }
//    ret = pread(nvm_fd, nvm_buffer, NVM_BUFFER_SIZE, nvm_buf_hdr->nvm_buf_id*NVM_BUFFER_SIZE);
    if(ret < 0)
    {
        perror("[ERROR]:");
        printf("flushNVMBuffer():---- read from NVM: nvm_fd=%d, errorcode=%d, offset=%lu\n", nvm_fd, ret, nvm_buf_hdr->nvm_buf_id*NVM_BUFFER_SIZE);
        exit(0);
    }
//    ret = writeOrReadPage(ssd_id, ssd_offset, nvm_buffer, 1);
    if(ret < 0)
    {
        perror("[ERROR]:");
        return nvm_buf_hdr;
        printf("flushNVMBuffer()------write to data_ssd_id=%d, errorcode=%d, offset=%lu\n",data_id, ret, nvm_buf_hdr->nvm_buf_tag.offset);
        return nvm_buf_hdr;
        exit(0);
    }
    ret = writeOrReadPage(parity_id, ssd_offset,nvm_buffer, 1);
    if(ret < 0)
    {
        perror("[ERROR]:");
        printf("flushNVMBuffer()------write to parity_ssd_id=%d, errorcode=%d, offset=%lu\n",parity_id, ret, nvm_buf_hdr->nvm_buf_tag.offset);
        exit(0);
    }
    free(nvm_buffer);*/
    flush_blocks += 1;
    return NULL;
}

void *flushNVMStripeBuffer(NVMStripeBufferDesc *nvm_stripe_hdr)
{
    long stripe_id = nvm_stripe_hdr->stripe_id;
    off_t page_id = (stripe_id +1)* (N + 1) ;   // raid page id
    int i = 0;
    int firstOrlast = 0;
    flush_stripe++;
    for(i = 0; i < N+1 ;++i)
    {
        NVMBufferTag nvm_buf_tag;
        nvm_buf_tag.offset = page_id + i;
        unsigned long hashcode = nvmBufferTableHashCode(&nvm_buf_tag);
        long nvm_buf_id = nvmBufferTableLookup(&nvm_buf_tag, hashcode);
        // nvm buffer exist
        if(nvm_buf_id >= 0)
        {
            NVMBufferDesc *nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buf_id];
            if(firstOrlast==0)
            {
                nvm_buffer_control->first_freenvm = nvm_buf_hdr->nvm_buf_id;
                nvm_buffer_control->last_freenvm = nvm_buf_hdr->nvm_buf_id;
                nvm_buf_hdr->next_freenvm = -1;
                firstOrlast++;
            }
            else if(firstOrlast > 0)
            {
                nvm_buffer_descriptors[nvm_buffer_control->last_freenvm].next_freenvm = nvm_buf_id;
                nvm_buf_hdr->next_freenvm = -1;
                nvm_buffer_control->last_freenvm = nvm_buf_id;
            }
            nvm_buffer_control->n_usednvm--;
            flushNVMBuffer(nvm_buf_hdr);
//            NVMBufferTag old_tag = nvm_buf_hdr->nvm_buf_tag;
  //          unsigned long old_hash = nvmBufferTableHashCode(&old_tag);
            nvmBufferTableDelete(&nvm_buf_tag, hashcode);
        }
    }
    return NULL;
}

void read_block(off_t offset, char* nvm_buffer)
{
    //void *nvm_buf_block;
    bool found = 0;
    int ret;

    static NVMBufferDesc *nvm_buf_hdr;
    static NVMBufferTag nvm_buf_tag;
    nvm_buf_tag.offset = offset;
/*
    nvm_buf_tag.data_ssd_id = data_ssd_id;
    nvm_buf_tag.ssd_offset = ssd_page_off;
    nvm_buf_tag.parity_ssd_id = parity_ssd_id;
    nvm_buf_tag.flag = 1;
  */
    if(DEBUG)
        printf("[INFO]:read() ------ offset=%lu\n",offset);
 //   nvm_buf_hdr = NVMBufferAlloc(nvm_buf_tag, &found);
    if(found==1)
    {
        // read 4KB
 //       ret = pread(nvm_fd, nvm_buffer, NVM_BUFFER_SIZE, nvm_buf_hdr->nvm_buf_id*NVM_BUFFER_SIZE);
        if(ret < 0)
        {
            printf("[ERROR]:read_block()---read from nvm cache: nvm_fd=%d, errorcode=%d, offset=%lu\n", nvm_fd, ret, offset);
            perror("[ERROR]");
            exit(0);
        }
    }
    else {
        // read 0
   //     ret = writeOrReadPage(data_ssd_id, ssd_page_off, nvm_buffer, 0);
//        ret = ssdread(ssd_fd, nvm_buffer, NVM_BUFFER_SIZE, offset);
        if(ret < 0)
        {
            printf("[ERROR]:read_block()---read from ssd: ssd_fd = %d, errorcode=%d, offset=%lu\n", data_ssd_id, ret, offset);
            perror("[ERROR]");
            exit(0);
        }
    }
}

void write_block(off_t offset, char *nvm_buffer, int flag)
{
  //  void *nvm_buf_block;
    bool found;
    int ret;

    static NVMBufferTag nvm_buf_tag;
    static NVMBufferDesc *nvm_buf_hdr;
    nvm_buf_tag.offset = offset;
    if(flag==1)  // data
        nvm_buf_tag.data_ssd_id = data_ssd_id;
    else  // parity
        nvm_buf_tag.data_ssd_id = -1;
    nvm_buf_tag.ssd_offset = ssd_page_off;
    nvm_buf_tag.parity_ssd_id = parity_ssd_id;
    nvm_buf_tag.stripe_id = global_stripe_id;
    nvm_buf_tag.flag = flag;
    if(DEBUG)
        printf("[INFO] write()----offset=%lu\n", offset);
    nvm_buf_hdr = NVMBufferAlloc(nvm_buf_tag, &found);
    write_blocks++;
    if(write_blocks%100000==0)
        printf("hit num:%lu  hit_parity:%lu write_blocks:%lu flush_blocks:%lu flush_stripes=%lu\n", hit_num, hit_parity, write_blocks, flush_blocks, flush_stripe);

//    ret = pwrite(nvm_fd, nvm_buffer, NVM_BUFFER_SIZE, nvm_buf_hdr->nvm_buf_id*NVM_BUFFER_SIZE);
    if(ret < 0)
    {
        printf("[ERROR]: write_block nvm_fd=%d, errorcode=%d, offset=%lu\n", nvm_fd, ret, offset);
        perror("[ERROR]");
        exit(0);
    }
}
    



