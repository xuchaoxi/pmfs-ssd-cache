/*************************************************************************
	> File Name: nvm_stripe_table.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 11 Apr 2017 08:39:16 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm_cache.h"

void initNVMStripeTable(size_t size)
{
    nvm_stripe_table = (NVMStripeBufferBucket*)malloc(size*sizeof(NVMStripeBufferBucket));
    size_t i;
    NVMStripeBufferBucket *nvm_buf_hdr = nvm_stripe_table;
    for(i = 0; i < size; ++i)
    {
        nvm_buf_hdr->stripe_buf_id = -1;
        nvm_buf_hdr->stripe_id = -1;
        nvm_buf_hdr->next = NULL;
        nvm_buf_hdr++;
    }
}

unsigned long nvmStripeTableHashCode(unsigned long stripe_id)
{
    unsigned long hashcode = (stripe_id) % STRIPES;
    return hashcode;
}

long nvmStripeTableLookup(unsigned long stripe_id, unsigned long hashcode)
{
    NVMStripeBufferBucket *nowbucket = GetNVMStripeBufferBucket(hashcode);
    while(nowbucket->next!=NULL && nowbucket!=NULL)
    {
        if(nowbucket->next->stripe_id == stripe_id)
            return nowbucket->next->stripe_buf_id;
        nowbucket = nowbucket->next;
    }
    return -1;
}

long nvmStripeTableInsert(unsigned long stripe_id, unsigned long hashcode, long stripe_buf_id)
{
    NVMStripeBufferBucket *nowbucket = GetNVMStripeBufferBucket(hashcode);
    while(nowbucket->next!=NULL && nowbucket!=NULL)
    {
        if(nowbucket->next->stripe_id == stripe_id)
            return nowbucket->next->stripe_buf_id;
        nowbucket = nowbucket->next;
    }
    if(nowbucket!=NULL)
    {
        NVMStripeBufferBucket *newbucket = (NVMStripeBufferBucket*)malloc(sizeof(NVMStripeBufferBucket));
        newbucket->stripe_id = stripe_id;
        newbucket->stripe_buf_id = stripe_buf_id;
        newbucket->next = NULL;
        nowbucket->next = newbucket;
    }
    return -1;
}

long nvmStripeTableDelete(unsigned long stripe_id, unsigned long hashcode)
{
    NVMStripeBufferBucket *nowbucket = GetNVMStripeBufferBucket(hashcode);
    long del_id;
    NVMStripeBufferBucket *delitem;
    while(nowbucket->next!=NULL && nowbucket!=NULL)
    {
        if(nowbucket->next->stripe_id==stripe_id)
        {
            del_id = nowbucket->next->stripe_buf_id;
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

