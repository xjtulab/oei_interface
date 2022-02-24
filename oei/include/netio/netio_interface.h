/*
 * FILE: netio_interface.h - NETIO API
 *
 */

/*ce
 * \file
 * \brief Generic NETIO_Interface functions
 *
 * \details
 * This file contains generic NETIO interface functions. A NETIO_Interface is
 * a base-class and is typically not instantiated on its own. Derived classes
 * call methods here to initialize/finalize a base-class. This file also
 * defines the NETIO interface structure as well as the NETIO_Packet structure
 * and protocol types.
 */
#ifndef netio_interface_h
#define netio_interface_h

#ifndef netio_dll_h
#include "netio/netio_dll.h"
#endif

#ifndef osapi_config_h
#include "osapi/osapi_config.h"
#endif

#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef reda_sequenceNumber_h
#include "reda/reda_sequenceNumber.h"
#endif

#ifndef db_api_h
#include "db/db_api.h"
#endif

#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif

#ifndef netio_address_h
#include "netio/netio_address.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*ce
 * \brief Default name for the automatically registered UDP transport
 */
NETIODllVariable extern const char* const NETIO_DEFAULT_UDP_NAME;

/*ce
 * \brief Default name for the automatically registered INTRA transport
 */
NETIODllVariable extern const char* const NETIO_DEFAULT_INTRA_NAME;

/*ce
 * \brief Default name for the automatically registered RTPS interface
 */
NETIODllVariable extern const char* const NETIO_DEFAULT_RTPS_NAME;

/*ci
 * \defgroup  NETIO_PacketClass NETIO Packet API
 * \ingroup NETIOCommon
 * \brief NETIO_Packet implementation
 */
/*ci
 * \addtogroup NETIO_PacketClass
 * @{
 */
/*
 * The following constants are passed as part of a NETIO_Packet to
 * indicate what protocol it originated from. It may be used the receiver
 */
/*ci
 * \def   NETIO_PROTOCOL_INTRA
 * \brief The packet originates from the INTRA protocol
 */
#define NETIO_PROTOCOL_INTRA               (1)

/*ci
 * \def   NETIO_PROTOCOL_RTPS
 * \brief The packet originates from the RTPS protocol
 */
#define NETIO_PROTOCOL_RTPS                (2)

/*ci
 * \def   NETIO_PROTOCOL_UDP
 * \brief The packet originates from the UDP protocol
 */
#define NETIO_PROTOCOL_UDP                 (3)

/*
 * The following constants are passed as part of a NETIO_Packet to
 * provide meta information about the content (if any).
 */
/*ci
 * \def   NETIO_RTPS_FLAGS_DISPOSE
 * \brief The packet is a RTPS dispose message, a key is disposed
 */
#define NETIO_RTPS_FLAGS_DISPOSE           (0x1)

/*ci
 * \def   NETIO_RTPS_FLAGS_UNREGISTER
 * \brief The packet is a RTPS unregister message, a key has been unregistered
 *        by a writer
 */
#define NETIO_RTPS_FLAGS_UNREGISTER        (0x2)

/*ci
 * \def   NETIO_RTPS_FLAGS_LIVELINESS
 * \brief The packet counts as liveliness, even if it does not contain any data
 */
#define NETIO_RTPS_FLAGS_LIVELINESS        (0x4)

/*ci
 * \def  NETIO_RTPS_FLAGS_INLINEQOS
 * \brief The packet contains inline Qos as defined by RTPS
 */
#define NETIO_RTPS_FLAGS_INLINEQOS         (0x8)

/*ci
 * \def  NETIO_RTPS_FLAGS_LOST_DATA
 * \brief The flag indicates that some packets have been permanently lost
 *        and any attempt to wait for them should be aborted
 */
#define NETIO_RTPS_FLAGS_LOST_DATA         (0x10)

/*ci
 * \def  NETIO_RTPS_FLAGS_COMMIT_DATA
 * \brief The flag indicates that even if the packet does not contain
 *        data, it does contain information about which packets can be considered
 *        to be received in order.
 */
#define NETIO_RTPS_FLAGS_COMMIT_DATA       (0x20)

/*ci
 * \def  NETIO_RTPS_FLAGS_DATA
 * \brief The flag indicates that the packet contains data
 */
#define NETIO_RTPS_FLAGS_DATA              (0x40)

/*ci
 * \def  NETIO_RTPS_FLAGS_LAST_WRITE_FOR_SN
 * \brief
 */
#define NETIO_RTPS_FLAGS_LAST_WRITE_FOR_SN (0x80)

/*ci
 * \def  NETIO_RTPS_FLAGS_LITTLE_ENDIAN
 * \brief The packet payload is in little endian format
 */
#define NETIO_RTPS_FLAGS_LITTLE_ENDIAN (0x100)

/*ci
 * \brief RTPS protocol specific data
 */
struct NETIO_RtpsInfo
{
    /*ci
     * \brief Pointer to beginning of data-payload
     */
    struct REDA_Buffer *inline_data;

    /*ci
     * \brief The GUID prefix of the peer
     */
    struct NETIO_GuidPrefix guid_prefix;
};

/*ci
 * \brief INTRA protocol specific data
 */
struct NETIO_IntraInfo
{
    /*ci
     * \brief opaque pointer to in-memory representation of data
     */
    const void *user_data;

    /*ci
     * \brief Pointer to beginning of intra data-payload. Intra data-payload
     *        only contains inline Qos
     */
    struct REDA_Buffer *inline_data;
};

/*ci
 *\brief NETIO_IntraInfo initializer constant
 */
#define NETIO_IntraInfo_INITIALIZER \
{ \
    NULL, /* user_data */ \
    NULL, /* inline_data */ \
}

/*ci
 *\brief Structure containing meta-data about a \ref NETIO_Packet payload
 */
struct NETIO_PacketInfo
{
    /*ci
     * \brief The SN of the packet. The SN and PacketId is synonymous
     */
    struct REDA_SequenceNumber sn;

    /*ci
     * \brief The reception time-stamp of the packet
     */
    struct OSAPI_NtpTime timestamp;

    /*ci
     * \brief Whether the serialized-payload is valid data or not
     */
    OEI_UINT8 valid_data;

    /*ci
     * \brief Whether the serialized-payload is a valid key or not
     */
    OEI_UINT8 valid_key;

    /*ci
     * \brief Key for the sample
     */
    struct NETIO_Guid instance; 

    /*ci
     * \brief Flags describing the contents of the payload
     */
    OEI_UINT32 rtps_flags;

    /*ci
     * \brief The committable_sn means that all SNs up to, but not including
     *        this SN can be committed (made available to the user).
     */
    struct REDA_SequenceNumber committable_sn;

    /*ci
     * \brief The first_available_sn indicates the first available SN which is
     *        available. It is typically used downstream to boot-strap the
     *        reliability protocol.
     */
    struct REDA_SequenceNumber first_available_sn;

    /*ci
     * \brief If NETIO_RTPS_FLAGS_LOST_DATA is set, this is the highest sequence
     *        number which has been lost.
     */
    struct REDA_SequenceNumber lost_sample_sn;

