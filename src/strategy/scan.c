/*************************************************************************
	> File Name: scan.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 18 Apr 2017 07:38:18 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "nvm_cache.h"
#include "scan.h"

static volatile void *deleteFromSCAN(long nvm_buf_id);

void initNVMBufferForSCAN()
{
    nvm_buffer_control_scan = (NVMBufferControlForSCAN*)malloc(sizeof(NVMBufferControlForSCAN));
    nvm_buffer_control_scan->start = -1;
    nvm_buffer_control_scan->scan_ptr = -1;

    nvm_buffer_descriptors_scan = (NVMBufferDescForSCAN*)malloc(sizeof(NVMBufferDescForSCAN)*NNVMBuffers);
    NVMBufferDescForSCAN *nvm_buf_hdr_scan = nvm_buffer_descriptors_scan;
    long i;
    for(i = 0;i < NNVMBuffers; ++i)
    {
        nvm_buf_hdr_scan->nvm_buf_id = i;
        nvm_buf_hdr_scan->last_scan = -1;
        nvm_buf_hdr_scan->next_scan = -1;
        nvm_buf_hdr_scan++;
    }        
}

void insertIntoSCAN(NVMBufferTag nvm_buf_tag, long nvm_buf_id)
{
    if(nvm_buffer_control_scan->start==-1)
    {
        nvm_buffer_control_scan->start = nvm_buf_id;
        nvm_buffer_control_scan->scan_ptr = nvm_buf_id;
    }
    else {
        long movePtr = nvm_buffer_control_scan->start;
        if(nvm_buffer_descriptors[movePtr].nvm_buf_tag.offset > nvm_buf_tag.offset)
        {
            nvm_buffer_descriptors_scan[movePtr].last_scan = nvm_buf_id;
            nvm_buffer_descriptors_scan[nvm_buf_id].next_scan = movePtr;
            nvm_buffer_control_scan->start = nvm_buf_id;
        }
        else {
            while(nvm_buffer_descriptors[movePtr].nvm_buf_tag.offset <= nvm_buf_tag.offset)
            {
                if(nvm_buffer_descriptors_scan[movePtr].next_scan != -1)
                {
                    long next = nvm_buffer_descriptors_scan[movePtr].next_scan;
                    if(nvm_buffer_descriptors[next].nvm_buf_tag.offset <= nvm_buf_tag.offset)
                    {
                        movePtr = next;
                    }
                    else break;
                }
                else break;
            }
            long temp = nvm_buffer_descriptors_scan[movePtr].next_scan;
            nvm_buffer_descriptors_scan[movePtr].next_scan = nvm_buf_id;
            nvm_buffer_descriptors_scan[nvm_buf_id].last_scan = movePtr;
            if(temp!=-1)
            {
                nvm_buffer_descriptors_scan[temp].last_scan = nvm_buf_id;
                nvm_buffer_descriptors_scan[nvm_buf_id].next_scan = temp;
            }
            else {
                nvm_buffer_descriptors_scan[nvm_buf_id].next_scan = temp;
            }
        }
    }
}

static volatile void *deleteFromSCAN(long nvm_buf_id)
{
    long last = nvm_buffer_descriptors_scan[nvm_buf_id].last_scan;
    long next = nvm_buffer_descriptors_scan[nvm_buf_id].next_scan;

    nvm_buffer_descriptors_scan[last].next_scan = nvm_buffer_descriptors_scan[nvm_buf_id].next_scan;
    nvm_buffer_descriptors_scan[next].last_scan = nvm_buffer_descriptors_scan[nvm_buf_id].last_scan;
    nvm_buffer_descriptors_scan[nvm_buf_id].last_scan = -1;
    nvm_buffer_descriptors_scan[nvm_buf_id].next_scan = -1;
}

NVMBufferDesc *getSCANBuffer(NVMBufferTag nvm_buf_tag)
{
    NVMBufferDesc *nvm_buf_hdr;
    NVMBufferDescForSCAN *nvm_buf_hdr_scan;

    if(nvm_buffer_control->first_freenvm < 0) {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control_scan->scan_ptr];
        if(nvm_buf_hdr->nvm_buf_id == nvm_buffer_control_scan->start)
        {
            // flush the start position
            nvm_buffer_control_scan->start = nvm_buffer_descriptors_scan[nvm_buffer_control_scan->scan_ptr].next_scan;
        }

        if(nvm_buffer_descriptors_scan[nvm_buffer_control_scan->scan_ptr].next_scan!=-1)
        {
            nvm_buffer_control_scan->scan_ptr = nvm_buffer_descriptors_scan[nvm_buffer_control_scan->scan_ptr].next_scan;
        }
        else {
            nvm_buffer_control_scan->scan_ptr = nvm_buffer_control_scan->start;
        }
        NVMBufferTag old_tag = nvm_buf_hdr->nvm_buf_tag;
        unsigned long old_hash = nvmBufferTableHashCode(&old_tag);
        flushNVMBuffer(nvm_buf_hdr);
        nvmBufferTableDelete(&old_tag, old_hash);
        deleteFromSCAN(nvm_buf_hdr->nvm_buf_id);
        nvm_buf_hdr->next_freenvm = nvm_buffer_control->first_freenvm;
        nvm_buffer_control->first_freenvm = nvm_buf_hdr->nvm_buf_id;
    }
    
    if(nvm_buffer_control->first_freenvm >= 0)
    {
        nvm_buf_hdr = &nvm_buffer_descriptors[nvm_buffer_control->first_freenvm];
        nvm_buf_hdr_scan = &nvm_buffer_descriptors_scan[nvm_buffer_control->first_freenvm];
        nvm_buffer_control->first_freenvm = nvm_buf_hdr->next_freenvm;
        nvm_buf_hdr->next_freenvm = -1;
        insertIntoSCAN(nvm_buf_tag, nvm_buf_hdr->nvm_buf_id);
        return nvm_buf_hdr;
    }
    return nvm_buf_hdr;
}

void *hitInSCANBuffer(NVMBufferDesc *nvm_buf_tag)
{
    return NULL;
}



