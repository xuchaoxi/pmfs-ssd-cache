/*************************************************************************
	> File Name: nvm-buf-table.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 05:31:28 PM CST
 ************************************************************************/

#include<stdio.h>
#include "nvm-cache.h"
#include "raid-5.h"

static bool isSamebuf(NVMBufferTag tag);

void initNVMBufferTable(size_t size)
{
}

unsigned long nvmBufferTableHashCode(off_t offset)
{
    // offset = page_id
    // return the Nth table
    unsigned long nvm_buf_hash = offset % NNVMBufferTables;
    return nvm_buf_hash;
}

size_t nvmBufferTableLookup(off_t offset, unsigned long hash_code)
{
    if(DEBUG)
        printf("[INFO]:look up on nvm cache offset=%lu\n", offset);
    NVMBufferHashBucket *nowbucket = GetNVMBufferHashBucket(hash_code);
    while(nowbucket!=NULL)
    {
        if(isSamebuf(nowbucket->hash_key))
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

static bool isSamebuf(NVMBufferTag tag)
{
    return (tag->ssd_id==data_ssd_id && tag->offset==ssd_page_off);
}

