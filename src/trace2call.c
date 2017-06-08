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
    FILE *trace;
    if((trace = fopen(trace_file, "rt"))==NULL)
    {
        perror("[ERROR]:trace2call open trace_file");
        exit(0);
    }
    double time_begin, time_now;
    struct timeval tv_begin, tv_now;
    struct timezone tz_begin, tz_now;
    char action;
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
    while(!feof(trace))
    {
        ret = fscanf(trace, "%c %d %lu\n", &action, &i, &offset);
        if(ret < 0) 
            break;
        execute(offset);
        off_t data_global_offset = data_raid_offset;
        off_t parity_global_offset = parity_raid_offset;
        for(i = 0;i < PAGESIZE;++i)
            nvm_buffer[i] = '0';
        write_block(data_global_offset, nvm_buffer, 1);
//        write_block(parity_global_offset, nvm_buffer, 0);
    }
    gettimeofday(&tv_now, &tz_now);
    time_now = tv_now.tv_sec + tv_now.tv_usec / 1000000.0;
    printf("EvictStrategy=%d, total_run_time=%lfs, pm_time=%lfs, flush_time=%lfs\n",EvictStrategy, time_now - time_begin, pm_time, flush_time);
    printf("write_blocks=%lu, hit_num=%lu, hit_rate=%lf, flush_blocks=%lu\n",write_blocks, hit_num, hit_num*1.0/write_blocks, flush_blocks); 
    printf("hit_data=%lu, hit_data_rate=%lf, hit_parity=%lu, hit_parity_rate=%lf\n",hit_data, hit_data*1.0/write_blocks, hit_parity, hit_parity*1.0/write_blocks);
    printf("flush_blocks=%lu, flush_data=%lu, flush_parity=%lu\n",flush_blocks+flush_stripe, flush_data, flush_parity);
    printf("hit_stripe=%lu, flush_stripe=%lu\n", hit_stripe, flush_stripe);
    free(nvm_buffer);

    FILE *res;
    if((res = fopen("result", "a+"))==NULL)
    {
        perror("[ERROR]");
        exit(0);
    }
    fprintf(res, "EvictStrategy=%d, total_run_time=%lfs, pm_time=%lfs, flush_time=%lfs\n",EvictStrategy, time_now - time_begin, pm_time, flush_time);
    fprintf(res, "write_blocks=%lu, hit_num=%lu, hit_rate=%lf, flush_blocks=%lu\n",write_blocks, hit_num, hit_num*1.0/write_blocks, flush_blocks); 
    fprintf(res, "hit_data=%lu, hit_data_rate=%lf, hit_parity=%lu, hit_parity_rate=%lf\n",hit_data, hit_data*1.0/write_blocks, hit_parity, hit_parity*1.0/write_blocks);
    fprintf(res, "flush_blocks=%lu, flush_data=%lu, flush_parity=%lu\n",flush_blocks+flush_stripe, flush_data, flush_parity);
    fprintf(res, "hit_stripe=%lu, flush_stripe=%lu\n\n", hit_stripe, flush_stripe);
    fclose(res); 
    fclose(trace);
}

