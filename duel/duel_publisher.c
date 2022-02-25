#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oei_c.h"
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"

#include "HelloWorld.h"
#include "HelloWorldSupport.h"
#include "myApplication.h"

/* See Wire Protocol Specification on http://www.omg.org/spec/DDSI-RTPS/2.2/
 * for more details about CDR encapsulation.
 */

/*ci \brief OMG CDR Big Endian encapsulation ID */
#define ENCAPSULATION_ID_CDR_BE     0x0000

/*ci \brief OMG CDR Little Endian encapsulation ID */
#define ENCAPSULATION_ID_CDR_LE     0x0001

/*ci \brief Empty encapsulation options */
#define ENCAPSULATION_OPTIONS_NONE  0x0000

/*ci \brief Unsigned Short type size */
#define UNSIGNED_SHORT_SIZE 2

/*ci \brief Unsigned Long type size */
#define UNSIGNED_LONG_SIZE  4

void
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

void
HelloWorldSubscriber_on_data_available(void *listener_data,
                                       DDS_DataReader * reader)
{
    HelloWorldDataReader *hw_reader = HelloWorldDataReader_narrow(reader);
    DDS_ReturnCode_t retcode;
    struct DDS_SampleInfo *sample_info = NULL;
    HelloWorld *sample = NULL;

    struct DDS_SampleInfoSeq info_seq = 
        DDS_SEQUENCE_INITIALIZER;
    struct HelloWorldSeq sample_seq = 
        DDS_SEQUENCE_INITIALIZER;

    const DDS_Long TAKE_MAX_SAMPLES = 32;
    DDS_Long i;

    retcode = HelloWorldDataReader_take(hw_reader,
       &sample_seq, &info_seq, TAKE_MAX_SAMPLES, 
       DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);

    if (retcode != DDS_RETCODE_OK)
    {
        printf("failed to take data, retcode(%d)\n", retcode);
        goto done;
    }

    /* Print each valid sample taken */
    for (i = 0; i < HelloWorldSeq_get_length(&sample_seq); ++i)
    {
        sample_info = DDS_SampleInfoSeq_get_reference(&info_seq, i);

        if (sample_info->valid_data)
        {
            sample = HelloWorldSeq_get_reference(&sample_seq, i);
            printf("\nSample received\n\tmsg: %s\n", sample->msg); 
        }
        else
        {
            printf("\nSample received\n\tINVALID DATA\n");
        }
    }

    HelloWorldDataReader_return_loan(hw_reader, &sample_seq, &info_seq);

done:
    HelloWorldSeq_finalize(&sample_seq);
    DDS_SampleInfoSeq_finalize(&info_seq);
}

void
HelloWorldSubscriber_on_subscription_matched(void *listener_data,
                                             DDS_DataReader * reader,
                                             const struct
                                             DDS_SubscriptionMatchedStatus
                                             *status)
{
    if (status->current_count_change > 0)
    {
        printf("Matched a publisher\n");
    }
    else if (status->current_count_change < 0)
    {
        printf("Unmatched a publisher\n");
    }
}

/*i
 * \brief Helper function to filter an HelloWorld sample
 *
 * \param[in]  sample       An HelloWorld data sample to filter
 * \param[out] drop_sample  Out parameter determining whether the sample
 *                          should be filtered out or not.
 */
void 
HelloWorldSubscriber_filter_sample(HelloWorld *sample,
                                   DDS_Boolean *drop_sample)
{
    /* Example filter: drop samples with even-numbered count in id */

    //*drop_sample = (sample->id % 2 == 0) ? DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE;
}

/*i
 * \brief Helper function to deserialize an unsigned short
 *
 * \param[in]  src_buffer      Pointer to CDR stream buffer
 * \param[in]  need_byte_swap  Indicates whether it is needed to swap bytes
 * \param[out] instance        Deserialized unsigned short
 */
void
HelloWorldSubscriber_deserialize_unsigned_short(char **src_buffer,
                                                OEI_BOOL need_byte_swap,
                                                OEI_UINT16 *instance)
{
    if (need_byte_swap)
    {
        *((OEI_INT8*)instance + 1) = *((*src_buffer)++);
        *((OEI_INT8*)instance    ) = *((*src_buffer)++);
    }
    else
    {
        *instance = *(OEI_UINT16*)(*src_buffer);
        (*src_buffer) += CDR_SHORT_SIZE;
    }
}

