/*************************************************************************
	> File Name: writezone.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Wed 22 Mar 2017 04:14:10 PM CST
 ************************************************************************/

// O_DIRECT
#define _GNU_SOURCE 1

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/file.h>
#include<sys/stat.h>

#define PAGE_SIZE 4096  // 4KB
#define ZONE_SIZE 3145728  // 1MB=1024*1024=1048576 2MB=2097152 3MB=3145728 
#define SSD_BUFFER_SIZE 50*1024*1024*1024 // 20GB
#define WRITE_BUFFER_SIZE 10*1024*1024*1024 // for how many to write

char* buffer;

void init()
{
    int i ;
    buffer = (char*)valloc(PAGE_SIZE);  // buffer = 4KB 
    if(!buffer) 
    {
        perror("[ERROR]:Fail to allocate write buffer");
        exit(0);
    }
    for(i = 0;i < PAGE_SIZE;i++)
    { 
        buffer[i] = '.';
    }
} 

void read_before_write(int N, unsigned long num)
{
    char* zone_buffer;
    ssize_t returnCode;
    off_t offset_fd;
    struct timeval tv_begin, tv_end;
    double bandWidth, total_time;
    unsigned long m;
    int n;
    int fd = open("/mnt/ssd/write", O_CREAT | O_RDWR | O_DIRECT, 0666);
    if(fd < 0)
    {
        perror("[ERROR]:Fail to open /mnt/ssd/write");
        exit(0);
    }
    zone_buffer = (char*)valloc(ZONE_SIZE);  // allocate ZONE_SIZE
    if(!zone_buffer)
    {
        perror("[ERROR]:Fail to allocate zone buffer");
        exit(0);
    }    

    srand(time(NULL));
    printf("-------------------------begin to read-before-write N=%d--------------------\n", N);
    gettimeofday(&tv_begin);
    for(m = 0;m < num;m++)
    {
        offset_fd = rand() % ((unsigned long)SSD_BUFFER_SIZE / PAGE_SIZE);   
        returnCode = pread(fd, zone_buffer, ZONE_SIZE, offset_fd*PAGE_SIZE);
        if(returnCode < 0)
        {
            perror("[ERROR]:Fail to read buffer");
            exit(0);
        }
       for(n = 0;n < N*4096;n++)
        {
            zone_buffer[n]++;
        }

        returnCode = pwrite(fd, zone_buffer, ZONE_SIZE, offset_fd*PAGE_SIZE);
        if(returnCode < 0)
        {
            perror("[ERROR]:Fail to write buffer");
            exit(0);
        }
    }
    gettimeofday(&tv_end);
    total_time = (tv_end.tv_usec - tv_begin.tv_usec)/1000000.0 + (tv_end.tv_sec - tv_begin.tv_sec);
    bandWidth = (unsigned long) WRITE_BUFFER_SIZE / (1024*1024) / total_time;
    printf("total time = %lf s, band width = %lf MB/s\n",total_time, bandWidth);
    printf("----------------------------------end!--------------------------------------\n\n");
    close(fd);

}

void direct_write(int N, unsigned long num)
{
    int fd; 
    struct timeval tv_begin ,tv_end;
    double total_time;
    double bandWidth;
    int returnCode;
    off_t offset_zone, offset_fd, cur_offset_fd; // offset in zone and offset in fd 
    int n;
    unsigned long m;

    fd = open("/mnt/ssd/write", O_CREAT | O_RDWR | O_DIRECT, 0666);
    if(fd < 0)
    {
        perror("[ERROR]:Cannot open file /mnt/ssd/write");
        exit(0);
    }
    srand(time(NULL));
    printf("-----------------------begin direct write, N=%d-----------------------------\n", N);
    gettimeofday(&tv_begin);

    for(m = 0; m < num;m++)  // total write num*N*4KB
    {
        offset_fd = rand() % ((unsigned long)SSD_BUFFER_SIZE / PAGE_SIZE); // fd randdom offset in 20GB 
        cur_offset_fd = lseek(fd, offset_fd*PAGE_SIZE , SEEK_SET);  // move fd from SEEK_SET to offset
        if(cur_offset_fd==-1)
        {
            perror("[ERROR]:lseek");
            exit(1);
        }
 //       srand(time(NULL));
        for(n = 0; n < N; n++)  //  N * PAGE_SIZE
        {   
 //           offset_zone =  rand() % ((unsigned long)ZONE_SIZE/PAGE_SIZE); // write offset=1MB  ?/PAGE_SIZE  
  //          returnCode = pwrite(fd, buffer, PAGE_SIZE, offset_zone*PAGE_SIZE);  // randdom write PAGE_SIZE(4KB) in the zone
            returnCode = write(fd, buffer, PAGE_SIZE);
            if(returnCode < 0)
            {
                perror("[ERROR]: Randdom write zone fail");
                exit(0);
            }
        }
    }
    gettimeofday(&tv_end);
    total_time = (tv_end.tv_usec - tv_begin.tv_usec)/1000000.0 + tv_end.tv_sec - tv_begin.tv_sec;
    bandWidth = (unsigned long)WRITE_BUFFER_SIZE / (1024*1024) / total_time;
    printf("total_time = %lf s, bandWidth = %lf MB/s\n", total_time, bandWidth);
    printf("------------------------------end!----------------------------------------\n\n");
    close(fd);
}

int main(int argc, char* argv[])
{
    int N = 64;  // N*4KB in a zone
 //   unsigned long num = (unsigned long) WRITE_BUFFER_SIZE / (N*4*1024);  // how many times to write
    init();
    unsigned long num2 = (unsigned long) WRITE_BUFFER_SIZE / ZONE_SIZE;
    for(N = 1;N <= 2048; N *= 2)
    {
 //       unsigned long num = (unsigned long) WRITE_BUFFER_SIZE / (N*4*1024);
   //     direct_write(N, num);
        read_before_write(N, num2);
    }
    return 0;
}
