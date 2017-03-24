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
#define OFF_SET 50*1024*1024*1024  // 100GB


unsigned int write_magnification[] = {1,2,4,8,16,32,64,128,256,512,1024};  // 4KB, 8KB, 16KB. 32KB, 64KB, 128KB, 256KB, 512KB,1MB,2MB,4MB
const char tmp[] = {'0','1','2','3','4','5','6','7','8','9','10','11','12','\0'};
unsigned long page_size = 4096;  // 4KB
unsigned long write_size;  // write size per time


char* buffer;  // buffer
int fd;       // file descriptor

void init(int t)
{
    long i;
    write_size = page_size * write_magnification[t];
    // allocate 
  //  if(t >= 3)
    //    void* memalign(size_t boundary, size_t size)
//        buffer = (char*)memalign(512, write_size);
   // else
//    buffer = (char*)malloc(write_size);   
    buffer = (char*)valloc(write_size);
    if(!buffer) {
        printf("Fail to allocate write buffer\n");
        exit(1);
    }
    for(i = 0; i < write_size; i++)
    {
        buffer[i] = tmp[t];
    }
}

void random_write(unsigned long num, int t)
{
    unsigned long i;
    off_t off_num;
    struct timeval tv_begin, tv_end;
    double totle_time;
    double bandWidth;
    int returnCode;

    fd = open("/mnt/ssd/write", O_RDWR | O_CREAT | O_DIRECT, 0666);
    if(fd < 0)
    {
        perror("[ERROR]:Cann't open /mnt/ssd/write");
        exit(1);
    }

    gettimeofday(&tv_begin, NULL);
    printf("-------------------randdom write, write size = %dKB----------------------\n", write_magnification[t]*4);

    srand((unsigned)time(NULL));
    for(i = 0; i < num; i++)
    {
        off_num = rand() % ((unsigned long)OFF_SET / page_size); // %(1GB/PAGE_SIZE)  // 30GB

//	printf("off_num = %ld\n", off_num);
        returnCode = pwrite(fd, buffer, write_size, off_num*page_size);

        if(returnCode < 0)
        {
            printf("[ERROR]:write /mnt/ssd/write fail offset= %ld, PAGE_SIZE= %d\n", off_num*write_size, write_size);
            exit(1);
        }
    }
    gettimeofday(&tv_end, NULL);
    totle_time = (tv_end.tv_usec - tv_begin.tv_usec) / 1000000.0 + (tv_end.tv_sec - tv_begin.tv_sec);
    bandWidth = (num * write_size) / (1024 * 1024) / totle_time;
    printf("totle time = %lf s, bandwidth = %lf MB/s\n", totle_time, bandWidth);
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

    fd = open("/mnt/ssd/write", O_RDWR | O_CREAT | O_DIRECT , 0666);
    if(fd < 0)
    {
        perror("cann't open /mnt/ssd/write");
        exit(1);
    }
    gettimeofday(&tv_begin, NULL);
    printf("-------------------sequential write, write size = %dKB------------------\n", write_magnification[t]*4);
   
    for(i = 0; i < num; i++)
    {
        returnCode = write(fd, buffer, write_size);  // num * block
        if(returnCode < 0)
        {
            printf("ERROR:write /mnt/ssd/write fail, PAGE_SIZE= %ld\n", write_size);
            exit(1);
        }
    }

    gettimeofday(&tv_end, NULL);
    totle_time = (tv_end.tv_usec - tv_begin.tv_usec) / 1000000.0 + (tv_end.tv_sec - tv_begin.tv_sec);
    bandWidth = (num * write_size) / (1024 * 1024) / totle_time;
    printf("totle time = %lf s, bandwidth = %lf MB/s\n", totle_time, bandWidth);
    printf("-------------------------------end!-----------------------------------\n\n");
    close(fd);
 //   system("rm -f /mnt/ssd/iotest");
}

int main()
{
    int i;
    for(i = 0; i < 10; i++)
    {
        init(i);
        unsigned long num = (unsigned long)SSD_BUFFER_SIZE / write_size;
//        random_write(num, i);
        sequential_write(num, i);
    }
    return 0;
}




