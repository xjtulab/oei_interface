#ifndef dds_cpp_domain_hxx
#define dds_cpp_domain_hxx

/* ================================================================= */
/*i @file
  @ingroup DDSDomainModule

  @brief Defines the \dds domain package
*/

#ifndef dds_cpp_dll_hxx
  #include "dds_cpp/dds_cpp_dll.hxx"
#endif
#ifndef dds_cpp_infrastructure_hxx
  #include "dds_cpp/dds_cpp_infrastructure.hxx"
#endif
#ifndef dds_cpp_topic_hxx
  #include "dds_cpp/dds_cpp_topic.hxx"
#endif
#ifndef dds_cpp_publication_hxx
  #include "dds_cpp/dds_cpp_publication.hxx"
#endif
#ifndef dds_cpp_subscription_hxx
  #include "dds_cpp/dds_cpp_subscription.hxx"
#endif
#ifndef dds_cpp_rt_hxx
#include "dds_cpp/dds_cpp_rt.hxx"
#endif
#ifndef dds_c_domain_h
  #include "dds_c/dds_c_domain.h"
#endif


/* ================================================================= */
/*                       Listeners                                   */
/* ================================================================= */

/*e \dref_DomainParticipantListener
 *
 *  \ndds C++ API avoids the use of multiple inheritance for certification
 *  purposes. For this reason, DDSDomainParticipantListener is not
 *  declared as a sub-class of DDSPublisherListener, DDSSubscriberListener,
 *  and DDSTopicListener, but it explicitly declares all supported
 *  call-backs.
 */
class DDSCPPDllExport DDSDomainParticipantListener
{
  public:
    /*e \dref_DomainParticipantListener_on_data_available
     */
    virtual void on_data_available(DDSDataReader*) { }

    /*e \dref_DomainParticipantListener_on_requested_deadline_missed
     */
    virtual void on_requested_deadline_missed(
            DDSDataReader*,
            const DDS_RequestedDeadlineMissedStatus&) { }

    /*e \dref_DomainParticipantListener_on_liveliness_changed
     */
    virtual void on_liveliness_changed(
            DDSDataReader *,
            const DDS_LivelinessChangedStatus&) { }

    /*e \dref_DomainParticipantListener_on_requested_incompatible_qos
     */
    virtual void on_requested_incompatible_qos(
            DDSDataReader *,
            const DDS_RequestedIncompatibleQosStatus&) { }

    /*e \dref_DomainParticipantListener_on_sample_rejected
     */
    virtual void on_sample_rejected(
            DDSDataReader *,
            const DDS_SampleRejectedStatus&) { }

    /*e \dref_DomainParticipantListener_on_subscription_matched
     */
    virtual void on_subscription_matched(
            DDSDataReader *,
            const DDS_SubscriptionMatchedStatus&) { }

    /*e \dref_DomainParticipantListener_on_sample_lost
     */
    virtual void on_sample_lost(
            DDSDataReader *,
            const DDS_SampleLostStatus&) { }

    /*e \dref_DomainParticipantListener_on_instance_replaced
     */
    virtual void on_instance_replaced(
            DDSDataReader *,
            const DDS_DataReaderInstanceReplacedStatus&) { }

    /*e \dref_DomainParticipantListener_on_before_sample_deserialize
     */
    virtual DDS_Boolean on_before_sample_deserialize(
            DDSDataReader *,
            NDDS_Type_Plugin *,
            CDR_Stream_t *,
            DDS_Boolean *)
    {
        return OEI_TRUE;
    }

    /*e \dref_DomainParticipantListener_on_before_sample_commit
     */
    virtual DDS_Boolean on_before_sample_commit(
            DDSDataReader *,
            const void *const,
            const struct DDS_SampleInfo *const,
            DDS_Boolean *)
    {
        return OEI_TRUE;
    }

    /*e \dref_DomainParticipantListener_on_data_on_readers
     */
    virtual void on_data_on_readers(DDSSubscriber*) { }

    /*e \dref_DomainParticipantListener_on_offered_deadline_missed
     */
    virtual void on_offered_deadline_missed(
            DDSDataWriter *,
            const struct DDS_OfferedDeadlineMissedStatus&) { }

    /*e \dref_DomainParticipantListener_on_liveliness_lost
     */
    virtual void on_liveliness_lost(
            DDSDataWriter *,
            const struct DDS_LivelinessLostStatus&) { }

    /*e \dref_DomainParticipantListener_on_offered_incompatible_qos
     */
    virtual void on_offered_incompatible_qos(
            DDSDataWriter *,
            const struct DDS_OfferedIncompatibleQosStatus&) { }

