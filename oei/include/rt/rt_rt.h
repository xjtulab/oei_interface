/*
 * FILE: rt_log.h - RT API
 *
 */

/*ci
 * \file 
 * \defgroup RT_RunTimeClass Run-Time 
 * \ingroup OEInternalModule 
 * \brief RT Run-Time API
 */
/*ci \addtogroup OEInternalModule
 * @{
 */
#ifndef rt_rt_h
#define rt_rt_h

#ifndef rt_dll_h
#include "rt/rt_dll.h"
#endif

#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifndef osapi_timer_h
#include "osapi/osapi_timer.h"
#endif

#ifndef reda_circularlist_h
#include "reda/reda_circularlist.h"
#endif

#ifndef db_api_h
#include "db/db_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

/*******************************************************************************
 *                               RTConstants
 ******************************************************************************/

/*ci
 * \brief Create an interface id from a class and and instance
 *
 * \param[in] class_    The interface class
 * \param[in] instance_ The instance of the interface class
 *
 * \return The interface id
 */
#define RT_MKINTERFACEID(class_,instance_) \
                       ((((class_)<<16)&0xffff0000) | (instance_))

/*ci
 * \brief Get the interface class from an interface class id
 *
 * \param[in] id_ The interface id
 *
 * \return interface class
 */
#define RT_INTERFACE_CLASS(id_)     ((id_) >> 16)

/*ci
 * \brief Get the interface class instance from an interface class id
 *
 * \param[in] id_ The interface id
 *
 * \return interface class instance
 */
#define RT_INTERFACE_INSTANCE(id_)  ((id_) & 0xffff)

/* INTERFACE CLASSES */
/*ci
 * \brief The unknown interface class
 */
#define RT_COMPONENT_CLASS_UNKNOWN              (0x0000)

/*ci
 *\brief The discovery interface class
 */
#define RT_COMPONENT_CLASS_DISCOVERY            (0x0001)

/*ci
 *\brief The writer history interface class
 */
#define RT_COMPONENT_CLASS_WHISTORY             (0x0002)

/*ci
 *\brief The writer history interface class
 */
#define RT_COMPONENT_CLASS_RHISTORY             (0x0003)

/*ci
 *\brief The NETIO interface class
 */
#define RT_COMPONENT_CLASS_NETIO                (0x0004)

/* DDS Discovery class components */

/*ci
 * \brief The DPDE discovery class instance
 */
#define RT_COMPONENT_INSTANCE_DISCOVERY_DPDE   (1)

/*ci
 * \brief The DPSE discovery class instance
 */
#define RT_COMPONENT_INSTANCE_DISCOVERY_DPSE   (2)

/*ci
 * \brief DDS WH_SM WriterHistory class instance
 */
#define RT_COMPONENT_INSTANCE_WHISTORY_SM      (1)

/*ci
 * \brief DDS RH_SM WriterHistory class instance
 */
#define RT_COMPONENT_INSTANCE_RHISTORY_SM      (1)

/*ci
 * \brief UDP NETIO class instance
 */
#define RT_COMPONENT_INSTANCE_UDP              (1)

/*ci
 * \brief RTPS NETIO class instance
 */
#define RT_COMPONENT_INSTANCE_RTPS             (2)

/*ci
 * \brief INTRA NETIO class instance
 */
#define RT_COMPONENT_INSTANCE_INTRA            (3)

/*ci
 * \brief DDS WriterInterface NETIO class instance
 */
#define RT_COMPONENT_INSTANCE_DDSWI            (4)

/*ci
 * \brief DDS Readernterface NETIO class instance
 */
#define RT_COMPONENT_INSTANCE_DDSRI            (5)

/*ci The number of tables created by RT cannot exceed this number. If more tables
 * are required, increase this number.
 */
#define RT_RESOURCE_TABLES_PER_INSTANCE     (1)

/*ci
 * \brief Type used to identify components
 */
typedef OEI_UINT32 RT_ComponentInterfaceId_t;

/*******************************************************************************
 *                               RT_Component
 ******************************************************************************/
struct RT_Component;

/*ci
 * \brief Abstract RT_Component type
 */
