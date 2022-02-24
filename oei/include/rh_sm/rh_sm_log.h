/*
 * FILE: rh_sm_log.h - Reader Log definitions
 *
 */
/*ce
 * \file 
 * \brief RH module log codes 
 */
#ifndef rh_sm_log_h
#define rh_sm_log_h

#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

/*ce
 * \defgroup RHLogCodesClass RH 
 * \brief Reader History. ModuleID = 8 
 * \ingroup LoggingModule
 */

/*ce
 * \brief Failed to delete Reader History due to outstanding, unremoved sample
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_OUTSTANDING_SAMPLE_EC                       (RHSM_LOG_BASE + 1)
#define RHSM_LOG_OUTSTANDING_SAMPLE(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_OUTSTANDING_SAMPLE_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief Failed to create Reader History due to unspecified property Qos
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_NO_PROPERTY_QOS_EC                          (RHSM_LOG_BASE + 2)
#define RHSM_LOG_NO_PROPERTY_QOS(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_NO_PROPERTY_QOS_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief KEEP_ALL History kind is not supported
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_KEEP_ALL_HISTORY_NOT_SUPPORTED_EC           (RHSM_LOG_BASE + 3)
#define RHSM_LOG_KEEP_ALL_HISTORY_NOT_SUPPORTED(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_KEEP_ALL_HISTORY_NOT_SUPPORTED_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief DataReaderQos.resource_limits.max_samples set too small 
 *  
 * \details DataReaderQos.resource_limits.max_samples too small, must be at least 
 *        DataReaderQos.resource_limits.max_instances *
 *        DataReaderQos.resource_limits.max_samples_per_instance
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_MAX_SAMPLE_TOO_SMALL_EC                     (RHSM_LOG_BASE + 4)
#define RHSM_LOG_MAX_SAMPLE_TOO_SMALL(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_MAX_SAMPLE_TOO_SMALL_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief Time-based filtering is not supported
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_TBF_NOT_SUPPORTED_EC                        (RHSM_LOG_BASE + 5)
#define RHSM_LOG_TBF_NOT_SUPPORTED(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_TBF_NOT_SUPPORTED_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief Failed to allocate a buffer pool for sample info 
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_SAMPLE_INFO_POOL_EC                         (RHSM_LOG_BASE + 6)
#define RHSM_LOG_SAMPLE_INFO_POOL(level_,size_,count_) \
OSAPI_LOG_ENTRY_ADD_2INT((level_),RHSM_LOG_SAMPLE_INFO_POOL_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM ,\
        "size",(size_),"count=%d",(count_))

/*ce
 * \brief Failed to allocate a buffer pool for sample pointers
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_SAMPLE_POOL_EC                              (RHSM_LOG_BASE + 7)
#define RHSM_LOG_SAMPLE_POOL(level_,size_,count_) \
OSAPI_LOG_ENTRY_ADD_2INT((level_),RHSM_LOG_SAMPLE_POOL_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM,\
        "size",(size_),"count=%d",(count_))

/*ce
 * \brief Failed to get sample buffer. 
 *  
 * \details May have exceeded 
 *        DataReaderQos.reader_resource_limits.max_outstanding_reads 
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_SAMPLE_PTR_ARRAY_EC                         (RHSM_LOG_BASE + 8)
#define RHSM_LOG_SAMPLE_PTR_ARRAY(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_SAMPLE_PTR_ARRAY_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief Failed to get sample info buffer. 
 *  
 * \details May have exceeded 
 *        DataReaderQos.reader_resource_limits.max_outstanding_reads 
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_INFO_ARRAY_EC                               (RHSM_LOG_BASE + 9)
#define RHSM_LOG_INFO_ARRAY(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_INFO_ARRAY_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief Failed to get time for reception timestamp.
 *  
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_GET_RECEPTION_TIMESTAMP_EC                 (RHSM_LOG_BASE + 10)
#define RHSM_LOG_GET_RECEPTION_TIMESTAMP(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_GET_RECEPTION_TIMESTAMP_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief An invalid instance replacement policy was specified.
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_INVALID_INSTANCE_REPLACEMENT_EC            (RHSM_LOG_BASE + 11)
#define RHSM_LOG_INVALID_INSTANCE_REPLACEMENT(level_,kind_) \
OSAPI_LOG_ENTRY_ADD_1INT((level_),RHSM_LOG_INVALID_INSTANCE_REPLACEMENT_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM ,"kind",kind_)

/*ce
 * \brief Failed to remove the oldest sample
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_FAILED_TO_REMOVE_OLDEST_EC                 (RHSM_LOG_BASE + 12)
#define RHSM_LOG_FAILED_TO_REMOVE_OLDEST(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_FAILED_TO_REMOVE_OLDEST_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief Sample pool empty, may have exceeded
 *
 * \details
 *
 * The sample pool is empty, may have exceeded
 * DataReaderQos.resource_limits.max_samples
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_SAMPLE_POOL_EMPTY_EC                       (RHSM_LOG_BASE + 13)
#define RHSM_LOG_SAMPLE_POOL_EMPTY(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_SAMPLE_POOL_EMPTY_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief Failed to prune remote writer entry
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_RW_PRUNE_FAILED_EC                         (RHSM_LOG_BASE + 14)
#define RHSM_LOG_RW_PRUNE_FAILED(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_RW_PRUNE_FAILED_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief Failed to reserve an entry
 *
 * \details
 *
 * An entry reservation failed beyond normal resource settings
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_ENTRY_RESERVATION_FAILED_EC                (RHSM_LOG_BASE + 15)
#define RHSM_LOG_ENTRY_RESERVATION_FAILED(level_,reason_) \
OSAPI_LOG_ENTRY_ADD_1INT((level_),RHSM_LOG_ENTRY_RESERVATION_FAILED_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM ,"reason",(reason_))

/*ce
 * \brief Failed to return a sample
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_RETURN_SAMPLE_EC                           (RHSM_LOG_BASE + 16)
#define RHSM_LOG_RETURN_SAMPLE(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_RETURN_SAMPLE_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

/*ce
 * \brief Failed to update the history queue
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_HISTORY_UPDATE_EC                          (RHSM_LOG_BASE + 17)
#define RHSM_LOG_HISTORY_UPDATE(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_HISTORY_UPDATE_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)


/*ce
 * \brief Failed to get the system time
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_GET_TIME_EC                                (RHSM_LOG_BASE + 18)
#define RHSM_LOG_GET_TIME(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_GET_TIME_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)


/*ce
 * \brief A history object
 */