    /*ci
     * \brief If NETIO_RTPS_FLAGS_LOST_DATA is set, this is the total number
     *        of samples that have been lost up to and including lost_sample_sn
     */
    OEI_UINT32 lost_sample_count;

    /*ci
     * \brief The protocol which produced this packet, it may change as the
     *        packet traverses a NETIO stack
     */
    OEI_INT32 protocol_id;

    /*ci
     * \brief Protocol specific data based, protocol_id identifies which one
     *        is valid.
     */
    struct
    {
        struct NETIO_IntraInfo intra_info;
        struct NETIO_RtpsInfo rtps_data;
    } protocol_data;
};

/*ci
 * \def NETIO_PacketInfo_INITIALIZER
 * \brief Constant to initialize NETIO_PacketInfo
 */
#define NETIO_PacketInfo_INITIALIZER \
{ \
    REDA_SEQUENCE_NUMBER_ZERO, /* sn */\
    OSAPI_NTP_TIME_ZERO, /* timestamp */ \
    0, /* valid_data */ \
    0, /* valid_key */ \
    NETIO_ADDRESS_GUID_UNKNOWN, /* instance */ \
    0, /* flags */ \
    REDA_SEQUENCE_NUMBER_ZERO, /* committable_sn */ \
    REDA_SEQUENCE_NUMBER_ZERO, /* first_available_sn */ \
    REDA_SEQUENCE_NUMBER_ZERO, /* lost_sample_sn */ \
    0, /* lost sample count */ \
    0, /* protocol_id */\
    {{NULL,NULL},{NULL,{{0,0,0,0,0,0,0,0,0,0,0,0}}}}\
}

/*ci
 * \brief Implementation of the abstract NETIO_Packet type.
 *
 * \details
 * The NETIO layers passed \ref NETIO_Packet structures upstream/downstream.
 * Any meta-data about the packet must be encapsulated either in the packet
 * payload or the packet info field.
 */
struct NETIO_Packet
{
    /*ci
     * \brief The source of the packet, typically the peer address
     */
    struct NETIO_Address source;

    /*ci
     * \brief The local source of the packet, the address it was received on.
     */
    struct NETIO_Address local_source;

    /*ci
     * \brief Buffer holding a NETIO_Packet. The buffer must be allocated
     *        and managed outside of this structure. That, is NETIO_Packet
     *        only manages payloads by reference.
     */
    char *buffer;

    /*ci
     * \brief The maximum number of bytes buffer can hold
     */
    OEI_SIZE_T max_length;

    /*ci
     * \brief The current position of head cursor
     */
    OEI_SIZE_T head_pos;

    /*ci
     * \brief The current position of tail cursor
     */
    OEI_SIZE_T tail_pos;

    /*ci
     * \brief Temporary storage for the current head position so it can later
     *        be restored if a packet payload is traversed multiple times
     */
    OEI_SIZE_T saved_head_pos;

    /*ci
     * \brief Temporary storage for the current tail position so it can later
     *        be restored if a packet payload is traversed multiple times
     */
    OEI_SIZE_T saved_tail_pos;

    /*ci
     * \brief Meta data about the packet, refer to \ref NETIO_PacketInfo for
     *        details
     */
    struct NETIO_PacketInfo info;

    /*ci
     * \brief Sequence of destinations for single packet.  Memory owned by sender.
     */
    struct NETIO_AddressSeq *dests;

    /*ci
     * \brief Application level reference pointer to be able to retrieve
     *        content based on the NETIO_Packet.
     */
    void *ref;
};

/*ci
 * \brief Abstract NETIO_Packet type
 */
typedef struct NETIO_Packet NETIO_Packet_T;

/*ci
 * \brief Every packet must have a unique ID. Since RTPS SN does not wrap around,
 *        it is safe to use as a unique ID per data-writer. It also avoids having
 *        to map a between packet id to a RTPS SN.
 */
typedef struct REDA_SequenceNumber NETIO_PacketId_T;

/*ci
 * \def NETIO_Packet_INITIALIZER
 * \brief Constant to initialize a \ref NETIO_Packet
 */
#define NETIO_Packet_INITIALIZER \
{\
    NETIO_Address_INITIALIZER,\
    NETIO_Address_INITIALIZER,\
    NULL, /* buffer */\
    0, /* max_length */\
    0, /* head_pos */\
    0, /* tail_pos */\
    0, /* saved_head_pos */\
    0, /* saved_tail_pos */\
    NETIO_PacketInfo_INITIALIZER,\
    NULL /* dests */,\
    NULL /* ref */\
}

/*ci
 * \brief Adjust the head cursor in the packet payload
 *
 * \details
 *
  * Adjust the head cursor in the packet payload. A positive value moves the
  * cursor forward, a negative value backwards.
  *
  * \param[in] packet Packet to adjust head cursor in
  * \param[in] delta  Adjustment to make
  *
  * \return OEI_TRUE on success, OEI_FALSE on failure
 */
MUST_CHECK_RETURN NETIODllExport OEI_BOOL
NETIO_Packet_set_head(NETIO_Packet_T *packet,OEI_INT32 delta);

/*ci
 * \brief Adjust the tail cursor in the packet payload
 *
 * \details
 *
  * Adjust the tail cursor in the packet payload. A positive value moves the
  * cursor forward, a negative value backwards.
  *
  * \param[in] packet Packet to adjust tail cursor in
  * \param[in] delta  Adjustment to make
  *
  * \return OEI_TRUE on success, OEI_FALSE on failure
 */
MUST_CHECK_RETURN NETIODllExport OEI_BOOL
NETIO_Packet_set_tail(NETIO_Packet_T *packet,OEI_INT32 delta);

/*ci
 * \brief Get a pointer to the current head
 *
 * \details
 *
 * Get a pointer to the current head. Note that it can be assumed that the
 * payload buffer is contiguous.
 *
 * \param[in] packet Packet to get the head pointer for
 *
 * \return pointer to packet head
 *
 * \sa \ref NETIO_Packet_set_head
 */
MUST_CHECK_RETURN NETIODllExport void*
NETIO_Packet_get_head(NETIO_Packet_T *packet);

/*ci
 * \brief Get a pointer to the tail head
 *
 * \details
 *
 * Get a pointer to the current tail. Note that it can be assumed that the
 * payload buffer is contiguous.
 *
 * \param[in] packet Packet to get the tail pointer for
 *
 * \return pointer to packet tail
 *
 * \sa \ref NETIO_Packet_set_tail
 */
MUST_CHECK_RETURN NETIODllExport void*
NETIO_Packet_get_tail(NETIO_Packet_T *packet);

/*ci
 * \brief Initialize a packet with a payload buffer of the given length
 *
 * \details
 *
 * NETIO_Packet only manipulates payloads by reference. The actual buffer
 * holding a payload must be managed outside of the packet structure. A
 * payload buffer is assigned to the packet with the length and an initialized
 * destination sequence.
 *
 * \param[in] packet         Packet structure to initialize
 * \param[in] init_buffer    Payload buffer
 * \param[in] init_length    Initial length in bytes of the buffer
 * \param[in] trailer_length The initial trail position in the payload
 * \param[in] dest_seq       Initialized sequence of destination address for
 *                           the packet
 *
 * \return OEI_TRUE on successful initialization, OEI_FALSE on failure
 */
