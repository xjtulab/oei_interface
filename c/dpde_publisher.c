#include "dpde_publisher.h"

static DDS_Publisher *publisher;
static DDS_DataWriter *datawriter;
static HelloWorldDataWriter *hw_datawriter;
static DDS_ReturnCode_t retcode;
static HelloWorld *sample;
static struct Application *application;
static DDS_Long i;


static void
HelloWorldPublisher_on_publication_matched(void *listener_data,
                                           DDS_DataWriter * writer,
                                           const struct
                                           DDS_PublicationMatchedStatus *status)
{
    if (status->current_count_change > 0)
    {
        printf("Matched a subscriber\n");
    }
    else if (status->current_count_change < 0)
    {
        printf("Unmatched a subscriber\n");
    }
}


void init_publisher(const char *local_name, const char *remote_name, DDS_Long domain_id, 
                        char *udp_intf, char *peer, DDS_Long sleep_time, DDS_Long count)
{
    struct DDS_DataWriterQos dw_qos = DDS_DataWriterQos_INITIALIZER;
    sample = NULL;
    application = NULL;
    struct DDS_DataWriterListener dw_listener =
        DDS_DataWriterListener_INITIALIZER;

    sample = HelloWorld_create();
    if (sample == NULL)
    {
        printf("failed HelloWorld_create\n");
        return;
    }

    application = Application_create(local_name, remote_name, domain_id,
                                     udp_intf, peer, sleep_time, count);

    if (application == NULL)
    {
        printf("failed Application create\n");
        goto done;
    }

    publisher = DDS_DomainParticipant_create_publisher(application->participant,
                                                       &DDS_PUBLISHER_QOS_DEFAULT,
                                                       NULL,
                                                       DDS_STATUS_MASK_NONE);
    if (publisher == NULL)
    {
        printf("publisher == NULL\n");
        goto done;
    }

    dw_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;

    dw_qos.resource_limits.max_samples = 32;
    dw_qos.resource_limits.max_samples_per_instance = 32;
    dw_qos.resource_limits.max_instances = 1;
    dw_qos.history.depth = 32;
    dw_qos.protocol.rtps_reliable_writer.heartbeat_period.sec = 0;
    dw_qos.protocol.rtps_reliable_writer.heartbeat_period.nanosec = 250000000;


    /* INTEROPERABILITY NOTE:
       Non-Connext Micro DDS writers and readers will have default LivelinessQoS
       kind set to DDS_AUTOMATIC_LIVELINESS_QOS.
     
       Because Connext Micro currently only supports
       DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS for finite lease_duration,
       a non-Connext Micro writer must set its liveliness kind to
       DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS as well in order for
       Requested-Offered (RxO) semantics to be compatible.
    */
 
    dw_listener.on_publication_matched =
        HelloWorldPublisher_on_publication_matched;
    datawriter =
        DDS_Publisher_create_datawriter(publisher, application->topic, &dw_qos,
                                        &dw_listener,
                                        DDS_PUBLICATION_MATCHED_STATUS);

    if (datawriter == NULL)
    {
        printf("datawriter == NULL\n");
        goto done;
    }

    retcode = Application_enable(application);
    if (retcode != DDS_RETCODE_OK)
    {
        printf("failed to enable application\n");
        goto done;
    }

    hw_datawriter = HelloWorldDataWriter_narrow(datawriter);


    return;

  done:

    Application_delete(application);

    DDS_DataWriterQos_finalize(&dw_qos);

    if (sample != NULL)
    {
        HelloWorld_delete(sample);
    }

}


void publish_msg(char *msg)
{
    strcpy(sample->msg, msg);
    //printf("%s\n", sample->msg);

    retcode = HelloWorldDataWriter_write(hw_datawriter, sample, &DDS_HANDLE_NIL);
    if (retcode != DDS_RETCODE_OK)
    {
        printf("Failed to write to sample\n");
    }
}