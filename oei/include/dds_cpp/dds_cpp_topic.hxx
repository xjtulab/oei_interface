#ifndef dds_cpp_topic_hxx
#define dds_cpp_topic_hxx

/*i @file
  @ingroup DDSTopicModule

  @brief Defines the \dds topic package
*/

#ifndef dds_cpp_dll_hxx
#include "dds_cpp/dds_cpp_dll.hxx"
#endif
#ifndef dds_cpp_infrastructure_hxx
#include "dds_cpp/dds_cpp_infrastructure.hxx"
#endif
#ifndef dds_c_topic_h
#include "dds_c/dds_c_topic.h"
#endif



/* ================================================================= */
/*                   DDSTopicDescription                             */
/* ================================================================= */

class DDSDomainParticipant;
class DDSDomainParticipant_impl;
class DDSTopicDescription_impl;
class DDSSubscriber_impl;
class DDSDataReader_impl;
class DDSTopic_impl;

/* ----------------------------------------------------------------- */
/*e \dref_TopicDescription
 */
class DDSCPPDllExport DDSTopicDescription
{
  friend class DDSSubscriber_impl;
  friend class DDSDomainParticipant_impl;
  friend class DDSTopicDescription_impl;
  friend class DDSDataReader_impl;
  friend class DDSTopic_impl;

  public:
    /*e \dref_TopicDescription_get_type_name
     */
    virtual const char* get_type_name() = 0;

    /*e \dref_TopicDescription_get_name
     */
    virtual const char* get_name() = 0;

    /*e \dref_TopicDescription_get_participant
     */
    virtual DDSDomainParticipant* get_participant() = 0;

    /*i
     */
    DDSTopicDescription_impl* get_impl();

  protected: 
    virtual ~DDSTopicDescription() { }

  private:
    DDSTopicDescription_impl *_topic_desc_impl;
};

/* ================================================================= */
/*                           Topic                                   */
/* ================================================================= */

class DDSTopic;
class DDSTopicListener;
/* ----------------------------------------------------------------- */
/*e \dref_Topic
 */
class DDSCPPDllExport DDSTopic : public DDSEntity, public DDSTopicDescription
{
  public: 
    /*e \dref_Topic_narrow
     */
    static DDSTopic* narrow(DDSTopicDescription* topic_description);

    /*e \dref_Topic_as_topicdescription
     */
    virtual DDSTopicDescription* as_topicdescription() = 0;

    /*e \dref_Topic_as_entity
     */
    virtual DDSEntity* as_entity() = 0;

    /*e \dref_Topic_set_qos
     */
    virtual DDS_ReturnCode_t set_qos(const DDS_TopicQos& qos) = 0;

    /*e \dref_Topic_get_qos
     */
    virtual DDS_ReturnCode_t get_qos(DDS_TopicQos& qos) = 0;

    /*e \dref_Topic_get_inconsistent_topic_status
     */
    virtual DDS_ReturnCode_t get_inconsistent_topic_status(
                DDS_InconsistentTopicStatus& status) = 0;

    /*e \dref_Topic_set_listener
     */
    virtual DDS_ReturnCode_t set_listener(
                const DDSTopicListener *listener,
                DDS_StatusMask mask) = 0;

    /*e \dref_Topic_get_listener
     */
    virtual DDSTopicListener* get_listener() = 0;

    /* TopicDescription methods */

    virtual const char* get_type_name() = 0;

    virtual const char* get_name() = 0;

    virtual DDSDomainParticipant* get_participant() = 0;

  protected:

    DDSTopic(DDS_Topic *c_topic);
    virtual ~DDSTopic();
};

/*e \dref_TopicListener
 */
class DDSCPPDllExport DDSTopicListener : public virtual DDSListener
{
  public:
    /*e \dref_TopicListener_on_inconsistent_topic
     */
    virtual void on_inconsistent_topic(
            DDSTopic *,
            const DDS_InconsistentTopicStatus&) { }

  public:
    DDSTopicListener() { }
    virtual ~DDSTopicListener() { }
};

#endif /* dds_cpp_topic_hxx */

