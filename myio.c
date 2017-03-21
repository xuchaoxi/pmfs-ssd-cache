/*************************************************************************
	> File Name: myio.c
	> Author: 
	> Mail: 
	> Created Time: 2017年03月21日  6:03:52
 ************************************************************************/

// O_DIRECT
#define _GNU_SOURCE 1

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/file.h>
#include<sys/types.h>
#include<sys/stat.h>

#define PAGE_SIZE 4096     // 4KB
#define SSD_BUFFER_SIZE 20*1024*1024*1024   // 20GB

unsigned int write_magnification[] = {1,2,4,8,16,32,64,128};  // 4KB, 8KB, 16KB. 32KB, 64KB, 128KB, 256KB, 512KB


char* buffer;
int fd;

void init()
{
    int i;
    // allocate 
    // void* memalign(size_t boundary, size_t size)
    buffer = (char*)memalign(512, PAGE_SIZE);
    if(!buffer) {
        printf("Fail to allocate write buffer\n");
        exit(1);
    }
    for(i = 0;i < PAGE_SIZE;i++)
    {
        buffer[i] = '.';
    }
}

void random_write(unsigned long num, int size)
{
    unsigned long i, off_num;
    struct timeval tv_begin, tv_end;
    double totle_time;
    double bandWidth;
    int returnCode;

    gettimeofday(&tv_begin, NULL);
    printf("-------------------begin to write, write size = %dKB-------------------\n",size*4);
    for(i = 0; i < num; i++)
    {
        off_num = rand() % ((unsigned long)SSD_BUFFER_SIZE / (unsigned long)PAGE_SIZE / 20); // %(1GB/PAGE_SIZE)
        returnCode = pwrite(fd, buffer, PAGE_SIZE, off_num*PAGE_SIZE);
        if(returnCode < 0)
        {
            printf("ERROR:write /mnt/ssd/test.data fail offset= %ld, PAGE_SIZE= %d", off_num*PAGE_SIZE, PAGE_SIZE);
            exit(1);
        }
    }
    gettimeofday(&tv_end, NULL);
    totle_time = (tv_end.tv_usec - tv_begin.tv_usec) / 1000000.0 + (tv_end.tv_sec - tv_begin.tv_sec);
    bandWidth = (num * PAGE_SIZE) / (1024 * 1024) / totle_time;
    printf("totle time = %lf s, bandwidth = %lf MB\n", totle_time, bandWidth);
    printf("-------------------------------end!-----------------------------------\n\n");
}

int main()
{
    int i;
    fd = open("/mnt/ssd/test.data", O_RDWR | O_CREAT | O_DIRECT, 0666);
    if(fd < 0)
    {
        perror("cann't open /mnt/ssd/test.data");
        exit(1);
    }
    init();
    for(int i = 0; i < 8; i++)
    {
        random_write((unsigned long)SSD_BUFFER_SIZE / (unsigned long)(PAGE_SIZE*write_magnification[i]));
    }
    close(fd);
    return 0;
}