MUST_CHECK_RETURN NETIODllExport OEI_BOOL
NETIO_Packet_initialize(NETIO_Packet_T *packet,
                       void *init_buffer,
                       OEI_SIZE_T init_length,
                       OEI_SIZE_T trailer_length,
                       struct NETIO_AddressSeq *dest_seq);

#ifndef OEI_CERT
/*ci
 * \brief Finalize a packet structure
 *
 * \details
 *
 * Finalize a packet structure
 *
 * \param[in] packet Packet structure to finalize
 *
 * \return OEI_TRUE on successful initialization, OEI_FALSE on failure
 */
SHOULD_CHECK_RETURN NETIODllExport OEI_BOOL
NETIO_Packet_finalize(NETIO_Packet_T *packet);
#endif /* !OEI_CERT */

/*ci
 * \brief Get pointer to the packet info
 * *
 * \param[in] packet Packet structure to get packet info for
 *
 * \return Pointer to packet info
 */
MUST_CHECK_RETURN NETIODllExport struct NETIO_PacketInfo*
NETIO_Packet_get_info(NETIO_Packet_T *packet);

/*ci
 * \brief Set the source address of a packet
 *
 * \param[in] packet Packet to set source address in
 * \param[in] src    Source address of packet
 */
NETIODllExport void
NETIO_Packet_set_source(NETIO_Packet_T *packet,struct NETIO_Address *src);

/*ci
 * \brief Get the length the packet payload
 *
 * \details
 *
 * Get the length the packet payload
 *
 * \param[in] packet Packet to return the payload length for
 *
 * \return Length of payload
 */
SHOULD_CHECK_RETURN NETIODllExport OEI_SIZE_T
NETIO_Packet_get_payload_length(NETIO_Packet_T *packet);

/*ci
 * \brief Save the current head and tail positions
 *
 * \details
 *
 * Save the current head and tail positions
 *
 * \param[in] packet Packet to save the head and tail positions for
 *
 *\sa \ref NETIO_Packet_restore_positions
 *
 */
NETIODllExport void
NETIO_Packet_save_positions(NETIO_Packet_T *packet);

/*ci
 * \brief Restore the saved head and tail positions
 *
 * \details
 *
 * Restore the saved head and tail positions
 *
 * \param[in] packet Packet to restore the head and tail positions for
 *
 * \sa \ref NETIO_Packet_save_positions
 */
NETIODllExport void
NETIO_Packet_restore_positions(NETIO_Packet_T *packet);

/*ci
 * \brief Save the saved head and tail positions to external variables
 *
 * \details
 *
 * Save the saved head and tail positions to external variables
 *
 * \param[in]  packet Packet to save the head and tail positions for
 * \param[out] head   Contains current head position for packet on return
 * \param[out] tail   Contains current tail position for packet on return
 *
 * \sa \ref NETIO_Packet_restore_positions_from
 */
NETIODllExport void
NETIO_Packet_save_positions_to(NETIO_Packet_T *packet,
                               OEI_SIZE_T *head,
                               OEI_SIZE_T *tail);

/*ci
 * \brief Save the saved head and tail positions to external variables
 *
 * \details
 *
 * Restore the saved head and tail positions from external variables
 *
 * \param[in] packet Packet to restore the head and tail positions for
 * \param[in] head   Set head position for packet
 * \param[in] tail   Set tail position for packet
 *
 * \sa \ref NETIO_Packet_save_positions_to
 */
NETIODllExport void
NETIO_Packet_restore_positions_from(NETIO_Packet_T *packet,
                                    OEI_SIZE_T head,
                                    OEI_SIZE_T tail);

/*ci @} */

/*ci
 * \defgroup NETIO_InterfaceClass NETIO Interface API
 * \ingroup NETIOModule
 */
/*ci \addtogroup NETIO_InterfaceClass
 *   @{
 */

/*ci
 * \brief The required number of bytes to convert an component id and
 *        counter to a table name
 */
#define NETIO_TABLE_NAME_SIZE (16)

/*ci
 *  \brief NETIO base-class property
 */
struct NETIO_InterfaceProperty
{
    struct RT_ComponentProperty _parent;
    OEI_INT32 max_routes;
    OEI_INT32 max_binds;
};


/*ci
 * \brief Constant to initialize \ref NETIO_InterfaceProperty
 */
#define NETIO_InterfaceProperty_INITIALIZER \
{\
    RT_ComponentProperty_INITIALIZER,\
    0,\
    0\
}

/*ci
 * \brief NETIO listener structure
 *
 * \details
 *
 * All NETIO interface are RT Components, and all NETIO layers derives
 * the listener class from the RT_ComponentListener.
 */
struct NETIO_InterfaceListener
{
    /*ci
     * \brief Derived from member
     */
    struct RT_ComponentListener _parent;
};

/*ci
 * \def   NETIO_InterfaceListener_INITIALIZE
 * \brief Constant to initialize \ref NETIO_InterfaceListener
 */
#define NETIO_InterfaceListener_INITIALIZE \
{\
    RT_ComponentListener_INITIALIZER \
}

/*ci
 * \brief Common NETIO Interface factory properties
 *
 * \details
 *
 * All NETIO layers are implemented as RT components. All NETIO layers
 * are created from NETIO factories, and all NETIO factories are RT
 * component factories that share common properties inherited from
 * this structure.
 */
struct NETIO_InterfaceFactoryProperty
{
    /*ci
     * \brief derived from member
     */
    struct RT_ComponentFactoryProperty _parent;
};

/*ci
 * \def NETIO_InterfaceFactoryProperty_INITIALIZER
 * \brief Constant to initialize a \ref NETIO_InterfaceFactoryProperty
 */
#define NETIO_InterfaceFactoryProperty_INITIALIZER \
{\
    RT_ComponentFactoryProperty_INITIALIZER \
}

/*ci
 * \brief Definitions of valid NETIO interface states
 */
typedef enum
{
    /*ci
     * \brief The interface has been created, but will not send/receive
     */
    NETIO_INTERFACESTATE_CREATED,

    /*ci
     * \brief The interface is enabled, can send/receive
     */
    NETIO_INTERFACESTATE_ENABLED
} NETIO_InterfaceState_T;

/*ci
 * \brief Base-class definition for all NETIO interfaces
 *
 * \details
 *
 * All NETIO layers are RT components and are derived from RT components.
 * All NETIO layers also share a common base class, \ref NETIO_Interface
 * and share common properties.
 */
struct NETIO_Interface
{
    /*ci
     * \brief Derived member
     */
    struct RT_Component _parent;

    /*ci
     * \brief A route table with addresses to send to
     */
    DB_Table_T _rtable;

    /*ci
     * \brief A bind table with addresses to listen to
     */
    DB_Table_T _btable;

    /*ci
     * \brief The local address of the NETIO layer. The use is NETIO
     *        specific. It is not guaranteed to be unique outside of the
     *        stack the layer belongs to.
     */
    struct NETIO_Address local_address;