typedef struct RT_Component RT_Component_T;

/*ci
 * \brief Notify listeners on changes in component state.
 *
 * \details
 *
 * The RT_ComponentListener structure is for future extensions.
 */
struct RT_ComponentListener
{
    /*ci
     * \brief data passed in listeners
     */
    void *data;
};

/*ci
 * \brief Initializer for RT_ComponentListener
 */
#define RT_ComponentListener_INITIALIZER \
{ \
    NULL \
}

/*ci
 * \brief Properties a component can be created with
 */
struct RT_ComponentProperty
{
    /*ci
     * \brief A database to create tables in if needed
     */
    DB_Database_T db;

    /*ci
     * \brief A timer to create timeouts from if needed
     */
    OSAPI_Timer_T timer;
};

#define RT_ComponentProperty_INITIALIZER \
{\
   NULL,\
   NULL \
}

/*ci
 * \brief The component base interface
 */
struct RT_ComponentI
{
    /*ci
     * \brief Some compilers do not allow empty structures. There are
     *        currently no base interface functions required
     */
    OEI_INT32 dummy;
};

/*ci
 * \brief The component base class
 */
struct RT_Component
{
    /*ci
     *\brief Pointer to the component interface
     */
    struct RT_ComponentI *_intf;

    /*ci
     *\brief The component id
     */
    OEI_UINT32 id;
};

/*ci
 *
 * \brief Initialize a RT_Component base-class
 *
 * \details
 *
 * All components must initialized the RT_Component base-class by
 * calling this function
 *
 * \param[in] c         The component to initialize
 * \param[in] intf      Pointer to implementation of the RT_ComponentI interface
 * \param[in] id        Unique ID for the component
 * \param[in] property  Properties for the component
 * \param[in] listener  Listeners to call when a component changes
 *
 * \sa \ref RT_Component_finalize
 */
RTDllExport void
RT_Component_initialize(RT_Component_T *c,
                        struct RT_ComponentI *intf,
                        OEI_UINT32 id,
                        const struct RT_ComponentProperty *const property,
                        const struct RT_ComponentListener *const listener);

#ifndef OEI_CERT
/*ci
 *
 * \brief Finalize a RT_Component base-class
 *
 * \details
 *
 * All components must be finalized by calling this function
 *
 * \param[in] c The component to finalize
 *
 * \sa \ref RT_Component_initialize
 */
RTDllExport void
RT_Component_finalize(RT_Component_T *c);
#endif /* !OEI_CERT */

#define RT_COMPONENTI_BASE {0}

/*******************************************************************************
 *                               RT_ComponentFactoryI
 ******************************************************************************/
struct RT_ComponentFactory;

/*ci
 * \brief Properties a RT_ComponentFactory can be created with
 */
struct RT_ComponentFactoryProperty
{
    /* The "base class" cannot be empty due to some compilers failing,
     * and there are currently no common factory properties.
     */
    void *dummy;
};

/*ci
 * \brief Initializer for \ref RT_ComponentFactoryProperty
 */
#define RT_ComponentFactoryProperty_INITIALIZER \
{ \
  NULL\
}

/*ci
 * \brief Listeners that can be installed on a RT_ComponentFactoryProperty
 *        when it is created
 */
struct RT_ComponentFactoryListener
{
    void *dummy;
};

/*ci
 * \brief Initializer for \ref RT_ComponentFactoryListener
 */
#define RT_ComponentFactoryListener_INITIALIZER \
{\
   NULL\
}

/*ci
 * \brief Function typedef for a factory initializer
 *
 * \param[in] property The property to initialize the factory with
 * \param[in] listener The listener to initialise the factory with
 *
 * \return An implementation of this function must return the initialized
 *         factory on success, NULL otherwise
 */
FUNCTION_MUST_TYPEDEF(
struct RT_ComponentFactory*
(*RT_ComponentFactory_initializeFactoryFunc)(
                                struct RT_ComponentFactoryProperty *property,
                                struct RT_ComponentFactoryListener *listener)
)

