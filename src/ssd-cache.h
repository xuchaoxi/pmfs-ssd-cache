/*************************************************************************
	> File Name: ssd-cache.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Mon 27 Mar 2017 03:59:44 PM CST
 ************************************************************************/

#ifndef _SSD-CACHE_H
#define _SSD-CACHE_H

typedef struct {
    off_t offset;
}SSDBufferTag;

typedef struct {
    SSDBufferTag ssd_buf_tag;
    long ssd_buf_id;
    unsigned ssd_buf_flag;
    long next_freessd;
}SSDBufferDesc;

typedef struct {
    SSDBufferTag hash_key;
    long ssd_buf_id;
    struct SSDBuferHashBucket *next_item;
}SSDBufferHashBucket;

#endif
