/*************************************************************************
	> File Name: trace2call.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Thu 06 Apr 2017 06:57:12 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<unistd.h>
#include "nvm-cache.h"
#include "raid/raid-5.h"

void trace_to_iocall(char *trace_file)
{
    FILE *trace;
/*    if((trace = fopen(trace_file, "rt"))==NULL)
    {
        exit(0);
    }*/
    double time, time_begin, time_now;
    struct timeval tv_begin, tv_now;
    struct timezone tz_begin, tz_now;
    char action;
    off_t offset;
    size_t size;
    char *nvm_buffer;
    int i;
    gettimeofday(&tv_begin, &tz_begin);
    time_begin = tv_begin.tv_sec + tv_begin.tv_usec / 1000000.0;
    int ret = posix_memalign(&nvm_buffer, 512, sizeof(char)*PAGESIZE);
    if(ret < 0)
    {
        printf("[ERROR]:trace_to_iocall() ---- posix_memalign\n");
        perror("[ERROR]:");
        exit(0);
    }
    while(scanf("%c %d %lu", &action, &i, &offset)!=EOF)
    {
    //    gettimeofday(&tv_now, &tz_now);
     //   time_now = tv_now.tv_sec + tv_now.tv.usec / 1000000.0;
            execute(offset);
            int global_offset = raid_offset;
            for(i = 0;i < PAGESIZE;++i)
                nvm_buffer[i] = '0';
            if(action=='1')
                write_block(global_offset, nvm_buffer);
            else
                read_block(global_offset, nvm_buffer);
    }
    gettimeofday(&tv_now, &tz_now);
    time_now = tv_now.tv_sec + tv_now.tv_usec / 1000000.0;
    printf("total_run_time=%lfs, ",time_now - time_begin);
    printf("hit_num=%lu,", hit_num);
    
    free(nvm_buffer);
//    close(trace);
}