/*ci
 * \brief Function typedef for a factory finalizer
 *
 * \param[in]    factory  The factory being finalized
 * \param[inout] property The property the factory was initialized with if not NULL
 * \param[inout] listener The listener the factory was initialized with if not NULL
 *
 */
typedef void
(*RT_ComponentFactory_finalizeFactoryFunc)(struct RT_ComponentFactory *factory,
                                struct RT_ComponentFactoryProperty **property,
                                struct RT_ComponentFactoryListener **listener);

/*ci
 * \brief Function typedef for a factory create component function
 *
 * \param[in] factory  The factory creating an instance of a component
 * \param[in] property The property for the component
 * \param[in] listener The listener for the component
 *
 * \return An implementation of this function must return the initialized
 *         component on success, NULL otherwise
 */
FUNCTION_MUST_TYPEDEF(
RT_Component_T*
(*RT_ComponentFactory_createFunc)(struct RT_ComponentFactory *factory,
                                  struct RT_ComponentProperty *property,
                                  struct RT_ComponentListener *listener)
)

/*ci
 * \brief Function typedef for a factory delete component function
 *
 * \param[in] factory   The factory deleting an instance of a component
 * \param[in] component The component being deleted
 *
 * \return An implementation of this function must return the initialized
 *         component on success, NULL otherwise
 */
typedef void
(*RT_ComponentFactory_deleteFunc)(struct RT_ComponentFactory *factory,
                                 RT_Component_T* component);

/*ci
 * \brief Function typedef for a factory get interface function
 *
 * \param[in] factory   The factory to get the interface for
 *
 * \return An implementation of this function must return the interface
 *         structure for the component
 */
FUNCTION_MUST_TYPEDEF(
struct RT_ComponentI*
(*RT_ComponentFactory_get_ifFunc)(struct RT_ComponentFactory *factory)
)

/*ci
 * \brief A default factory id if an interface does not specify it
 */
#define RT_COMPONENT_FACTORY_ID_DEFAULT (0)

/*ci
 * \brief The RT_ComponentFactory interface
 */
struct RT_ComponentFactoryI
{
    /*ci
     * \brief interface id
     */
    OEI_INT32 id;

    /*ci
     * \brief function to call when the factory is first registered
     */
    RT_ComponentFactory_initializeFactoryFunc initialize;

    /*ci
     * \brief function to call when the factory is unregistered
     */
    RT_ComponentFactory_finalizeFactoryFunc finalize;

    /*ci
     * \brief function to call to create an new instance of a component
     */
    RT_ComponentFactory_createFunc create_component;

    /*ci
     * \brief function to call to delete an instance of a component
     */
    RT_ComponentFactory_deleteFunc delete_component;

    /*ci
     * \brief function to get the components interface implementation
     */
    RT_ComponentFactory_get_ifFunc get_if;
};

/*ci
 * \def RT_MAX_FACTORY_NAME
 * \brief Maximum length of a factory name, it does _not_ include \0
 */
#define RT_MAX_FACTORY_NAME 7

/*
 * Structure to represent a component name as either a 64 bit integer
 * or name
 */
#ifdef OEI_WIN32
#pragma warning(push)
#pragma warning(disable: 4522)
#endif
union DDSCPPDllExport RT_ComponentFactoryId
{
    struct
    {
        char _name[RT_MAX_FACTORY_NAME+1];
    } _name;

    struct
    {
        OEI_UINT32 _high;
        OEI_UINT32 _low;
    } _value;

#ifdef OEI_CPP
    public:
        RT_ComponentFactoryId();
        bool set_name(const char *const name);
#ifdef OEI_CERT
    private:
#else
    public:
#endif
    RT_ComponentFactoryId& operator=(const char *const name);
    RT_ComponentFactoryId& operator=(const RT_ComponentFactoryId& from);
    const RT_ComponentFactoryId& operator=(const RT_ComponentFactoryId& from) const;
    bool operator==(const RT_ComponentFactoryId& other) const;
    bool operator==(const char *const name) const;
    bool operator!=(const RT_ComponentFactoryId& other) const;
    bool operator!=(const char *const name) const;
#endif
};

#ifdef OEI_WIN32
#pragma warning(pop)
#endif
/*
 * Abstract type for the factory id
 */
