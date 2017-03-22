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

#define SSD_BUFFER_SIZE 20*1024*1024*1024   // 20GB
#define OFF_SET 30*1024*1024*1024  // 30GB


unsigned int write_magnification[] = {1,2,4,8,16,32,64,128,256,512,1024};  // 4KB, 8KB, 16KB. 32KB, 64KB, 128KB, 256KB, 512KB,1MB,2MB,4MB
unsigned long page_size = 4096;  // 4KB
unsigned long size;  // write size per time


char* buffer;  // buffer
int fd;       // file descriptor

void init(int t)
{
    long i;
    size = page_size * t;
    // allocate 
    // void* memalign(size_t boundary, size_t size)
//    buffer = (char*)memalign(512, size);
    buffer = (char*)valloc(size);
    if(!buffer) {
        printf("Fail to allocate write buffer\n");
        exit(1);
    }
    for(i = 0; i < size; i++)
    {
        buffer[i] = '.';
    }
}

void random_write(unsigned long num, int t)
{
    unsigned long i, off_num;
    struct timeval tv_begin, tv_end;
    double totle_time;
    double bandWidth;
    int returnCode;

    fd = open("/mnt/ssd/iotest", O_RDWR | O_CREAT | O_DIRECT, 0666);
    if(fd < 0)
    {
        perror("cann't open /mnt/ssd/iotest");
        exit(1);
    }

    gettimeofday(&tv_begin, NULL);
    printf("-------------------randdom write, write size = %dKB----------------------\n", t*4);

    srand((unsigned)time(NULL));
    for(i = 0; i < num; i++)
    {
        off_num = rand() % ((unsigned long)OFF_SET / (unsigned long)size ); // %(1GB/PAGE_SIZE)  // 30GB

        returnCode = pwrite(fd, buffer, size, off_num*size);

        if(returnCode < 0)
        {
            printf("ERROR:write /mnt/ssd/iotest fail offset= %ld, PAGE_SIZE= %d", off_num*size, size);
            exit(1);
        }
    }
    gettimeofday(&tv_end, NULL);
    totle_time = (tv_end.tv_usec - tv_begin.tv_usec) / 1000000.0 + (tv_end.tv_sec - tv_begin.tv_sec);
    bandWidth = (num * size) / (1024 * 1024) / totle_time;
    printf("totle time = %lf s, bandwidth = %lf MB\n", totle_time, bandWidth);
    printf("-------------------------------end!-----------------------------------\n\n");
    close(fd);
 //   system("rm -f /mnt/ssd/iotest");
}

void sequential_write(unsigned long num, int t)
{
    unsigned long i;
    struct timeval tv_begin, tv_end;
    double totle_time;
    double bandWidth;
    int returnCode;

    fd = open("/mnt/ssd/iotest", O_RDWR | O_CREAT | O_DIRECT, 0666);
    if(fd < 0)
    {
        perror("cann't open /mnt/ssd/iotest");
        exit(1);
    }
    gettimeofday(&tv_begin, NULL);
    printf("-------------------sequential write, write size = %dKB------------------\n", t*4);
   
    for(i = 0; i < num; i++)
    {
        returnCode = write(fd, buffer, size);  // num * block
        if(returnCode < 0)
        {
            printf("ERROR:write /mnt/ssd/iotest fail, PAGE_SIZE= %ld\n", size);
            exit(1);
        }
    }

    gettimeofday(&tv_end, NULL);
    totle_time = (tv_end.tv_usec - tv_begin.tv_usec) / 1000000.0 + (tv_end.tv_sec - tv_begin.tv_sec);
    bandWidth = (num * size) / (1024 * 1024) / totle_time;
    printf("totle time = %lf s, bandwidth = %lf MB\n", totle_time, bandWidth);
    printf("-------------------------------end!-----------------------------------\n\n");
    close(fd);
 //   system("rm -f /mnt/ssd/iotest");
}

int main()
{
    int i;
    for(i = 0; i < 3; i++)
    {
        printf("begin\n");
        init(write_magnification[i]);
        printf("end\n");
        random_write((unsigned long)SSD_BUFFER_SIZE / (unsigned long)(page_size*write_magnification[i]), write_magnification[i]);
    sequential_write((unsigned long)SSD_BUFFER_SIZE / (unsigned long)(page_size*write_magnification[i]),write_magnification[i]);
    }
    return 0;
}