    /*ci
     * \brief The current NETIO interface state
     */
    NETIO_InterfaceState_T state;
};

/*ci
 * \brief Abstract NETIO_Interface type
 */
typedef struct NETIO_Interface NETIO_Interface_T;

/*ci
 * \brief Common bind properties
 */
struct NETIOBindProperty
{
    /*ci
     * \brief The strength of the bind. Strength is a DDS concept and used to
     *        determine ownerships for instances.
     */
    OEI_UINT32 strength;
};

/*ci
 * \def NETIOBindProperty_INITIALIZER
 * \brief Constant to initialize \ref NETIOBindProperty
 */
#define NETIOBindProperty_INITIALIZER \
{\
    0\
}

/*ci
 * \brief Common route properties
 */
struct NETIORouteProperty
{
    /*ci
     * \brief There are no common route properties. This field is present
     *        because some compilers do not allow empty structures.
     */
    OEI_INT32 _dummy;
};

/*ci
 * \def NETIORouteProperty_INITIALIZER
 * \brief Constant to initialize \ref NETIORouteProperty
 */
#define NETIORouteProperty_INITIALIZER \
{\
    0 \
}

/*ci
 * \brief Generic bind key to listen to a source
 */
struct NETIOBindEntryKey
{
    /*ci
     * \brief The source address to listen to
     */
    struct NETIO_Address source;

    /*ci
     * \brief The destination address listening to the source
     */
    struct NETIO_Address destination;
};

/*ci
 * \brief Generic bind entry
 */
struct NETIOBindEntry
{
    /*ci
     * \brief The source address to listen to
     */
    struct NETIO_Address source;

    /*ci
     * \brief The destination address listening to the source
     */
    struct NETIO_Address destination;

    /*ci
     * \brief The interface to use to listen to the source
     */
    NETIO_Interface_T *intf;
};

/*ci
 * \brief The state of a route as determined by the protocol layer
 */
typedef enum
{
    /*ci
     * \brief The route is active, it can send data
     */
    NETIO_ROUTESTATE_ACTIVE,

    /*ci
     * \brief The route is inactive, it cannot send data
     */
    NETIO_ROUTESTATE_INACTIVE
} NETIO_RouteState_T;

/*ci
 * \brief Definition of generic route entry key. Typically all route entries
 *        inherit from this key
 */
struct NETIORouteEntryKey
{
    /*ci
     * \brief The address which an be reached on this route
     */
    struct NETIO_Address destination;

    /*ci
     * \brief The interface which can reach the destination
     */
    NETIO_Interface_T *intf;

    /*ci
     * \brief The address of the interface itself (source)
     */
    struct NETIO_Address intf_address;
};

/*ci
 * \brief Generic route entry, typically all routes inherit from this type
 */
struct NETIORouteEntry
{
   /*ci
    * \brief The destination address which can be reached
    */
    struct NETIO_Address destination;

    /*ci
     * \brief The interface which can reach the address
     */
    NETIO_Interface_T *intf;

    /*ci
     * \brief The address of the interface
     */
    struct NETIO_Address intf_address;

    /*ci
     * \brief State of the route entry
     */
    NETIO_RouteState_T state;
};

/*ci
 * \brief Discriminator for status changes detected at protocol level
 */
typedef enum  
{
    /*ci
     * \brief A NETIO layer has determined that the peer is inactive
     */
    NETIO_EVENTKIND_INACTIVE_PEER,

    /*ci
     * \brief A NETIO layer has determined that the peer is active
     */
    NETIO_EVENTKIND_ACTIVE_PEER
} NETIO_EventKind_T;

/*ci
 * \brief Structure to signal protocol level status changes upstream/downstream
 */
struct NETIO_Event_PeerActivity
{
    /*ci
     * \brief Address of peer layer with the activity change
     */
    struct NETIO_Address peer_addr;

    /*ci
     * \brief Total number of times the peer has become inactive
     */
    OEI_INT32 inactive_total;

    /*ci
     * \brief Change in inactivity since last time signaled
     */
    OEI_INT32 inactive_change;

    /*ci
     * \brief Total number of times the peer has become active
     */
    OEI_INT32 active_total;

    /*ci
     * \brief Change in activity since last time signaled
     */
    OEI_INT32 active_change;
};

/*ci
 * \brief Generic event structure to signal status changes detected at the
 *        protocol level
 */
struct NETIO_Event
{
    /*ci
     * \brief Event discriminator
     */
    NETIO_EventKind_T kind;

    /*ci
     * \brief Additional event information
     */
    union
    {
        struct NETIO_Event_PeerActivity peer_activity;
    } value;
};

struct NETIO_InterfaceI;

/*ci
 * \brief Create a table name with a consistent format for NETIO route
 *        and bind tables
 *
 * \param[out] tbl_name The resulting table name is placed here. It is assumed
 *                      that the buffer is long enough and is should be at least
 *                      NETIO_TABLE_NAME_SIZE bytes large.
 * \param[in]  id       The name of the factory owning the table
 * \param[in]  suffix   Single character identifying the type of table
 * \param[in]  instance The instance the table is created from
 *
 */
NETIODllExport void
NETIO_Interface_Table_name_from_id(char *tbl_name,
                                  union RT_ComponentFactoryId *id,
                                  char suffix,
                                  OEI_INT32 instance);

/*ci
 * \brief Initialize a NETIO_Interface base-class
 *
 * \param[in] netio      The netio interface to initialize
 * \param[in] netio_intf Pointer to the NETIO interface implementation
 * \param[in] property   The NETIO properties
 * \param[in] listener   The NETIO listener
 *
 * \return This function always returns OEI_TRUE
 *
 * \sa \ref NETIO_Interface_finalize
 */
MUST_CHECK_RETURN NETIODllExport OEI_BOOL
NETIO_Interface_initialize(struct NETIO_Interface *netio,
                          struct NETIO_InterfaceI *netio_intf,
                          const struct NETIO_InterfaceProperty *const property,
                          const struct NETIO_InterfaceListener *const listener);

#ifndef OEI_CERT
/*ci
 * \brief Finalize a NETIO_Interface base-class
 *
 * \param[in] netio The netio interface to finalize
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa \ref NETIO_Interface_initialize
 */
SHOULD_CHECK_RETURN NETIODllExport OEI_BOOL
NETIO_Interface_finalize(struct NETIO_Interface *netio);
#endif /* !OEI_CERT */

/*ci
 * \brief Implementation of required database function to compare
 *        NETIO route records stored in a database
 *
 * \details
 *
 * This function is installed as the compare function for the
 * tables holding NETIO_Route records
 *
 * \param[in] flags Passed in from the database
 * \param[in] op1   Existing database record
 * \param[in] op2   New database record or key depending on flags
 *
 * \return positive integer if op1 is greater than op2,
 *         negative integer if op1 is less than op2,
 *         zero if op1 is equal to op2
 */
MUST_CHECK_RETURN NETIODllExport OEI_INT32
NETIO_Interface_compare_route(OEI_INT32 flags,
                              const DB_Record_T op1, void *op2);