typedef union RT_ComponentFactoryId  RT_ComponentFactoryId_T;

#define RT_ComponentFactoryId_INITIALIZER \
{\
    {{0}} \
}

/*ci
 * \brief Clear a factory id
 *
 * \param[in] id Id to clear
 */
RTDllExport void
RT_ComponentFactoryId_clear(union RT_ComponentFactoryId *id);

/*ci
 * \brief A ComponentFactory base-class. All component factories must derive
 *        from this class
 */
struct RT_ComponentFactory
{
    /*ci
     * \brief The factory interface
     */
    struct RT_ComponentFactoryI *intf;

    /*ci
     * \brief The factory instance
     */
    struct RT_ComponentFactory *_factory;

    /*ci
     * \brief The factory id
     */
    union RT_ComponentFactoryId _id;
};

/*ci
 * \brief Initializer for the \ref RT_ComponentFactory
 */
#define RT_ComponentFactory_INITIALIZER \
{ \
    NULL,\
    NULL,\
    {{0,0}}\
}

/*ci
 * \brief The abstract component factory type
 */
typedef struct RT_ComponentFactory RT_ComponentFactory_T;

/*ci
 * \brief Properties a \ref RT_Registry can be created with
 */
struct RT_RegistryProperty
{
    /*ci
     * \brief The maximum number of component factories which can be registered
     */
    OEI_SIZE_T max_factories;

    /*ci
     * \brief A database the component factory can use, if needed
     */
    DB_Database_T db;
};

/*ci
 * \brief Initializer for \ref RT_RegistryProperty
 */
#define RT_RegistryProperty_INITIALIZER \
{ \
    1,\
    NULL\
}

/*ci
 * \brief Default values for \ref RT_RegistryProperty
 */
extern RTDllVariable struct RT_RegistryProperty
                                 RTCOMPONENTFACTORY_REGISTRY_PROPERTY_DEFAULT;

/* forward declaraion for the abstract type definition */
struct RT_Registry;

/*ci
 * \brief Abstract RT_Registry type
 */
typedef struct RT_Registry RT_Registry_T;

/*ci
 * \brief Get the RT_Registry
 *
 * \details
 *
 * The RT_Registry exists as a single instance within a process.
 *
 * \return pointer to registry on success, NULL on failure
 *
 * \sa ref RT_Registry_finalize
 */
SHOULD_CHECK_RETURN RTDllExport RT_Registry_T*
RT_Registry_get_instance(void);

#ifndef OEI_CERT
/*ci
 * \brief Finalize the RT_Registry
 *
 * \details
 *
 * The RT_Registry exists as a single instance within a process. This
 * call finalizes the registry instance. If an already finalized registry
 * is finalized again nothing is done.
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa ref RT_Registry_get_instance
 */
SHOULD_CHECK_RETURN RTDllExport OEI_BOOL
RT_Registry_finalize(RT_Registry_T*);
#endif

/*e \dref_RT_Registry_register
 */
MUST_CHECK_RETURN RTDllExport OEI_BOOL
RT_Registry_register(RT_Registry_T *registry,
                            const char *name,
                            struct RT_ComponentFactoryI *intf,
                            struct RT_ComponentFactoryProperty *property,
                            struct RT_ComponentFactoryListener *listener);

/*e \dref_RT_Registry_unregister
 */
SHOULD_CHECK_RETURN RTDllExport OEI_BOOL
RT_Registry_unregister(RT_Registry_T *registry,
                            const char *name,
                            struct RT_ComponentFactoryProperty **property,
                            struct RT_ComponentFactoryListener **listener);

/*ci
 * \brief Find a component factory based on the name
 *
 * \details
 *
 * This function is used to find a component based on its name.
 *
 * \param[in] registry  Registry to search for the component factory in
 * \param[in] name      The name of the component factory to find

 * \return pointer to \ref RT_ComponentFactory on success, NULL on failure
 *
 * \sa ref RT_Registry_lookup_cid
 */
MUST_CHECK_RETURN RTDllExport struct RT_ComponentFactory*
RT_Registry_lookup(RT_Registry_T *registry,const char *name);