/*i
 * \brief Helper function to deserialize an unsigned short in big endian from
 *        a CDR stream buffer
 *
 * \param[in]  src_buffer      Pointer to CDR stream buffer
 * \param[out] instance        Deserialized unsigned short
 */
void
HelloWorldSubscriber_deserialize_unsigned_short_from_big_endian(
                                                    char **src_buffer,
                                                    OEI_UINT16 *instance)
{
#ifdef OEI_ENDIAN_LITTLE
    /* LE: always swap */
    HelloWorldSubscriber_deserialize_unsigned_short(src_buffer,
                                                    OEI_TRUE,
                                                    instance);
#else
    /* BE: never swap */
    HelloWorldSubscriber_deserialize_unsigned_short(src_buffer,
                                                    OEI_FALSE,
                                                    instance);
#endif
}

/*i
 * \brief Helper function to deserialize an unsigned long
 *
 * \param[in]  src_buffer      Pointer to CDR stream buffer
 * \param[in]  need_byte_swap  Indicates whether it is needed to swap bytes
 * \param[out] instance        Deserialized unsigned long
 */
void
HelloWorldSubscriber_deserialize_unsigned_long(char **src_buffer,
                                               OEI_BOOL need_byte_swap,
                                               DDS_UnsignedLong *instance)
{
    OEI_INT32 i;
    if (need_byte_swap)
    {
        for (i = 3; i >= 0; --i)
        {
            *((OEI_INT8*)instance + i) = *((*src_buffer)++);
        }
    }
    else
    {
        *instance = *(OEI_UINT32*)(*src_buffer);
        (*src_buffer) += CDR_LONG_SIZE;
    }
}

/*i
 * \brief Helper function to deserialize the CDR stream header
 *
 * \param[in]      src_buffer      Pointer to CDR stream buffer
 * \param[in/out]  endian          CDR stream endianness
 * \param[in/out]  need_byte_swap  Indicates whether it is needed to swap bytes
 *
 * \return OEI_TRUE if the header is correctly deserialized, otherwise OEI_FALSE
 */
DDS_Boolean
HelloWorldSubscriber_deserialize_header(char **src_buffer,
                                        OEI_UINT16 *endian,
                                        OEI_BOOL *need_byte_swap)
{
    OEI_UINT16 kind;
    OEI_UINT16 options;
    OEI_BOOL   ret_value;

    ret_value = OEI_FALSE;

    HelloWorldSubscriber_deserialize_unsigned_short_from_big_endian(
                                                            src_buffer, &kind);

    /* It is not defined whether the options are byte-swapped or not. It is not
     * important at the moment as the only supported value is 0.
     */
    HelloWorldSubscriber_deserialize_unsigned_short(src_buffer,
                                                    *need_byte_swap,
                                                    &options);

    /* No CDR options are supported */
    if (options == ENCAPSULATION_OPTIONS_NONE)
    {
        switch (kind)
        {
            case ENCAPSULATION_ID_CDR_BE:
                if ((*endian) != OEI_CDR_ENDIAN_BIG)
                {
                    *endian = OEI_CDR_ENDIAN_BIG;
                    *need_byte_swap =
                          (*need_byte_swap == OEI_TRUE) ? OEI_FALSE : OEI_TRUE;
                }
                ret_value = OEI_TRUE;
                break;
            case ENCAPSULATION_ID_CDR_LE:
                if ((*endian) != OEI_CDR_ENDIAN_LITTLE)
                {
                    *endian = OEI_CDR_ENDIAN_LITTLE;
                    *need_byte_swap =
                          (*need_byte_swap == OEI_TRUE) ? OEI_FALSE : OEI_TRUE;
                }
                ret_value = OEI_TRUE;
                break;
            default:
                break;
        }
    }

    return ret_value;
}

/*i
 * \brief Implementation of \ref DDS_DataReaderListener::on_before_sample_deserialize
 */