/*ci
 * \brief Implementation of required database function to compare
 *        NETIO route records stored in a database
 *
 * \details
 *
 * This function is installed as the compare function for the
 * tables holding NETIO_Bind records
 *
 * \param[in] flags Passed in from the database
 * \param[in] op1   Existing database record
 * \param[in] op2   New database record or key depending on flags
 *
 * \return positive integer if op1 is greater than op2,
 *         negative integer if op1 is less than op2,
 *         zero if op1 is equal to op2
 */
MUST_CHECK_RETURN NETIODllExport OEI_INT32
NETIO_Interface_compare_bind(OEI_INT32 flags,
                             const DB_Record_T op1, void *op2);

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI send method
 *
 * \param[in] self        NETIO interface to send from
 * \param[in] source      The source interface for the packet
 * \param[in] destination The destination address for the packet
 * \param[in] packet      The packet to send
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa \ref NETIO_Interface_send, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_sendFunc)(NETIO_Interface_T *self,
                            struct NETIO_Interface *source,
                            struct NETIO_Address *destination,
                            NETIO_Packet_T *packet)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI receive method
 *
 * \param[in] netio_intf NETIO interface to receive in
 * \param[in] src_addr   The source address of the packet
 * \param[in] dst_addr   The destination address for the packet
 * \param[in] packet     The forwarded packet from downstream
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa \ref NETIO_Interface_receive, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_receiveFunc)(NETIO_Interface_T *netio_intf,
                              struct NETIO_Address *src_addr,
                              struct NETIO_Address *dst_addr,
                              NETIO_Packet_T *packet)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI acknack method
 *
 * \param[in] self      The NETIO interface receiving the acknack
 * \param[in] source    The peer source address of the acknack
 * \param[in] packet_id The packet_id/SN of the NETIO_Packet being acked/nacked
 * \param[in] nack      OEI_TRUE if this is a negative acknowledgment
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa \ref NETIO_Interface_acknack, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_acknackFunc)(NETIO_Interface_T *self,
                               struct NETIO_Address *source,
                               NETIO_PacketId_T *packet_id,
                               OEI_BOOL nack)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI request method
 *
 * \param[in] self             The NETIO interface receiving the request
 * \param[in] source           The peer source address of the request
 * \param[in] dest             The peer destination address of the request
 * \param[out] packet          The requested packet if it existed, NULL
 *                             otherwise
 * \param[in] packet_id        The packet_id/SN of the NETIO_Packet being
 *                             requested
 * \param[in] actual_packet_id The if packet_id is not available, the next
 *                             available SN/packet_id
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa \ref NETIO_Interface_return_loan, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_requestFunc)(NETIO_Interface_T *self,
                               struct NETIO_Address *source,
                               struct NETIO_Address *dest,
                               NETIO_Packet_T **packet,
                               NETIO_PacketId_T *packet_id,
                               NETIO_PacketId_T *actual_packet_id)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI return_loan method
 *
 * \param[in] self       The NETIO interface receiving the request
 * \param[in] source     The peer source address of the loan
 * \param[in] packet     The packet being returned
 * \param[in] packet_id  The packet_id/SN of the NETIO_Packet being returned
 *                       requested
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa \ref NETIO_Interface_request, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_return_loanFunc)(NETIO_Interface_T *self,
                                   struct NETIO_Address *source,
                                   NETIO_Packet_T *packet,
                                   NETIO_PacketId_T *packet_id)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI xmit_remove method
 *
 * \param[in] self        NETIO interface to cancel a transmit on
 * \param[in] destination The destination address of the packet
 * \param[in] packet_id   The packet_id/SN of the NETIO_Packet to cancel
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa \ref NETIO_Interface_xmit_remove, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_xmit_removeFunc)(NETIO_Interface_T *self,
                                   struct NETIO_Address *destination,
                                   NETIO_PacketId_T *packet_id)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI add_route method
 *
 * \param[in] self      NETIO interface to add the route to
 * \param[in] dst_addr  The destination address for the route
 * \param[in] via_intf  The downstream interface
 * \param[in] via_addr  The address to pass to the downstream interface
 * \param[in] property  The route property
 * \param[in] existed   Whether the route already existed
 *
 * \sa \ref NETIO_Interface_add_route, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_add_routeFunc)(NETIO_Interface_T *self,
                                 struct NETIO_Address *dst_addr,
                                 NETIO_Interface_T *via_intf,
                                 struct NETIO_Address *via_addr,
                                 struct NETIORouteProperty *property,
                                 OEI_BOOL *existed)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI delete_route method
 *
 * \param[in]  self      NETIO interface to delete the route from
 * \param[in]  dst_addr  The destination address for the route
 * \param[in]  via_intf  The downstream interface
 * \param[in]  via_addr  The address to pass to the downstream interface
 * \param[out] existed   Whether the route existed
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure. Note that is it not
 *         considered a failure if the interface didn't exist.
 *
 * \sa \ref NETIO_Interface_delete_route, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_delete_routeFunc)(NETIO_Interface_T *self,
                                    struct NETIO_Address *dst_addr,
                                    NETIO_Interface_T *via_intf,
                                    struct NETIO_Address *via_addr,
                                    OEI_BOOL *existed)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI bind method
 *
 * \param[in]  netio_intf NETIO interface to bind
 * \param[in]  src_addr   The address to bind to
 * \param[in]  property   The property to use for the bind
 * \param[out] existed    Whether a previous bind existed or not
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 *
 * \sa \ref NETIO_Interface_bind, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_bindFunc)(NETIO_Interface_T *netio_intf,
                           struct NETIO_Address *src_addr,
                           struct NETIOBindProperty *property,
                           OEI_BOOL *existed)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI unbind method
 *
 * \param[in]  netio_intf NETIO interface to unbind
 * \param[in]  src_addr   The address to unbind from
 * \param[in]  dst_intf   Interface to unbind from
 * \param[out] existed    Whether a bind existed or not
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 *
 * \sa \ref NETIO_Interface_unbind, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_unbindFunc)(NETIO_Interface_T *netio_intf,
                             struct NETIO_Address *src_addr,
                             NETIO_Interface_T *dst_intf,
                             OEI_BOOL *existed)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI bind_external method
 *
 * \param[in]  src_intf   NETIO interface to bind to upstream interface
 * \param[in]  src_addr   The address to bind to
 * \param[in]  dst_intf   The upstream interface
 * \param[in]  dst_addr   The address to pass to the upstream interface
 * \param[in]  property   The properties for the bind
 * \param[out] existed    Whether a previous bind already existed for this entry
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 *
 * \sa \ref NETIO_Interface_bind_external, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_bind_externalFunc)(NETIO_Interface_T *src_intf,
                                    struct NETIO_Address *src_addr,
                                    NETIO_Interface_T *dst_intf,
                                    struct NETIO_Address *dst_addr,
                                    struct NETIOBindProperty *property,
                                    OEI_BOOL *existed)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI unbind_external method
 *
 * \param[in]  src_intf   NETIO interface to unbind from upstream interface
 * \param[in]  src_addr   The address to unbind from
 * \param[in]  dst_intf   The upstream interface
 * \param[in]  dst_addr   The address to passed to the upstream interface
 * \param[out] existed    Whether a previous bind already existed for this entry
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 *
 * \sa \ref NETIO_Interface_unbind_external, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_unbind_externalFunc)(NETIO_Interface_T *src_intf,
                                      struct NETIO_Address *src_addr,
                                      NETIO_Interface_T *dst_intf,
                                      struct NETIO_Address *dst_addr,
                                      OEI_BOOL *existed)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI get_external_interface method
 *
 * \param[in]   netio_intf   NETIO interface to get the external interface for
 * \param[in]   src_addr     The address to send to
 * \param[out]  dst_intf     The interface to use when sending to the address
 * \param[out]  dst_addr     The destination address to use when forwarding
 *                           packets to the destination interface
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 *
 * \sa \ref NETIO_Interface_get_external_interface, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_get_external_interfaceFunc)(NETIO_Interface_T *netio_intf,
                                             struct NETIO_Address *src_addr,
                                             NETIO_Interface_T **dst_intf,
                                             struct NETIO_Address *dst_addr)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI reserve_address method
 *
 * \param[in]    self          NETIO interface to reserve addresses on
 * \param[in]    req_address   List of requested addresses
 * \param[inout] rsvd_address  The downstream interface
 * \param[in]    property      Properties to use to listen on the reserved addresses
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 *
 * \sa \ref NETIO_Interface_reserve_address, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_reserve_addressFunc)(NETIO_Interface_T *self,
                                       struct NETIO_AddressSeq *req_address,
                                       struct NETIO_AddressSeq *rsvd_address,
                                       struct NETIOBindProperty *property)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI release_address method
 *
 * \param[in] self     NETIO interface to release addresses on
 * \param[in] src_addr Address to release
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 *
 * \sa \ref NETIO_Interface_release_address, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_release_addressFunc)(NETIO_Interface_T *self,
                                    struct NETIO_Address *src_addr)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI set_state method
 *
 * \param[in] netio_intf NETIO interface to set state on
 * \param[in] state      New state
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 *
 * \sa \ref NETIO_Interface_set_state, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_set_stateFunc)(NETIO_Interface_T *netio_intf,
                                NETIO_InterfaceState_T state)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI resolve_adddress method
 *
 * \param[out] netio_intf     Interface
 * \param[in]  address_string Address to convert
 * \param[out] address_value  Converted address on success
 * \param[out] is_invalid     Whether the address is valid or not
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 *
 * \sa \ref NETIO_Interface_resolve_adddress, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_resolve_adddressFunc)(NETIO_Interface_T *netio_intf,
                                        const char *address_string,
                                        struct NETIO_Address *address_value,
                                        OEI_BOOL *invalid)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI get_route_table method
 *
 * \param[in]    netio_intf The NETIO interface
 * \param[inout] address    Sequence of NETIO addresses this interface understands
 * \param[inout] netmask    Sequence of the corresponding netmasks
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 *
 * \sa \ref NETIO_Interface_get_route_table, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_get_route_tableFunc)(NETIO_Interface_T *netio_intf,
                                        struct NETIO_AddressSeq *address,
                                        struct NETIO_NetmaskSeq *netmask)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI post_event method
 *
 * \param[in] self     The datawriter interface the event occurred on
 * \param[in] src_intf The source of the event
 * \param[in] event    The NETIO event
 *
 * \return OEI_TRUE if the event was handled successfully, OEI_FALSE if not
 *
 * \sa \ref NETIO_Interface_post_event, \ref NETIO_InterfaceI
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_post_eventFunc)(NETIO_Interface_T *self,
                                  NETIO_Interface_T *src_intf,
                                  struct NETIO_Event *event)
)

