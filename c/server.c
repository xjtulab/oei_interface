#include "dpde_subscriber.h"
#include "dpde_publisher.h"



char buffer[64];


int main(int argc, char const *argv[])
{
    /* code */
    DDS_Long i = 0;
    DDS_Long domain_id = 0;
    char *peer = "127.0.0.1";
    char *udp_intf = "lo";
    DDS_Long sleep_time = 1000;
    DDS_Long count = 0;
    pthread_t publish_tid;


    for (i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-domain"))
        {
            ++i;
            if (i == argc)
            {
                printf("-domain <domain_id>\n");
                return -1;
            }
            domain_id = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-udp_intf"))
        {
            ++i;
            if (i == argc)
            {
                printf("-udp_intf <interface>\n");
                return -1;
            }
            udp_intf = argv[i];
        }
        else if (!strcmp(argv[i], "-peer"))
        {
            ++i;
            if (i == argc)
            {
                printf("-peer <address>\n");
                return -1;
            }
            peer = argv[i];
        }
        else if (!strcmp(argv[i], "-sleep"))
        {
            ++i;
            if (i == argc)
            {
                printf("-sleep_time <sleep_time>\n");
                return -1;
            }
            sleep_time = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-count"))
        {
            ++i;
            if (i == argc)
            {
                printf("-count <count>\n");
                return -1;
            }
            count = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-h"))
        {
            Application_help(argv[0]);
            return 0;
        }
        else
        {
            printf("unknown option: %s\n", argv[i]);
            return -1;
        }
    }


    int piper[2]; //管道
    if (pipe(piper) < 0) //创建管道
    {
        printf("pipe error\n");
        return 0;
    }

     pid_t pid;
    pid = fork(); //创建进程 获取id

    if (pid < 0)
            printf("error in fork!");
    else if (pid == 0) //子进程
    {
        printf("This is child process for publish\n");
        close(piper[1]);  //关闭写端  准备读

        init_publisher("client", "server", domain_id,  udp_intf, peer, 
            sleep_time, count);

        for(;;){
            if(read(piper[0], buffer, 64) > 0){
                publish_msg(buffer);
            }else
                break;
            
            if(strcmp(buffer, "q") == 0)
                break;
        }

    }
    else //父进程
    {
            printf("This is parent process for subscribe.\n");            //打印进程id
            close(piper[0]); //关闭读端口，开始写内容

            init_subscriber("client", "server", domain_id,  udp_intf, peer, 
            sleep_time, count);

            
            for(;;){
                char *msg = subscribe_msg(1000);
                int break_flag = 0;
                printf("Main: Recv msg: %s\n",msg);

                if(strcmp(msg, "q\n") == 0 || strcmp(msg, "q") ==0 ){
                    strcpy(buffer, "q");
                    break_flag = 1;

                }else{
                    strcpy(buffer, "This is response.\n");
                }

                write(piper[1], buffer, 64);

                if(break_flag)
                    break;
            }
            

            if (-1 != write(piper[1], "hello(from parent)", strlen("hello(from parent)"))) //写入信息
            {
                    printf("send data to child\n");
            }
    }

    
    return 0;
}


