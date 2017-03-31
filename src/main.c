/*************************************************************************
	> File Name: main.c
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Fri 31 Mar 2017 11:15:59 AM CST
 ************************************************************************/

#include<stdio.h>
#include"raid-5.h"

int main()
{
    int con, rw;
    long id;
//    rotate_width = 10;
    while(scanf("%d %d %ld", &con, &rw, &id)!=EOF)
    {
        execute(id);
    }
    return 0;
}