DDS_Boolean
HelloWorldSubscriber_on_before_sample_deserialize(
        void *listener_data,
        DDS_DataReader *reader,
        struct NDDS_Type_Plugin *plugin,
        struct CDR_Stream_t *stream,
        DDS_Boolean *dropped)
{
    DDS_Boolean result = DDS_BOOLEAN_FALSE;
    DDS_Long id = 0;
    OEI_BOOL need_byte_swap = DDS_BOOLEAN_FALSE;
    OEI_UINT16 endian = 0;
    char *src_buffer = NULL;

    need_byte_swap = stream->need_byte_swap;
    endian = stream->endian;
    src_buffer = CDR_Stream_get_current_position_ptr(stream);

    /* deserialize only header and field 'id', instead of the whole sample */

    /* Header has two shorts and we need to deserialize also the first field of
     * the sample, which is a long. So in total we will deserialize 2 short int
     * and one long
     */
    if (!CDR_Stream_check_size(stream,
                               2 * UNSIGNED_SHORT_SIZE + UNSIGNED_LONG_SIZE))
    {
        printf("Failed to deserialize header. The stream is too short, missing data\n");
        goto done;
    }

    if (!HelloWorldSubscriber_deserialize_header(&src_buffer,
                                                 &endian,
                                                 &need_byte_swap))
    {
        printf("failed deserialize header\n");
        goto done;
    }

    /* Note primitive types must be aligned to their length in the CDR stream.
     * For example, a long must start on a 4-byte boundary. The boundaries are
     * counted from the start of the CDR stream.
     * As the sample 'id' is the first data in the stream it is already aligned.
     * Position 0 (beginning of the stream) is aligned to 4 (size of long).
     */
    HelloWorldSubscriber_deserialize_unsigned_long(&src_buffer,
                                                   need_byte_swap,
                                                   (DDS_UnsignedLong*)&id);

    /* filter the sample */
    *dropped = (id % 2 == 0) ? DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE;

    if (*dropped)
    {
        printf("\nSample filtered, before deserialize...\n\tDROPPED - id: %d\n",
               id);
    }

    result = DDS_BOOLEAN_TRUE;
done:
    return result;
}

/*i
 * \brief Implementation of \ref DDS_DataReaderListener::on_before_sample_commit
 */
DDS_Boolean
HelloWorldSubscriber_on_before_sample_commit(
        void *listener_data,
        DDS_DataReader *reader,
        const void *const sample,
        const struct DDS_SampleInfo *const sample_info,
        DDS_Boolean *dropped)
{
    HelloWorld *hw_sample = (HelloWorld *)sample; 

    HelloWorldSubscriber_filter_sample(hw_sample, dropped);

    if (*dropped)
    {
        printf("\nSample filtered, before commit...\n\tDROPPED - msg: %s\n",
               hw_sample->msg);
    }

    return DDS_BOOLEAN_TRUE;
}

