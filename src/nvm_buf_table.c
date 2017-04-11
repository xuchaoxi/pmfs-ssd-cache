/*************************************************************************
	> File Name: nvm-buf-table.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 05:31:28 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm_cache.h"
#include "raid_5.h"

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
        nvm_buf_hdr->hash_key.data_ssd_id = -1;
        nvm_buf_hdr->hash_key.parity_ssd_id = -1;
        nvm_buf_hdr->hash_key.flag = -1;
        nvm_buf_hdr->next = NULL;
    }
}

unsigned long nvmBufferTableHashCode(NVMBufferTag *tag)
{
    // offset = page_id
    // return the Nth table
    unsigned long nvm_buf_hash = tag->offset % NNVMBufferTables;
    return nvm_buf_hash;
}

size_t nvmBufferTableLookup(NVMBufferTag *nvm_buf_tag, unsigned long hash_code)
{
    if(DEBUG)
        printf("[INFO]:look up on nvm cache offset=%lu\n", nvm_buf_tag->offset);
    NVMBufferHashBucket *nowbucket = GetNVMBufferHashBucket(hash_code);
    while(nowbucket!=NULL)
    {
        if(isSamebuf(&nowbucket->hash_key, nvm_buf_tag))
            return nowbucket->nvm_buf_id;
        nowbucket = nowbucket->next;
    }
    return -1;
}

size_t nvmBufferTableInsert(NVMBufferTag *nvm_buf_tag, unsigned long hash_code, size_t nvm_buf_id)
{
    if(DEBUG)
        printf("[INFO]:Insert buf_tag=%lu\n", nvm_buf_tag->offset);
    NVMBufferHashBucket *nowbucket = GetNVMBufferHashBucket(hash_code);
    while(nowbucket->next!=NULL&&nowbucket!=NULL)
    {
        if(isSamebuf(&nowbucket->hash_key, nvm_buf_tag))
        {
            return nowbucket->nvm_buf_id;
        }
        nowbucket = nowbucket->next;
    }
    if(nowbucket!=NULL)
    {
        NVMBufferHashBucket *newbucket = (NVMBufferHashBucket*)malloc(sizeof(NVMBufferHashBucket));
        newbucket->hash_key = *nvm_buf_tag;
        newbucket->nvm_buf_id = nvm_buf_id;
        newbucket->next = NULL;
        nowbucket->next = newbucket;
    }
    else {
        nowbucket->nvm_buf_id = nvm_buf_id;
        nowbucket->hash_key = *nvm_buf_tag;
        nowbucket->next = NULL;
    }
    return -1;

}

size_t nvmBufferTableDelete(NVMBufferTag *nvm_buf_tag, unsigned long hash_code)
{
    if(DEBUG)
        printf("[INFO]:Delet buf_tag=%lu\n",nvm_buf_tag->offset);
    NVMBufferHashBucket *nowbucket = GetNVMBufferHashBucket(hash_code);
    size_t del_id;
    NVMBufferHashBucket *delitem;
    while(nowbucket->next!=NULL && nowbucket!=NULL)
    {
        if(isSamebuf(&nowbucket->next->hash_key, nvm_buf_tag))
        {
            del_id = nowbucket->next->nvm_buf_id;
            break;
        }
        nowbucket = nowbucket->next;
    }
    if(isSamebuf(&nowbucket->hash_key, nvm_buf_tag))
        del_id = nowbucket->nvm_buf_id;
    if(nowbucket->next!=NULL)
    {
        delitem = nowbucket->next;
        nowbucket->next = nowbucket->next->next;
        free(delitem);
        return del_id;
    }
    else {
        delitem = nowbucket->next;
        nowbucket->next = NULL;
        free(delitem);
        return del_id;
    }
    return -1;
}


static bool isSamebuf(NVMBufferTag *tag1, NVMBufferTag *tag2)
{
    return (tag1->offset==tag2->offset);
}

