#include "dpde_subscriber.h"
#include "dpde_publisher.h"


typedef struct OEIParam{
    DDS_Long domain_id;
    char *peer;
    char *udp_intf;
    DDS_Long sleep_time;
    DDS_Long count;
} OEIParam;

static void * publish_thread(void *arg);


pthread_mutex_t mutex;
pthread_cond_t cond;


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

    OEIParam param;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

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


    init_subscriber("client", "server", domain_id,  udp_intf, peer, 
            sleep_time, count);

    param.count = count;
    param.peer = peer;
    param.sleep_time = sleep_time;
    param.udp_intf = udp_intf;
    param.domain_id = domain_id + 1;

    pthread_create(&publish_tid, NULL, publish_thread, &param);


    for(i=0; i <9999; i++){
        char *msg = subscribe_msg(1000);
        printf("Main: Recv msg: %s\n",msg);

        if(strcmp(msg, "q\n") == 0 || strcmp(msg, "q") ==0 )
            break;

        //wake up publish thread
        pthread_mutex_lock(&mutex);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        //publish_msg(response);
    }

    
    return 0;
}


static void * publish_thread(void *arg){
    OEIParam *param = (OEIParam *)arg;

    init_publisher("server", "client", param->domain_id, param->udp_intf,
            param->peer, param->sleep_time, param->count);
    
    printf("Start publish thread.\n");
    const char *resp = "This is response";

    while(1){
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);

        printf("Publish a response!\n");
        publish_msg(resp);

        pthread_mutex_unlock(&mutex);
    }

}