/*ci
 * \brief Definition of the \ref NETIO_InterfaceI lookup_route method
 *
 * \details
 *
 * Check if a NETIO interface has a route to a specific destination
 *
 * \param[in]  netio_intf   NETIO interface to get the external interface for
 * \param[in]  dst_reader   The address to send to
 * \param[in]  via_intf     The interface to use
 * \param[in]  via_address  The interface address to use
 * \param[out] route_exists OEI_TRUE if the route existed, OEI_FALSE if not
 *
 * \return OEI_TRUE if the event was handled successfully, OEI_FALSE if not
 */
FUNCTION_MUST_TYPEDEF(
OEI_BOOL
(*NETIO_Interface_lookup_routeFunc)(struct NETIO_Interface *netio_intf,
                                    struct NETIO_Address *dst_reader,
                                    struct NETIO_Interface *via_intf,
                                    struct NETIO_Address *via_address,
                                    OEI_BOOL *route_exists)
)

/*ci
 * \brief NETIO_InterfaceI interface required to be implemented by all NETIO
 *        layers.
 */
struct NETIO_InterfaceI
{
    /*ci
     * \brief Base-class interface
     */
    struct RT_ComponentI _parent;

    /*ci
     * \brief Method to send packets to one or more destinations
     */
    NETIO_Interface_sendFunc send;

    /*ci
     * \brief Method to ACK/NACK a packet upstream
     */
    NETIO_Interface_acknackFunc acknack;

    /*ci
     * \brief Method to request packet upstream
     */
    NETIO_Interface_requestFunc request;

    NETIO_Interface_return_loanFunc return_loan;

    /*ci
     * \brief Method to cancel a transmission downstream
     */
    NETIO_Interface_xmit_removeFunc xmit_remove;

    /*ci
     * \brief Method to add a destination route to an interface
     */
    NETIO_Interface_add_routeFunc add_route;

    /*ci
     * \brief Method to remove a route to a destination from an interface
     */
    NETIO_Interface_delete_routeFunc delete_route;

    /*ci
     * \brief Method to reserve addresses to listen to on an interface
     */
    NETIO_Interface_reserve_addressFunc reserve_address;

    /*ci
     * \brief Method to start listening for data from a peer interface
     */
    NETIO_Interface_bindFunc bind;

    /*ci
     * \brief Method to stop listening for data from a peer interface
     */
    NETIO_Interface_unbindFunc unbind;

    /*ci
     * \brief Method to forward a packet to an interface upstream
     */
    NETIO_Interface_receiveFunc receive;

    /*ci
     * \brief Method to get the interface to forward packets to from an
     *        downstream interface
     */
    NETIO_Interface_get_external_interfaceFunc get_external_interface;

    /*ci
     * \brief Method to add a path from an interface to an upstream interface
     */
    NETIO_Interface_bind_externalFunc bind_external;

    /*ci
     * \brief Method to remove a path from an interface to an upstream interface
     */
    NETIO_Interface_unbind_externalFunc unbind_external;

    /*ci
     * \brief Method to set the state of an interface
     */
    NETIO_Interface_set_stateFunc set_state;

    /*ci
     * \brief Method to release addresses to listen to on an interface
     */
    NETIO_Interface_release_addressFunc release_address;

    /*ci
     * \brief Method to query an interface if it can resolve an address or not
     */
    NETIO_Interface_resolve_adddressFunc resolve_address;

