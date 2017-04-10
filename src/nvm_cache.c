/*************************************************************************
	> File Name: nvm-cache.c
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
#include "strategy/lru.h"

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
        nvm_buf_hdr->nvm_buf_flag = 0;
        nvm_buf_hdr->next_freenvm = i+1;
        nvm_buf_hdr++;
    }
    nvm_buf_hdr[NNVMBuffers-1].next_freenvm = -1;

    hit_num = 0;
    flush_nvm_blocks = 0;
}

static void *initStrategyNVMBuffer(NVMEvictionStrategy strategy)
{
    if(strategy==LRU)
        initNVMBufferForLRU();
    return NULL;
}

static NVMBufferDesc *getStrategyNVMBuffer(NVMBufferTag nvm_buf_tag, NVMEvictionStrategy strategy)
{
    if(strategy==LRU)
        return getLRUBuffer();
    return NULL;
}

static void *hitInNVMBuffer(NVMBufferDesc *nvm_buf_hdr, NVMEvictionStrategy strategy)
{
    if(strategy==LRU)
        hitInLRUBuffer(nvm_buf_hdr);
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
        *found = 1;
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buf_id];
        hitInNVMBuffer(nvm_buf_hdr, EvictStrategy);
        // stratege
        return nvm_buf_hdr;
    }

    nvm_buf_hdr = getStrategyNVMBuffer(nvm_buf_tag, EvictStrategy);
    // else not hit
    // getstratege
    nvmBufferTableInsert(&nvm_buf_tag, nvm_buf_hash, nvm_buf_hdr->nvm_buf_id);
    nvm_buf_hdr->nvm_buf_flag &= ~(NVM_BUF_VALID | NVM_BUF_DIRTY);
    nvm_buf_hdr->nvm_buf_tag = nvm_buf_tag;
    *found = 0;
    return nvm_buf_hdr;   
}

void *flushNVMBuffer(NVMBufferDesc *nvm_buf_hdr)
{
    char *nvm_buffer;
    int ret;

    int data_id = nvm_buf_hdr->nvm_buf_tag.data_ssd_id;
    int parity_id = nvm_buf_hdr->nvm_buf_tag.parity_ssd_id;
    off_t ssd_offset = nvm_buf_hdr->nvm_buf_tag.ssd_offset;
    
    // page
    ret = posix_memalign(&nvm_buffer, 512, sizeof(char)*PAGESIZE);
    if(ret < 0)
    {
        perror("[ERROR]:flushNVMBuffer()------posix_memalign");
        free(nvm_buffer);
        exit(0);
    }
    ret = pread(nvm_fd, nvm_buffer, NVM_BUFFER_SIZE, nvm_buf_hdr->nvm_buf_id*NVM_BUFFER_SIZE);
    if(ret < 0)
    {
        perror("[ERROR]:");
        printf("flushNVMBuffer():---- read from NVM: nvm_fd=%d, errorcode=%d, offset=%lu\n", nvm_fd, ret, nvm_buf_hdr->nvm_buf_id*NVM_BUFFER_SIZE);
        exit(0);
    }
    ret = writeOrReadPage(data_id, ssd_offset, 1);
    if(ret < 0)
    {
        perror("[ERROR]:");
        printf("flushNVMBuffer()------write to data_ssd_id=%d, errorcode=%d, offset=%lu\n",data_id, ret, nvm_buf_hdr->nvm_buf_tag.offset);
        exit(0);
    }
    ret = writeOrReadPage(parity_id, ssd_offset, 1);
    if(ret < 0)
    {
        perror("[ERROR]:");
        printf("flushNVMBuffer()------write to parity_ssd_id=%d, errorcode=%d, offset=%lu\n",parity_id, ret, nvm_buf_hdr->nvm_buf_tag.offset);
        exit(0);
    }
    free(nvm_buffer);
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
    if(DEBUG)
        printf("[INFO]:read() ------ offset=%lu\n",offset);
    nvm_buf_hdr = NVMBufferAlloc(nvm_buf_tag, &found);
    if(found==1)
    {
        // read 4KB
        ret = pread(nvm_fd, nvm_buffer, NVM_BUFFER_SIZE, nvm_buf_hdr->nvm_buf_id*NVM_BUFFER_SIZE);
        if(ret < 0)
        {
            printf("[ERROR]:read_block()---read from nvm cache: nvm_fd=%d, errorcode=%d, offset=%lu\n", nvm_fd, ret, offset);
            perror("[ERROR]");
            exit(0);
        }
    }
    else {
        // read 0
        ret = writeOrReadPage(data_ssd_id, ssd_page_off, 0);
//        ret = ssdread(ssd_fd, nvm_buffer, NVM_BUFFER_SIZE, offset);
        if(ret < 0)
        {
            printf("[ERROR]:read_block()---read from ssd: ssd_fd = %d, errorcode=%d, offset=%lu\n", data_ssd_id, ret, offset);
            perror("[ERROR]");
            exit(0);
        }
        flush_nvm_blocks++;
        nvm_buf_hdr->nvm_buf_flag &= ~NVM_BUF_VALID;
        nvm_buf_hdr->nvm_buf_flag |= NVM_BUF_VALID;
    }
}

void write_block(off_t offset, char *nvm_buffer)
{
  //  void *nvm_buf_block;
    bool found;
    int ret;

    static NVMBufferTag nvm_buf_tag;
    static NVMBufferDesc *nvm_buf_hdr;
    nvm_buf_tag.offset = offset;
    nvm_buf_tag.data_ssd_id = data_ssd_id;
    nvm_buf_tag.ssd_offset = ssd_page_off;
    nvm_buf_tag.parity_ssd_id = parity_ssd_id;
    if(DEBUG)
        printf("[INFO] write()----offset=%lu\n", offset);
    nvm_buf_hdr = NVMBufferAlloc(nvm_buf_tag, &found);
    flush_nvm_blocks++;
    if(flush_nvm_blocks%10000==0)
        printf("hit num:%lu  flush_nvm_blocks:%lu flush_fifo_blocks:\n", hit_num, flush_nvm_blocks);
    ret = pwrite(nvm_fd, nvm_buffer, NVM_BUFFER_SIZE, nvm_buf_hdr->nvm_buf_id*NVM_BUFFER_SIZE);
    if(ret < 0)
    {
        printf("[ERROR]: write_block nvm_fd=%d, errorcode=%d, offset=%lu\n", nvm_fd, ret, offset);
        perror("[ERROR]");
        exit(0);
    }
    nvm_buf_hdr->nvm_buf_flag |= NVM_BUF_VALID | NVM_BUF_DIRTY;
}
    