    /*e \dref_DomainParticipantListener_on_publication_matched
     */
    virtual void on_publication_matched(
            DDSDataWriter *,
            const struct DDS_PublicationMatchedStatus&) { }

    /*e \dref_DomainParticipantListener_on_reliable_reader_activity_changed
     */
    virtual void on_reliable_reader_activity_changed(
            DDSDataWriter *,
             const struct DDS_ReliableReaderActivityChangedStatus&) { }

    /*e \dref_DomainParticipantListener_on_reliable_sample_unacknowledged
     */
    virtual void on_reliable_sample_unacknowledged(
            DDSDataWriter *,
            const DDS_ReliableSampleUnacknowledgedStatus&) { }

    /*e \dref_DomainParticipantListener_on_inconsistent_topic
     */
    virtual void on_inconsistent_topic(
            DDSTopic *,
            const DDS_InconsistentTopicStatus&) { }

  public:
    DDSDomainParticipantListener() { }
    virtual ~DDSDomainParticipantListener() { }
};

/* ================================================================= */
/*                       DDSDomainParticipant                        */
/* ================================================================= */

class DDSDomainParticipantFactory;
class DDSPublisherListener;
class DDSSubscriberListener;


/* ----------------------------------------------------------------- */
/*e \dref_DomainParticipant
 */
class DDSCPPDllExport DDSDomainParticipant : public DDSEntity
{

  // --- <<interface>> DDSDomainParticipant: --------------------------
  public:

    /*e \dref_DomainParticipant_as_entity
     */
    virtual DDSEntity* as_entity() = 0;

    /*e \dref_DomainParticipant_create_publisher
     */
    virtual DDSPublisher* create_publisher(
            const DDS_PublisherQos& qos,
            DDSPublisherListener* listener,
            DDS_StatusMask mask) = 0;

    /*e \dref_DomainParticipant_delete_publisher
     */
    virtual DDS_ReturnCode_t delete_publisher(
            DDSPublisher* p) = 0;

    /*e \dref_DomainParticipant_create_subscriber
     */
    virtual DDSSubscriber* create_subscriber(
            const DDS_SubscriberQos& qos,
            DDSSubscriberListener* listener,
            DDS_StatusMask mask) = 0;

    /*e \dref_DomainParticipant_delete_subscriber
     */
    virtual DDS_ReturnCode_t delete_subscriber(
            DDSSubscriber* s) = 0;

    /*e \dref_DomainParticipant_create_topic
     */
    virtual DDSTopic* create_topic(
            const char* topic_name,
            const char* type_name,
            const DDS_TopicQos& qos,
            DDSTopicListener* listener,
            DDS_StatusMask mask) = 0;

     /*e \dref_DomainParticipant_delete_topic
     */
     virtual DDS_ReturnCode_t delete_topic(
            DDSTopic* topic) = 0;

#if INCLUDE_API_LOOKUP
     /*e \dref_DomainParticipant_find_topic
     */
     virtual DDSTopic* find_topic(
            const char* topic_name,
            const DDS_Duration_t& timeout) = 0;
#endif

     /*e \dref_DomainParticipant_get_qos
     */
     virtual DDS_ReturnCode_t get_qos(
             DDS_DomainParticipantQos& qos) = 0;

     /*e \dref_DomainParticipant_set_qos
     */
     virtual DDS_ReturnCode_t set_qos(
             const DDS_DomainParticipantQos& qos) = 0;

     /*e \dref_DomainParticipant_get_default_publisher_qos
      */
     virtual DDS_ReturnCode_t get_default_publisher_qos(
            DDS_PublisherQos& qos) = 0;

     /*e \dref_DomainParticipant_set_default_publisher_qos
      */
     virtual DDS_ReturnCode_t set_default_publisher_qos(
            const DDS_PublisherQos& qos) = 0;

     /*e \dref_DomainParticipant_get_default_subscriber_qos
      */
     virtual DDS_ReturnCode_t get_default_subscriber_qos(
            DDS_SubscriberQos& qos) = 0;

     /*e \dref_DomainParticipant_set_default_subscriber_qos
      */
     virtual DDS_ReturnCode_t set_default_subscriber_qos(
            const DDS_SubscriberQos& qos) = 0;

     /*e \dref_DomainParticipant_get_default_topic_qos
      */
     virtual DDS_ReturnCode_t get_default_topic_qos(
            DDS_TopicQos& qos) = 0;