    /*ci
     * \brief Method to query an interface for what addresses it can forward
     *        packets to
     */
    NETIO_Interface_get_route_tableFunc get_route_table;

    /*ci
     * \brief Method to post external events to an interface
     */
    NETIO_Interface_post_eventFunc post_event;

    /*ci
     * \brief Method to lookup if a route exists to a destination
     */
    NETIO_Interface_lookup_routeFunc lookup_route;
};

/*ci
 * \brief Wrapper to call of the NETIO_InterfaceFactory->create_component
 *
 * \param[in] f_  The NETIO interface factory
 * \param[in] p_  The property
 * \param[in] l_  The listener
 */
#define NETIO_InterfaceFactory_create_component(f_,p_,l_) \
        (NETIO_Interface_T*)((f_)->intf->create_component(\
                (f_),p_,l_))

/*ci
 * \brief Wrapper to call of the NETIO_InterfaceFactory->delete_component
 *
 * \param[in] f_    The NETIO interface factory
 * \param[in] intf_ The NETIO interface to delete
 */
#define NETIO_InterfaceFactory_delete_component(f_,intf_) \
        (f_)->intf->delete_component(f_,(&(intf_)->_parent))

/*ci
 * \brief Wrapper to call the \ref NETIO_InterfaceI::send
 *
 * \details
 * Forward a packet from a source interface to a destination
 *
 * \param[in] self_      NETIO interface to send from
 * \param[in] src_intf_  The source interface for the packet
 * \param[in] dst_addr_  The destination address for the packet
 * \param[in] pkt_       The packet to send
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 */
#define NETIO_Interface_send(self_,src_intf_,dst_addr_,pkt_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->send(self_,src_intf_,dst_addr_,pkt_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::acknack
 *
 * \details
 * An acknack is processed for a peer. If all peers who should receive this
 * sample has acked it, then it can be ack'ed in the queue and is up for removal
 * A peer (source parameter) must acknack a sample at most once.
 *
 * \param[in] self_     The NETIO interface receiving the acknack
 * \param[in] src_intf_ The peer source address of the acknack
 * \param[in] sn_       The packet_id/SN of the NETIO_Packet being acked/nacked
 * \param[in] nack_     OEI_TRUE if this is a negative acknowledgment
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 */
#define NETIO_Interface_acknack(self_,src_intf_,sn_,nack_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->acknack(self_,src_intf_,sn_,nack_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::request
 *
 * \details
 * Request a SN from source interface and destination address.
 * if SN  is not available nextpktid_ contains the next available
 * SN.
 *
 * \param[in] self_      The NETIO interface receiving the request
 * \param[in] src_       The peer source address of the request
 * \param[in] dst_       The peer destination address of the request
 * \param[in] pkt_       The requested packet if it existed, NULL
 *                       otherwise
 * \param[in] pktid_     The packet_id/SN of the NETIO_Packet being
 *                       requested
 * \param[in] nextpktid_ The if packet_id is not available, the next
 *                       available SN/packet_id
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 */
#define NETIO_Interface_request(self_,src_,dst_,pkt_,pktid_,nextpktid_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->request(self_,src_,dst_,pkt_,pktid_,nextpktid_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::return_loan method
 *
 * \param[in] self_    The NETIO interface receiving the request
 * \param[in] src_     The peer source address of the loan
 * \param[in] pkt_     The packet being returned
 * \param[in] pkt_id_  The packet_id/SN of the NETIO_Packet being returned
 *                     requested
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa \ref NETIO_Interface_request, \ref NETIO_InterfaceI
 */
