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
#include "nvm_cache.h"
#include "raid/raid_5.h"

void trace_to_iocall(char *trace_file)
{
  //  FILE *trace;
/*    if((trace = fopen(trace_file, "rt"))==NULL)
    {
        exit(0);
    }*/
    double time_begin, time_now;
    struct timeval tv_begin, tv_now;
    struct timezone tz_begin, tz_now;
    int action;
    off_t offset;
    //size_t size;
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
    while(scanf("%d %d %lu", &action, &i, &offset)!=EOF)
    {
//        gettimeofday(&tv_now, &tz_now);
 //       time_now = tv_now.tv_sec + tv_now.tv.usec / 1000000.0;
            execute(offset);
            off_t data_global_offset = data_raid_offset;
//            off_t parity_global_offset = parity_raid_offset;
            for(i = 0;i < PAGESIZE;++i)
                nvm_buffer[i] = '0';
  //          if(action=='1')
            {
                write_block(data_global_offset, nvm_buffer);
            //    write_block(parity_global_offset, nvm_buffer);
            }
    //        else
            {
      //          printf("%c\n",action);
//                read_block(data_global_offset, nvm_buffer);
            }
    }
    gettimeofday(&tv_now, &tz_now);
    time_now = tv_now.tv_sec + tv_now.tv_usec / 1000000.0;
    printf("total_run_time=%lfs, ",time_now - time_begin);
    printf("hit_num=%lu, write_blocks=%lu, flush_blocks=%lu\n", hit_num, write_blocks, flush_blocks);
    printf("hit_rate = %lf\n", hit_num*1.0 / write_blocks);
    
    free(nvm_buffer);
//    close(trace);
}