#define RHSM_LOG_HISTORY_OBJECT                                      1

/*ce
 * \brief A removal list object
 */
#define RHSM_LOG_REMOVELIST_OBJECT                                   2

/*ce
 * \brief A remote writer pool object
 */
#define RHSM_LOG_RWPOOL_OBJECT                                       3

/*ce
 * \brief A remote writer index object
 */
#define RHSM_LOG_RWINDEX_OBJECT                                      4

/*ce
 * \brief A Key object
 */
#define RHSM_LOG_KEY_OBJECT                                          5

/*ce
 * \brief A Key object
 */
#define RHSM_LOG_RW_OBJECT                                           6

/*ce
 * \brief A Key pool object
 */
#define RHSM_LOG_KEYPOOL_OBJECT                                      7

/*ce
 * \brief A key owner object
 */
#define RHSM_LOG_OWNER_OBJECT                                        8

/*ce
 * \brief A sample pool object
 */
#define RHSM_LOG_SAMPLEPOOL_OBJECT                                   9

/*ce
 * \brief A key index pool
 */
#define RHSM_LOG_KEYINDEXPOOL_OBJECT                                10

/*ce
 * \brief A sample ptr pool object
 */
#define RHSM_LOG_SAMPLEPTPOOL_OBJECT                                11

/*ce
 * \brief A sample info pool
 */
#define RHSM_LOG_SAMPLEINFO_OBJECT                                  12

/*ce
 * \brief A sample info pool
 */
#define RHSM_LOG_INSTANCEHANDLE_OBJECT                              13

/*ce
 * \brief Failed to allocate object of the specified kind
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_OBJECT_ALLOCATE_EC                         (RHSM_LOG_BASE + 19)
#define RHSM_LOG_OBJECT_ALLOCATE(level_,kind_) \
OSAPI_LOG_ENTRY_ADD_1INT((level_),RHSM_LOG_OBJECT_ALLOCATE_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM ,"kind",(kind_))


/*ce
 * \brief Failed to delete object of the specified kind
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_OBJECT_DELETE_EC                           (RHSM_LOG_BASE + 20)
#define RHSM_LOG_OBJECT_DELETE(level_,kind_) \
OSAPI_LOG_ENTRY_ADD_1INT((level_),RHSM_LOG_OBJECT_DELETE_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM, "kind",(kind_))

/*ce
 * \brief Failed to index an object of the specified kind
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_OBJECT_INDEX_EC                            (RHSM_LOG_BASE + 21)
#define RHSM_LOG_OBJECT_INDEX(level_,kind_) \
OSAPI_LOG_ENTRY_ADD_1INT((level_),RHSM_LOG_OBJECT_INDEX_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM ,"kind",(kind_))


/*ce
 * \brief Invalid object specified
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_OBJECT_INVALID_EC                          (RHSM_LOG_BASE + 22)
#define RHSM_LOG_OBJECT_INVALID(level_,kind_) \
OSAPI_LOG_ENTRY_ADD_1INT((level_),RHSM_LOG_OBJECT_INVALID_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM ,"kind",(kind_))

/*ce
 * \brief No property when creating a reader history instance
 *
 * \ingroup RHLogCodesClass
 */
#define RHSM_LOG_NO_PROPERTY_EC                             (RHSM_LOG_BASE + 23)
#define RHSM_LOG_NO_PROPERTY(level_) \
OSAPI_LOG_ENTRY_ADD((level_),RHSM_LOG_NO_PROPERTY_EC,\
        OSAPI_LOG_MSG_PN_X2_STD_PARAM)

#endif