#define NETIO_Interface_return_loan(self_,src_,pkt_,pkt_id_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->return_loan(self_,src_,pkt_,pkt_id_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::xmit_remove
 *
 * \details
 * Inform all downstream interfaces all attempts at delivering a packet
 * should be cancelled.
 *
 * \param[in] self_   NETIO interface to cancel a transmit on
 * \param[in] dst_    The destination address of the packet
 * \param[in] pkt_id_ The packet_id/SN of the NETIO_Packet to cancel
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 */
#define NETIO_Interface_xmit_remove(self_,dst_,pkt_id_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->xmit_remove(self_,dst_,pkt_id_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::add_route
 *
 * \details
 * Add route to a matching peer to be reached via a downstream interface
 * using the specified downstream address.
 *
 * \param[in] self_      NETIO interface to add the route to
 * \param[in] dst_       The destination address for the route
 * \param[in] intf_      The downstream interface
 * \param[in] intf_addr_ The address to pass to the downstream interface
 * \param[in] prop_      The route property
 * \param[in] ex_        Whether the route already existed
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 */
#define NETIO_Interface_add_route(self_,dst_,intf_,intf_addr,prop_,ex_) \
    ((struct NETIO_InterfaceI*)(\
     (self_)->_parent._intf))->add_route(self_,dst_,intf_,intf_addr,prop_,ex_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::delete_route
 *
 * \details
 * Remove a route to a peer, either because it no longer matches or that the
 * peer has been deleted.
 *
 * \param[in]  self_      NETIO interface to add the route to
 * \param[in]  dst_       The destination address for the route
 * \param[in]  intf_      The downstream interface
 * \param[in]  intf_addr_ The address to pass to the downstream interface
 * \param[out] exist_     Whether the route existed
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure. Note that is it not
 *         considered a failure if the interface didn't exist.
 */
#define NETIO_Interface_delete_route(self_,dst_,intf_,intf_addr_,exist_) \
    ((struct NETIO_InterfaceI*)(\
     (self_)->_parent._intf))->delete_route(self_,dst_,intf_,intf_addr_,exist_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::reserve_address
 *
 * \details
 *  Reserve addresses on an interface to listen to using the requested addresses
 *  as the starting point. The resulting addresses may be fewer than requested,
 *  even 0 is legal.
 *
 * \param[in]    self_     NETIO interface to reserve addresses on
 * \param[in]    src_addr_ List of requested addresses
 * \param[inout] pub_addr_ List of addresses that are reserved
 * \param[in]    prop_     Properties to use to listen on the reserved addresses
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 */
#define NETIO_Interface_reserve_address(self_,src_addr_,pub_addr_,prop_)\
    ((struct NETIO_InterfaceI*)(\
    (self_)->_parent._intf))->reserve_address(self_,src_addr_,pub_addr_,prop_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::release_address
 *
 * Release an address previously reserved. The address will no longer be
 * listened on by the caller.
 *
 * \param[in] self_     NETIO interface to release addresses on
 * \param[in] src_addr_ Address to release
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 */
#define NETIO_Interface_release_address(self_,src_addr_) \
                    ((struct NETIO_InterfaceI*)(\
                    (self_)->_parent._intf))->release_address(self_,src_addr_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::bind
 *
 * \details
 * When an interface is matched with another interface (how is outside the
 * scope of this function) an entry in the bind-table for the interface is
 * created. The peer-to-peer state between the two interfaces should be
 * maintained based on this relationship until an unbind is performed.
 *
 * \param[in]  self_ NETIO interface to bind
 * \param[in]  src_  The address to bind to
 * \param[in]  prop_ The property to use for the bind
 * \param[out] ex_   Whether a previous bind existed or not
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 */
#define NETIO_Interface_bind(self_,src_,prop_,ex_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->bind(self_,src_,prop_,ex_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::unbind
 *
 * \details
 * When an interface is unmatched from another interface (how is outside the
 * scope of this function) an entry in the bind-table for the interface can
 * safely be removed (although not required). No state is expected to be
 * maintained from this point on
 *
 * \param[in]  self_  NETIO interface to unbind
 * \param[in]  src_   The address to unbind from
 * \param[in]  dst_   Interface to unbind from
 * \param[out] exist_ Whether a bind existed or not
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 */
#define NETIO_Interface_unbind(self_,src_,dst_,exist_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->unbind(self_,src_,dst_,exist_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::bind_external
 *
 * \details
 * When an upstream interface wants to listen to data from a local
 * downstream interface it binds to the downstream interface using the
 * external bind function. This typically means adding an interface to a
 * bind table in the downstream interface so the downstream interface is
 * able to forward data upstream.
 *
 * \param[in]  src_intf_ NETIO interface to bind to upstream interface
 * \param[in]  src_adr_  The address to bind to
 * \param[in]  dst_intf_ The upstream interface
 * \param[in]  dst_adr_  The address to pass to the upstream interface
 * \param[in]  p_        The properties for the bind
 * \param[out] e_        Whether a previous bind already existed for this entry
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 */
#define NETIO_Interface_bind_external(src_intf_,src_adr_,dst_intf_,dst_adr_,p_,e_) \
((struct NETIO_InterfaceI*)(\
 (src_intf_)->_parent._intf))->bind_external(src_intf_,src_adr_,dst_intf_,dst_adr_,p_,e_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::unbind_external
 *
 * \details
 * When an upstream interface no longer wants to listen to data from a local
 * downstream interface it unbinds from the downstream interface using the
 * external unbind function. This typically means removing an interface from a
 * bind table in the downstream interface so the downstream interface no longer
 * forwards data upstream to a non-existent interface.
 *
 * \param[in]  src_intf_ NETIO interface to unbind from upstream interface
 * \param[in]  src_adr_  The address to unbind from
 * \param[in]  dst_adr_  The upstream interface
 * \param[in]  dst_intf_ The address to passed to the upstream interface
 * \param[out] exist_    Whether a previous bind already existed for this entry
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 */
#define NETIO_Interface_unbind_external(src_intf_,src_adr_,dst_adr_,dst_intf_,exist_) \
((struct NETIO_InterfaceI*)(\
 (src_intf_)->_parent._intf))->unbind_external(src_intf_,src_adr_,dst_adr_,dst_intf_,exist_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::receive
 *
 * \details
 *
 *  A NETIO interface receives data from a downstream interface.
 *  The data can be in different protocol formats. Depending on the
 *  downstream protocol different actions may be taken, but semantically it
 *  does not matter where data is coming from. The receive interface is
 *  synchronous, _all_ processing takes place in the context of the downstream
 *  receive context, such as a thread.
 *
 * \param[in] self_ The interface receiving data
 * \param[in] src_  The source address of the data
 * \param[in] dst_  The destination address of the data
 * \param[in] p_    A NETIO_Packet with the payload
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 */
#define NETIO_Interface_receive(self_,src_,dst_,p_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->receive(self_,src_,dst_,p_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::get_external_interface
 *
 * \details
 * When an interface is bound to a downstream interface it must provide
 * which interface and address the downstream interface should use when
 * forwarding a NETIO_Packet.
 *
 * \param[in]   self_   NETIO interface to get the external interface for
 * \param[in]   src_    The address to send to
 * \param[out]  ul_     The interface to use
 * \param[out]  dst_    The destination address to use
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 */
#define NETIO_Interface_get_external_interface(self_,src_,ul_,dst_) \
    ((struct NETIO_InterfaceI*)(\
       (self_)->_parent._intf))->get_external_interface(self_,src_,ul_,dst_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::set_state
 *
 * \details
 * Set the state of an interface, the interface is not required to perform
 * any action as this is interface dependent.
 *
 * \param[in] self_  NETIO interface to set state on
 * \param[in] state_ New state
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 */
#define NETIO_Interface_set_state(self_,state_) \
    ((struct NETIO_InterfaceI*)(\
       (self_)->_parent._intf))->set_state(self_,state_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::resolve_address
 *
 * \details
 * Request an interface to convert a string address to a \ref NETIO_Address.
 * If an interface successfully translated it, is_invalid_ is set to OEI_TRUE,
 * otherwise false and the content of the output is undefined.
 *
 * \param[out] self_       Interface requested to do the conversion
 * \param[in]  address_    Address to convert
 * \param[out] resolved_   Converted address on success
 * \param[out] is_invalid_ Whether the address is valid or not
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 */
#define NETIO_Interface_resolve_address(self_,address_,resolved_,is_invalid_) \
((struct NETIO_InterfaceI*)(\
   (self_)->_parent._intf))->resolve_address(self_,address_,resolved_,is_invalid_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::get_route_table
 *
 * \details
 * Query an interface for what addresses it can forward packets to. Not
 * all interfaces provides a routing table, this is implementation dependent.
 *
 * \param[in]    self_    The NETIO interface
 * \param[inout] address_ Sequence of NETIO addresses this interface understands
 * \param[inout] netmask_ Sequence of the corresponding netmasks
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure.
 */
#define NETIO_Interface_get_route_table(self_,address_,netmask_) \
    ((struct NETIO_InterfaceI*)(\
       (self_)->_parent._intf))->get_route_table(self_,address_,netmask_)

/*ci
 * \brief Wrapper to call \ref NETIO_InterfaceI::post_event
 *
 * \details
 * Post an external event on the interface
 *
 * \param[in] self_     The interface interface the event occurred on
 * \param[in] src_intf_ The source of the event
 * \param[in] event_    The NETIO event
 *
 * \return OEI_TRUE if the event was handled successfully, OEI_FALSE if not
 */
#define NETIO_Interface_post_event(self_,src_intf_,event_) \
    ((struct NETIO_InterfaceI*)(\
       (self_)->_parent._intf))->post_event(self_,src_intf_,event_)

/*ci
 * \brief Lookup if a route exists to a destination via an interface
 *
 * \param[in]  self_         The source interface
 * \param[in]  dst_reader_   The destination address
 * \param[in]  via_intf_     The downstream interface
 * \param[in]  via_address_  The address to pass to the downstream interface
 * \param[out] route_exists_ OEI_TRUE if the route existed, OEI_FALSE if not
 *
 * \return OEI_TRUE if the event was handled successfully, OEI_FALSE if not
 */
#define NETIO_Interface_lookup_route(self_,dst_reader_,via_intf_,\
                                     via_address_,route_exists_)  \
 ((struct NETIO_InterfaceI*)(\
   (self_)->_parent._intf))->lookup_route(self_,dst_reader_,via_intf_,\
           via_address_,route_exists_)

#ifdef __cplusplus
}
#endif

#endif /* netio_interface_h */

/*ci @} */
