//sleep 函数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void catch_signal(int sign)
{
    switch(sign)
    {
    case SIGINT:
        printf("accept signal!\n");
        break;
    default:
        break;
    }
}

int main(int arg,char *args[])
{
    //注册信号
    if(signal(SIGINT,catch_signal)==SIG_ERR)
    {
        perror("signal error");
        return-1;
    }
    int num=0;
    num=sleep(100);
    //手动执行 ctrl+C  进程被唤醒
    if(num>0)
    {
        printf("sleep()函数被打算睡眠，醒过来了！\n");
    }
    //说明：sleep()函数是可中断睡眠

    printf("新的测试！\n");
    //再次设计sleep()函数，让其sleep需要的时间
    num=15;
    do{
        num=sleep(num);
        printf("被唤醒了，但是还要继续睡眠！剩余时间%d\n",num);
    }while(num);


    printf("game over!\n");
    return 0;
}