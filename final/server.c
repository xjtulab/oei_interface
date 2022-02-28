
#include "dpde_agent.h"

int main(int argc, char const *argv[])
{
    /* code */
    DDS_Long i = 0;
    DDS_Long domain_id = 0;
    char *peer = "127.0.0.1";
    char *udp_intf = "lo";
    DDS_Long sleep_time = 100000;
    DDS_Long count = 0;

    const char *pub_topic = "t1";
    const char *sub_topic = "t2";


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
        }else if(!strcmp(argv[i], "-pub_topic")){
            ++i;
            if (i == argc){
                printf("-pub_topic <pub_topic_name>\n");
                return -1;
            }
            pub_topic = argv[i];
        }else if(!strcmp(argv[i], "-sub_topic")){
            ++i;
            if (i == argc){
                printf("-sub_topic <sub_topic_name>\n");
                return -1;
            }
            sub_topic = argv[i];
        }
        else
        {
            printf("unknown option: %s\n", argv[i]);
            return -1;
        }
    }

    InitAll(pub_topic, sub_topic, domain_id, udp_intf, peer, sleep_time, count);

    printf("Wait for message: \n");
    char msg[64];
    for(i=0; i <9999; i++){


        char *recv_msg = subscribe_msg();
        printf("Get message: %s\n", recv_msg);
        
        strcpy(msg, "[response]:");
        strcat(msg, recv_msg);
        //remove last '\n'

        printf("Send response: %s\n", msg);
        publish_msg(msg);

    }

    return 0;
}
