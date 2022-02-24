#include "dpde_publisher.h"
#include "dpde_subscriber.h"



int main(int argc, char const *argv[])
{
    /* code */
    DDS_Long i = 0;
    DDS_Long domain_id = 0;
    char *peer = "127.0.0.1";
    char *udp_intf = "lo";
    DDS_Long sleep_time = 1000;
    DDS_Long count = 0;
    char msg[128];


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

    init_publisher("client", "server", domain_id,  udp_intf, peer, 
            sleep_time, count);
    init_subscriber("client2", "server2", domain_id+1,  udp_intf, peer, 
            sleep_time, count);

    printf("Please input your message: \n");
    for(i=0; i <9999; i++){
        fgets(msg, 128, stdin);
        //remove last '\n'
        msg[strlen(msg)-1] = '\0';
        //send msg and receive response
        publish_msg(msg);
        printf("Send %s done!\n",msg);

        OSAPI_Thread_sleep(sleep_time);
    }

    
    return 0;
}