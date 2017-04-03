/*************************************************************************
	> File Name: nvm-buf-table.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 05:31:28 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm-cache.h"
#include "raid-5.h"

static bool isSamebuf(NVMBufferTag *, NVMBufferTag *);

void initNVMBufferTable(size_t size)
{
    nvm_buffer_hashtable = (NVMBufferHashBucket*)malloc(size*sizeof(NVMBufferHashBucket));
    size_t i = 0;
    NVMBufferHashBucket *nvm_buf_hdr = nvm_buffer_hashtable;
    for(;i < size;++i)
    {
        nvm_buf_hdr->nvm_buf_id = -1;
        nvm_buf_hdr->hash_key.offset = -1;
        nvm_buf_hdr->next = NULL;
    }
}

unsigned long nvmBufferTableHashCode(NVMBufferTag *tag)
{
    // offset = page_id
    // return the Nth table
    unsigned long nvm_buf_hash = tag->offset % NNVMBufTables;
    return nvm_buf_hash;
}

size_t nvmBufferTableLookup(NVMBufferTag *tag, unsigned long hash_code)
{
    if(DEBUG)
        printf("[INFO]:look up on nvm cache offset=%lu\n", tag->offset);
    NVMBufferHashBucket *nowbucket = GetNVMBufferHashBucket(hash_code);
    while(nowbucket!=NULL)
    {
        if(isSamebuf(&nowbucket->hash_key, tag))
            return nowbucket->nvm_buf_id;
        nowbucket = nowbucket->next;    
    }
    return -1;
}

long nvmBufferTableInsert(off_t nvm_buf_tag, unsigned long hash_code, size_t nvm_buf_id)
{
}

long nvmBufferTableDelete(off_t nvm_buf_tag, unsigned long hash_code)
{
}

static bool isSamebuf(NVMBufferTag *tag1, NVMBufferTag *tag2)
{
    return (tag1->offset==tag2->offset);
}