int
publisher_main_w_args(DDS_Long domain_id, char *udp_intf, char *peer,
                      DDS_Long sleep_time, DDS_Long count)
{
    DDS_Publisher *publisher;
    DDS_DataWriter *datawriter;
    HelloWorldDataWriter *hw_datawriter;
    DDS_Subscriber *subscriber;
    DDS_DataReader *datareader;
    struct DDS_DataReaderQos dr_qos = DDS_DataReaderQos_INITIALIZER;
    struct DDS_DataWriterQos dw_qos = DDS_DataWriterQos_INITIALIZER;
    DDS_ReturnCode_t retcode;
    HelloWorld *sample = NULL;
    struct DDS_DataReaderListener dr_listener =
        DDS_DataReaderListener_INITIALIZER;
    struct Application *application = NULL;
    DDS_Long i;
    struct DDS_DataWriterListener dw_listener =
        DDS_DataWriterListener_INITIALIZER;

    sample = HelloWorld_create();
    if (sample == NULL)
    {
        printf("failed HelloWorld_create\n");
        return 0;
    }

    application = Application_create("publisher", "subscriber", domain_id, domain_id,
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

    subscriber =
        DDS_DomainParticipant_create_subscriber(application->participant,
                                                &DDS_SUBSCRIBER_QOS_DEFAULT,
                                                NULL, DDS_STATUS_MASK_NONE);
    if (subscriber == NULL)
    {
        printf("subscriber == NULL\n");
        goto done;
    }

#if 1
    dw_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
#else
    dw_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
#endif
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
        DDS_Publisher_create_datawriter(publisher, application->pub_topic, &dw_qos,
                                        &dw_listener,
                                        DDS_PUBLICATION_MATCHED_STATUS);

    if (datawriter == NULL)
    {
        printf("datawriter == NULL\n");
        goto done;
    }

    /* Uncomment to filter samples with callbacks */
/* #define HW_USE_SAMPLE_FILTER */
#ifdef HW_USE_SAMPLE_FILTER
    /* choose one callback to enable */
#if 1
    dr_listener.on_before_sample_deserialize = 
        HelloWorldSubscriber_on_before_sample_deserialize;
#else
    dr_listener.on_before_sample_commit = 
        HelloWorldSubscriber_on_before_sample_commit;
#endif

#endif

    /* INTEROPERABILITY NOTE:
       Non-Connext Micro DDS writers and readers will have default LivelinessQoS
       kind set to DDS_AUTOMATIC_LIVELINESS_QOS.
     
       Because Connext Micro currently only supports
       DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS for finite lease_duration,
       a non-Connext Micro writer must set its liveliness kind to
       DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS as well in order for
       Requested-Offered (RxO) semantics to be compatible.
    */

    dr_listener.on_data_available = HelloWorldSubscriber_on_data_available;
    dr_listener.on_subscription_matched =
        HelloWorldSubscriber_on_subscription_matched;

    /* Publisher sends samples with id = 0 or id = 1, so 2 instances maximum.
     * But in case filtering is done, all samples with 'id = 0' are
     * filtered so only one instance is needed.
     */
#ifdef HW_USE_SAMPLE_FILTER
    dr_qos.resource_limits.max_instances = 1;
#else
    dr_qos.resource_limits.max_instances = 2;
#endif

    dr_qos.resource_limits.max_samples_per_instance = 32;
    dr_qos.resource_limits.max_samples = dr_qos.resource_limits.max_instances *
                                dr_qos.resource_limits.max_samples_per_instance;
    /* if there are more remote writers, you need to increase these limits */
    dr_qos.reader_resource_limits.max_remote_writers = 10;
    dr_qos.reader_resource_limits.max_remote_writers_per_instance = 10;
    dr_qos.history.depth = 32;

    /* Reliability QoS */
#if 1
    dr_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
#else
    dr_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
#endif

    datareader = DDS_Subscriber_create_datareader(subscriber,
                                                  DDS_Topic_as_topicdescription
                                                  (application->sub_topic), &dr_qos,
                                                  &dr_listener,
                                                  DDS_DATA_AVAILABLE_STATUS |
                                                  DDS_SUBSCRIPTION_MATCHED_STATUS);

    if (datareader == NULL)
    {
        printf("datareader == NULL\n");
        goto done;
    }

    retcode = Application_enable(application);
    if (retcode != DDS_RETCODE_OK)
    {
        printf("failed to enable application\n");
        goto done;
    }

    hw_datawriter = HelloWorldDataWriter_narrow(datawriter);

    for (i = 0; (application->count > 0 && i < application->count) ||
         (application->count == 0); ++i)
    {
        sprintf(sample->msg, "Hello World! (%d)", i);
        printf("%s\n", sample->msg);

        retcode = HelloWorldDataWriter_write(hw_datawriter, sample, &DDS_HANDLE_NIL);
        if (retcode != DDS_RETCODE_OK)
        {
            printf("Failed to write to sample\n");
        }

        OSAPI_Thread_sleep(application->sleep_time);
    }

  done:

    Application_delete(application);

    DDS_DataWriterQos_finalize(&dw_qos);

    if (sample != NULL)
    {
        HelloWorld_delete(sample);
    }

    return 0;
}

#if !(defined(OEI_VXWORKS) && !defined(__RTP__))
int
main(int argc, char **argv)
{

    DDS_Long i = 0;
    DDS_Long domain_id = 0;
    char *peer = "127.0.0.1";
    char *udp_intf = "lo";
    DDS_Long sleep_time = 1000;
    DDS_Long count = 0;

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

    return publisher_main_w_args(domain_id, udp_intf, peer, sleep_time, count);
}
#elif defined(OEI_VXWORKS)
int
publisher_main(void)
{
    /* Explicitly configure args below */
    DDS_Long i = 0;
    DDS_Long domain_id = 44;
    char *peer = "10.10.30.101";
    char *udp_intf = NULL;
    DDS_Long sleep_time = 1000;
    DDS_Long count = 0;

    return publisher_main_w_args(domain_id, udp_intf, peer, sleep_time, count);
}
#endif