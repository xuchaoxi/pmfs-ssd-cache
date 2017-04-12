/*************************************************************************
	> File Name: nvm_stripe_table.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 11 Apr 2017 08:39:16 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "lrustripe.h"

void initNVMStripeTable(size_t size)
{
    nvm_buf_stripe_table = (NVMBufferStripeBucket*)malloc(size*sizeof(NVMBufferStripeBucket));
    size_t i;
    NVMBufferStripeBucket *nvm_buf_hdr = nvm_buf_stripe_table;
    for(i = 0; i < size; ++i)
    {
        nvm_buf_hdr->lru_buf_id = -1;
        nvm_buf_hdr->stripe_id = -1;
        nvm_buf_hdr->next = NULL;
        nvm_buf_hdr++;
    }
}

unsigned long nvmBufferTableHashCode(unsigned long stripe_id);
{
    unsigned long hashcode = (stripe_id) % STRIPES;
    return hashcode;
}

long nvmStripeTableLookup(unsigned long stripe_id, unsigned long hashcode)
{
    NVMBufferStripeBucket *nowbucket = GETNVMBufferStripeBucket(hashcode);
    while(nowbucket!=NULL)
    {
        if(nowbucket->stripe_id = stripe_id)
            return nowbucket->lru_buf_id;
        nowbucket = nowbucket->next;
    }
    return -1;
}

long nvmStripeTableInsert(unsigned long stripe_id, unsigned long hashcode, long lru_buf_id)
{
    NVMBufferStripeBucket *nowbucket = GETNVMBufferStripeBucket(hashcode);
    while(nowbucket->next!=NULL && nowbucket!=NULL)
    {
        if(nowbucket->next->stripe_id == stripe_id)
            return nowbucket->lru_buf_id;
        nowbucket = nowbucket->next;
    }
    if(nowbucket!=NULL)
    {
        NVMBufferStripeBucket *newbucket = (NVMBufferStripeBucket*)malloc(sizeof(NVMBufferStripeBucket));
        newbucket->stripe_id = stripe_id;
        newbucket->lru_buf_id = lru_buf_id;
        newbucket->next = NULL;
        nowbucket->next = newbucket;
    }
    return -1;
}

long nvmBufferStripeDelete(unsigned long stripe_id, unsigned long hashcode)
{
    NVMBufferStripeBucket *nowbucket = GETNVMBufferStripeBucket(hashcode);
    long del_id;
    NVMBufferStripeBucket *delitem;
    while(nowbuck->next!=NULL && nowbucket!=NULL)
    {
        if(nowbucket->next->stripe_id==stripe_id)
        {
            del_id = stripe_id;
            break;
        }
        nowbucket = nowbucket->next;
    }
    if(nowbucket->next!=NULL)
    {
        delitem = nowbucket->next;
        nowbucket->next = nowbucket->next->next;
        free(delitem);
        return del_id;
    }
    return -1;
}