/*ci
 * \brief Find a component factory based on the name and class id
 *
 * \details
 *
 * This function is used to find a component based on its name <em> and
 * </em> the class id. If the name exists but is of the wrong class id
 * NULL is returned.
 *
 * \param[in]  registry Registry to search for the component factory in
 * \param[in]  name     The name of the component factory to find
 * \param[in]  cid      Component ID to choose from

 * \return pointer to \ref RT_ComponentFactory on success, NULL on failure
 *
 * \sa ref RT_Registry_lookup
 */
MUST_CHECK_RETURN RTDllExport struct RT_ComponentFactory*
RT_Registry_lookup_cid(RT_Registry_T *registry,const char *name,OEI_INT32 cid);

/*ci
 * \brief Get the current properties for the registry
 *
 * \details
 *
 * This function returns the current properties for the registry
 *
 * \param[in]   registry  Registry to return properties for
 * \param[out]  property  Current properties on successful return

 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa ref RT_Registry_set_property
 */
MUST_CHECK_RETURN RTDllExport OEI_BOOL
RT_Registry_get_property(RT_Registry_T *registry,
                        struct RT_RegistryProperty *property);

/*ci
 * \brief Set the properties for the registry
 *
 * \details
 *
 * This function sets the properties for the registry. Note that the properties
 * can only be changed until the first registration takes place.
 *
 * \param[in]  registry  Registry to set properties on
 * \param[in]  property  New registry properties

 * \return OEI_TRUE on success, OEI_FALSE on failure
 *
 * \sa ref RT_Registry_get_property
 */
MUST_CHECK_RETURN RTDllExport OEI_BOOL
RT_Registry_set_property(RT_Registry_T *registry,
                        struct RT_RegistryProperty *property);

/*ci
 * \brief Get the interface of a component factory
 *
 * \details
 *
 * Get the interface of a component factory
 *
 * \param[in]  factory  Factory to return id for
 *
 * \return Factory id
 */
RTDllExport OEI_INT32
RT_ComponentFactory_get_id(struct RT_ComponentFactory *factory);

/*ce \dref_RT_Registry_ComponentFactoryId_set_name
 * \brief Assign a name to a RT_ComponentFactoryId_T variable
 *
 * \details
 *
 * Use this function to assign a name to a RT_ComponentFactoryId_T variable,
 * such as the name of the discovery plugin to use.
 *
 * \param id   \st_in Id to initialize
 * \param name \st_in Name to initialize with
 *
 * \return OEI_TRUE on success, OEI_FALSE on failure
 */
MUST_CHECK_RETURN RTDllExport OEI_BOOL
RT_ComponentFactoryId_set_name(RT_ComponentFactoryId_T *id,
                               const char *const name);

/*ci
 * \brief Return the component factory id as a name
 *
 * \details
 *
 * Return the component factory id as a name
 *
 * \param[in] id Id to return name from
 *
 * \return id as name
 */
MUST_CHECK_RETURN RTDllExport const char*
RT_ComponentFactoryId_get_name(const RT_ComponentFactoryId_T *const id);

/*ci
 * \brief Compare a component factory id to a name
 *
 * \details
 *
 * Compare a component factory id to a name
 *
 * \param[in] id   Id to compare
 * \param[in] name Name to compare against
 *
 * \return OEI_TRUE if the id is equal to the name
 */
MUST_CHECK_RETURN RTDllExport OEI_BOOL
RT_ComponentFactoryId_equals(const RT_ComponentFactoryId_T *const id,
                             const char *const name);

/*ci
 * \brief Compare a two component factory ids
 *
 * \details
 *
 * Compare a two component factory ids
 *
 * \param[in] id1 Left side of comparison
 * \param[in] id2 Right side of comparison
 *
 * \return positive integer if id1 is greater than id2,
 *         negative integer if id1 is less than id2,
 *         zero if id1 is equal to id2
 */
MUST_CHECK_RETURN RTDllExport OEI_INT32
RT_ComponentFactoryId_compare(const RT_ComponentFactoryId_T *const id1,
                              const RT_ComponentFactoryId_T *const id2);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#endif

/*ci @} */
