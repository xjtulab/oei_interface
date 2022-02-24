/*
 * FILE: dds_c_topic.h - DDS topic module
 *
 */
/*ce
 * \file
 * \brief DDS topic module
 * @addtogroup DDSDomainModule
 * @ingroup DDSCModule
 */
#ifndef dds_c_topic_h
#define dds_c_topic_h

#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#include "dds_c_config.h"
#ifndef dds_c_common_h
#include "dds_c/dds_c_common.h"
#endif
#ifndef dds_c_sequence_h
#include "dds_c/dds_c_sequence.h"
#endif
#ifndef dds_c_infrastructure_h
#include "dds_c/dds_c_infrastructure.h"
#endif

/* ================================================================= */
/*                       Basic Types                                 */
/* ================================================================= */

/*e \dref_InstanceId_t
 */
typedef struct DDS_BuiltinTopicKey_t DDS_InstanceId_t;


/* ================================================================= */
/*                       Status                                      */
/* ================================================================= */

/*e \dref_InconsistentTopicStatus
 */
struct DDSCPPDllExport DDS_InconsistentTopicStatus
{
    /*i \dref_InconsistentTopicStatus_total_count
     */
    DDS_Long total_count;

    /*i \dref_InconsistentTopicStatus_total_count_change
     */
    DDS_Long total_count_change;
    
    DDSC_CPP_STATUS_METHODS(DDS_InconsistentTopicStatus)
};

#ifdef __cplusplus
extern "C"
{
#endif

/*ci
 * \brief Initialize a DDS_InconsistentTopicStatus structure
 *
 * \param[in] self DDS_InconsistentTopicStatus to initialize.
 *
 * \return DDS_RETCODE_OK on success, one of the standard error codes on
 *         failure
 */
DDSCDllExport DDS_ReturnCode_t
DDS_InconsistentTopicStatus_initialize(
            struct DDS_InconsistentTopicStatus *self);

/*ce \dref_InconsistentTopicStatus_INITIALIZER
 */
#define DDS_InconsistentTopicStatus_INITIALIZER { 0L, 0L }

/*ci
 * \brief Reset the changed counters in DDS_InconsistentTopicStatus
 *
 * \param[in] s Structure to clear
 */
DDSCDllExport void
DDS_InconsistentTopicStatus_reset(struct DDS_InconsistentTopicStatus *s);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

/* ================================================================= */
/*                          QoS                                      */
/* ================================================================= */

/*e \dref_TopicQos
 */
struct DDSCPPDllExport DDS_TopicQos
{
    /*i \dref_TopicQos_management
     */
    struct OEI_ManagementQosPolicy management;

    DDSC_CPP_QOS_METHODS(DDS_TopicQos)
};

#ifdef __cplusplus
extern "C" {
#endif

/*ce \dref_TopicQos_initialize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_TopicQos_initialize(struct DDS_TopicQos *self);

/* #if INCLUDE_API_QOS */
/*ce \dref_TopicQos_copy
 */
DDSCDllExport DDS_ReturnCode_t
DDS_TopicQos_copy(struct DDS_TopicQos *self,
        const struct DDS_TopicQos *source);

#ifndef OEI_CERT
/*ce \dref_TopicQos_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_TopicQos_finalize(struct DDS_TopicQos *self);
#endif /* !OEI_CERT */

/*ci
 * \brief Compare two DDS_TopicQos policies for equality
 *
 * \param[in] left  The left side of the comparison
 * \param[in] right The right side of the comparison
 *
 * \return DDS_BOOLEAN_TRUE if the structures are equal,
 *         DDS_BOOLEAN_FALSE otherwise
 */
DDSCDllExport DDS_Boolean
DDS_TopicQos_is_equal(const struct DDS_TopicQos *left,
                      const struct DDS_TopicQos *right);

/*ce \dref_TopicQos_INITIALIZER
 */
#define DDS_TopicQos_INITIALIZER   {   \
    OEI_MANAGEMENT_QOS_POLICY_DEFAULT  \
}

/* ================================================================= */
/*                   DDS_TopicDescription                            */
/* ================================================================= */

/*ce \dref_TopicDescription
 */
typedef struct DDS_TopicDescriptionImpl DDS_TopicDescription;

/*ce \dref_TopicDescription_get_type_name
 */
DDSCDllExport const char*
DDS_TopicDescription_get_type_name(DDS_TopicDescription * self);

/*ce \dref_TopicDescription_get_name
 */
DDSCDllExport const char*
DDS_TopicDescription_get_name(DDS_TopicDescription * self);

/*ce \dref_TopicDescription_get_participant
 */
DDSCDllExport DDS_DomainParticipant*
DDS_TopicDescription_get_participant(DDS_TopicDescription * self);

/*ce \dref_Topic
 */
typedef struct DDS_TopicImpl DDS_Topic;

/* ================================================================= */
/*                       Listeners                                   */
/* ================================================================= */

/*ce \dref_TopicListener_InconsistentTopicCallback
 */
typedef void
(*DDS_TopicListener_InconsistentTopicCallback)(
        void *listener_data,
        DDS_Topic *topic,
        const struct DDS_InconsistentTopicStatus *status);

/*ce \dref_TopicListener
 */
struct DDS_TopicListener
{
    /*ce \dref_TopicListener_as_listener
     */
    struct DDS_Listener as_listener;

    /*ce \dref_TopicListener_on_inconsistent_topic
     */
    DDS_TopicListener_InconsistentTopicCallback on_inconsistent_topic;
};

/*ce \dref_TopicListener_INITIALIZER
 */
#define DDS_TopicListener_INITIALIZER \
{ DDS_Listener_INITIALIZER, (DDS_TopicListener_InconsistentTopicCallback)NULL}

/* ================================================================= */
/*                           Topic                                   */
/* ================================================================= */
/*ce \dref_Topic_as_entity
 */
DDSCDllExport DDS_Entity*
DDS_Topic_as_entity(DDS_Topic * self);

/*ce \dref_Topic_as_topicdescription
 */
DDSCDllExport DDS_TopicDescription*
DDS_Topic_as_topicdescription(DDS_Topic * self);

/*ce \dref_Topic_narrow
 */
DDSCDllExport DDS_Topic*
DDS_Topic_narrow(DDS_TopicDescription *self);

/*ce \dref_Topic_get_inconsistent_topic_status
*/
DDSCDllExport DDS_ReturnCode_t
DDS_Topic_get_inconsistent_topic_status(
        DDS_Topic *self,
        struct DDS_InconsistentTopicStatus *status);

#if INCLUDE_API_QOS
/*ce \dref_Topic_set_qos
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Topic_set_qos(
        DDS_Topic *self,
        const struct DDS_TopicQos *qos);

/*ce \dref_Topic_get_qos
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Topic_get_qos(DDS_Topic * self,struct DDS_TopicQos *qos);
#endif

#ifndef OEI_CERT 
/*ce \dref_Topic_set_listener
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Topic_set_listener(
        DDS_Topic *self,
        const struct DDS_TopicListener *l,
        DDS_StatusMask mask);
#endif 

#ifndef OEI_CERT 
/*ce \dref_Topic_get_listener
 */
DDSCDllExport struct DDS_TopicListener
DDS_Topic_get_listener(DDS_Topic *self);
#endif 

/* ----------------------------------------------------------------- */

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* dds_c_topic_h */