     /*e \dref_DomainParticipant_set_default_topic_qos
      */
     virtual DDS_ReturnCode_t set_default_topic_qos(
            const DDS_TopicQos& qos) = 0;

     /*e \dref_DomainParticipant_lookup_topicdescription
      */
     virtual DDSTopicDescription* lookup_topicdescription(
            const char *topic_name) = 0;

     /*i \dref_DomainParticipant_get_domain_id
      */
     virtual DDS_DomainId_t get_domain_id() = 0;

     /*e \dref_DomainParticipant_delete_contained_entities
      */
     virtual DDS_ReturnCode_t delete_contained_entities() = 0;

     /*e \dref_DomainParticipant_register_type
      */
     virtual DDS_ReturnCode_t register_type(
            const char *type_name,
            NDDS_Type_Plugin *plugin) = 0;

     /*e \dref_DomainParticipant_unregister_type
      */
     virtual NDDS_Type_Plugin* unregister_type(
            const char *type_name) = 0;

    /*e \dref_DomainParticipant_set_listener
     */
    virtual DDS_ReturnCode_t set_listener(
            const DDSDomainParticipantListener *listener,
            DDS_StatusMask mask) = 0;

    /*e \dref_DomainParticipant_get_listener
     */
    virtual DDSDomainParticipantListener* get_listener() = 0;


  // --- <<lifecycle>>: ------------------------------------------------
  protected: 

    DDSDomainParticipant(DDS_DomainParticipant* c_participant);

    ~DDSDomainParticipant();
};



/* ================================================================= */
/*                       Factory                                     */
/* ================================================================= */

class DDSDomainParticipantFactory_impl;

/* ----------------------------------------------------------------- */
/*e \dref_DomainParticipantFactory
 */
class DDSCPPDllExport DDSDomainParticipantFactory
{

  // --- <<interface>> DDSDomainParticipantFactory ---------------------
  public: 
    /*e \dref_DomainParticipantFactory_get_instance
     */
    static DDSDomainParticipantFactory* get_instance();

    /*e \dref_DomainParticipantFactory_finalize_instance
     */
    static DDS_ReturnCode_t finalize_instance();

    /*e \dref_DomainParticipantFactory_create_participant
     */
    virtual DDSDomainParticipant* create_participant(
            DDS_DomainId_t domainId,
            const DDS_DomainParticipantQos& qos,
            DDSDomainParticipantListener* listener,
            DDS_StatusMask mask) = 0;

    /*e \dref_DomainParticipantFactory_delete_participant
     */
    virtual DDS_ReturnCode_t delete_participant(
            DDSDomainParticipant* a_participant) = 0;

    /*e \dref_DomainParticipantFactory_lookup_participant
     */
    virtual DDSDomainParticipant* lookup_participant(
            DDS_DomainId_t domainId) = 0;

    /*e \dref_DomainParticipantFactory_set_default_participant_qos
     */
    virtual DDS_ReturnCode_t set_default_participant_qos(
            const DDS_DomainParticipantQos& qos) = 0;

    /*e \dref_DomainParticipantFactory_get_default_participant_qos
     */
    virtual DDS_ReturnCode_t get_default_participant_qos(
            DDS_DomainParticipantQos& qos) = 0;

    /*e \dref_DomainParticipantFactory_get_qos
     */
    virtual DDS_ReturnCode_t get_qos(
            DDS_DomainParticipantFactoryQos& qos) = 0;

    /*e \dref_DomainParticipantFactory_set_qos
     */
    virtual DDS_ReturnCode_t set_qos(
            const DDS_DomainParticipantFactoryQos& qos) = 0;

    /*e \dref_DomainParticipantFactory_get_registry
     */
    virtual RTRegistry* get_registry() = 0;
 
  // --- <<eXtension>> methods: ---------------------------------------


  // --- <<lifecycle>>: ------------------------------------------------
  protected:

    virtual ~DDSDomainParticipantFactory() { }

  // --- The <<singleton>> instance: ----------------------------------
  private:
    /*i
      @brief \st_singleton The singleton instance.
     */
    static DDSDomainParticipantFactory* _instance;
    /*i
    @brief \st_singleton The singleton instance.
    */
    static volatile OEI_INT32 _instance_initialized;
   
};

/* ----------------------------------------------------------------- */

/*e \dref_TheParticipantFactory
 */
#define DDSTheParticipantFactory DDSDomainParticipantFactory::get_instance()

#endif /* dds_cpp_domain_hxx */

/* ----------------------------------------------------------------- */

