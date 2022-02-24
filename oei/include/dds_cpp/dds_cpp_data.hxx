#ifndef dds_cpp_data_hxx
#define dds_cpp_data_hxx

#ifndef dds_cpp_dll_hxx
  #include "dds_cpp/dds_cpp_dll.hxx"
#endif
#ifndef dds_cpp_infrastructure_hxx
  #include "dds_cpp/dds_cpp_infrastructure.hxx"
#endif
#ifndef dds_cpp_publication_hxx
  #include "dds_cpp/dds_cpp_publication.hxx"
#endif
#ifndef dds_cpp_subscription_hxx
  #include "dds_cpp/dds_cpp_subscription.hxx"
#endif
#ifndef dds_cpp_domain_hxx
  #include "dds_cpp/dds_cpp_domain.hxx"
#endif


/* Make sure that NDDSUSERDllExport expands to nothing by default */
#ifndef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

/* ----------------------------------------------------------------- */

/* ================================================================= */
/*                   DATA WRITER                                     */
/* ================================================================= */

#define DDS_DATAWRITER_CPP(TDataWriter, TData) \
 \
class NDDSUSERDllExport DDSCPPDllExport TDataWriter : \
    public DDSDataWriter_impl \
{ \
    friend void* TData##Plugin_create_typed_datawriter(void* c_writer);\
    friend void TData##Plugin_delete_typed_datawriter(void* wrapper);\
public: \
    static TDataWriter* narrow(DDSDataWriter *writer); \
    DDSDataWriter* as_datawriter(); \
    DDS_ReturnCode_t write(const TData& instance_data, \
                                   const DDS_InstanceHandle_t& handle); \
    DDS_InstanceHandle_t register_instance(const TData& instance_data);\
    DDS_InstanceHandle_t register_instance_w_timestamp(\
            const TData& instance_data,\
            const DDS_Time_t& source_timestamp);\
    DDS_ReturnCode_t unregister_instance(\
            const TData& instance_data,\
            const DDS_InstanceHandle_t& handle);\
    DDS_ReturnCode_t unregister_instance_w_timestamp(\
            const TData& instance_data,\
            const DDS_InstanceHandle_t& handle,\
            const DDS_Time_t& source_timestamp);\
    DDS_ReturnCode_t dispose(\
            const TData& instance_data,\
            const DDS_InstanceHandle_t& handle);\
    DDS_ReturnCode_t dispose_w_timestamp(\
            const TData& instance_data,\
            const DDS_InstanceHandle_t& handle,\
            const DDS_Time_t& source_timestamp);\
    DDS_ReturnCode_t write_w_timestamp(\
            const TData& instance_data,\
            const DDS_InstanceHandle_t& handle,\
            const DDS_Time_t& source_timestamp);\
protected: \
    TDataWriter(DDS_DataWriter* cDataWriter); \
    virtual ~TDataWriter(); \
}


/* ----------------------------------------------------------------- */

/* ================================================================= */
/*                   DATA READER                                     */
/* ================================================================= */

#define DDS_DATAREADER_CPP(TDataReader, TData) \
 \
class NDDSUSERDllExport DDSCPPDllExport TDataReader : \
    public DDSDataReader_impl \
{ \
    friend void* TData##Plugin_create_typed_datareader(void* c_reader);\
    friend void TData##Plugin_delete_typed_datareader(void* wrapper);\
  public: \
      DDSDataReader* as_datareader(); \
    static TDataReader* narrow(DDSDataReader *reader); \
  public: \
    DDS_ReturnCode_t read_next_sample(TData& received_data, \
                                              DDS_SampleInfo& sample_info); \
    \
    DDS_ReturnCode_t take_next_sample(TData& received_data, \
                                              DDS_SampleInfo& sample_info); \
    \
    DDS_ReturnCode_t read(\
        TData##Seq& received_data,\
        DDS_SampleInfoSeq& info_seq,\
        DDS_Long max_samples,\
        DDS_SampleStateMask sample_states,\
        DDS_ViewStateMask view_states,\
        DDS_InstanceStateMask instance_states);\
    \
    DDS_ReturnCode_t take(\
        TData##Seq& received_data,\
        DDS_SampleInfoSeq& info_seq,\
        DDS_Long max_samples,\
        DDS_SampleStateMask sample_states,\
        DDS_ViewStateMask view_states,\
        DDS_InstanceStateMask instance_states);\
    \
    DDS_ReturnCode_t read_instance(\
        TData##Seq& received_data,\
        DDS_SampleInfoSeq& info_seq,\
        DDS_Long max_samples,\
        const DDS_InstanceHandle_t& a_handle,\
        DDS_SampleStateMask sample_states,\
        DDS_ViewStateMask view_states,\
        DDS_InstanceStateMask instance_states);\
    \
    DDS_ReturnCode_t take_instance(\
        TData##Seq& received_data,\
        DDS_SampleInfoSeq& info_seq,\
        DDS_Long max_samples,\
        const DDS_InstanceHandle_t& a_handle,\
        DDS_SampleStateMask sample_states,\
        DDS_ViewStateMask view_states,\
        DDS_InstanceStateMask instance_states);\
    \
    DDS_InstanceHandle_t\
    lookup_instance(const TData& key_holder);\
    \
    DDS_ReturnCode_t return_loan(\
        TData##Seq& received_data,\
        DDS_SampleInfoSeq& info_seq);\
  protected: \
    TDataReader(DDS_DataReader *cDataReader); \
    virtual ~TDataReader(); \
}

/* ================================================================= */
/*                   TYPE SUPPORT                                    */
/* ================================================================= */

#define DDS_TYPESUPPORT_CPP(TTypeSupport, TData)                         \
                                                                         \
class NDDSUSERDllExport DDSCPPDllExport TTypeSupport                     \
{ \
  public: \
    static bool initialize_data(TData* a_data); \
    static bool finalize_data(TData* a_data); \
    static TData* create_data(); \
    static void delete_data(TData* a_data); \
    static bool copy_data(TData* a_dst, const TData* a_src); \
    static const char* get_type_name();\
    static DDS_ReturnCode_t register_type(\
                    DDSDomainParticipant* participant,\
                    const char* type_name);\
    static DDS_ReturnCode_t unregister_type(\
                    DDSDomainParticipant* participant,\
                    const char* type_name);\
private: \
  TTypeSupport(); \
  ~TTypeSupport(); \
}


#ifdef DOXYGEN_DOCUMENTATION_ONLY


/* ----------------------------------------------------------------- */
/*e \dref_FooDataWriter
 */
class   FooDataWriter :
    public DDSDataWriter_impl
{
    friend void* FooPlugin_create_typed_datawriter(void* c_writer);\
    friend void FooPlugin_delete_typed_datawriter(void* wrapper);\
public: \
    /*e \dref_FooDataWriter_narrow
     */
    static FooDataWriter* narrow(DDSDataWriter *writer); \

    /*e \dref_FooDataWriter_as_datawriter
     */
    DDSDataWriter* as_datawriter(); \

    /*e \dref_FooDataWriter_write
     */
    DDS_ReturnCode_t write(const Foo& instance_data, \
                                   const DDS_InstanceHandle_t& handle); \

    /*e \dref_FooDataWriter_register_instance
     */
    DDS_InstanceHandle_t register_instance(const Foo& instance_data);\

    /*e \dref_FooDataWriter_register_instance_w_timestamp
     */
    DDS_InstanceHandle_t register_instance_w_timestamp(\
            const Foo& instance_data,\
            const DDS_Time_t& source_timestamp);\

    /*e \dref_FooDataWriter_unregister_instance
     */
    DDS_ReturnCode_t unregister_instance(\
            const Foo& instance_data,\
            const DDS_InstanceHandle_t& handle);\

    /*e \dref_FooDataWriter_unregister_instance_w_timestamp
     */
    DDS_ReturnCode_t unregister_instance_w_timestamp(\
            const Foo& instance_data,\
            const DDS_InstanceHandle_t& handle,\
            const DDS_Time_t& source_timestamp);\

     /*e \dref_FooDataWriter_dispose
     */
    DDS_ReturnCode_t dispose(\
            const Foo& instance_data,\
            const DDS_InstanceHandle_t& instance_handle);\

    /*e \dref_FooDataWriter_dispose_w_timestamp
     */
    DDS_ReturnCode_t dispose_w_timestamp(\
            const Foo& instance_data,\
            const DDS_InstanceHandle_t& instance_handle,\
            const DDS_Time_t& source_timestamp);\

    /*e \dref_FooDataWriter_write_w_timestamp
     */
    DDS_ReturnCode_t write_w_timestamp(\
            const Foo& instance_data,\
            const DDS_InstanceHandle_t& handle,\
            const DDS_Time_t& source_timestamp);

protected: \
    FooDataWriter(DDS_DataWriter* cDataWriter); \
    virtual ~FooDataWriter(); \
};


#endif /* DOXYGEN_DOCUMENTATION_ONLY */

#ifdef DOXYGEN_DOCUMENTATION_ONLY

/* ----------------------------------------------------------------- */
/*e \dref_FooDataReader
 */
class   FooDataReader : \
    public DDSDataReader_impl \
{ \
    friend void* FooPlugin_create_typed_datareader(void* c_reader);\
    friend void FooPlugin_delete_typed_datareader(void* wrapper);\
  public: \
    DDSDataReader* as_datareader(); \

     /*e \dref_FooDataReader_narrow
     */
    static FooDataReader* narrow(DDSDataReader *reader); \

  public: \

    /*e \dref_FooDataReader_return_loan
     */
    DDS_ReturnCode_t return_loan(FooSeq& received_data,
                     DDS_SampleInfoSeq& info_seq);\

    /*e \dref_FooDataReader_read_next_sample
     */
    DDS_ReturnCode_t read_next_sample(Foo& received_data, \
                                              DDS_SampleInfo& sample_info); \

    /*e \dref_FooDataReader_take_next_sample
     */
    DDS_ReturnCode_t take_next_sample(Foo& received_data, \
                                              DDS_SampleInfo& sample_info); \

    /*e \dref_FooDataReader_read
     */
    DDS_ReturnCode_t read(\
        FooSeq& received_data,\
        DDS_SampleInfoSeq& info_seq,\
        DDS_Long max_samples,\
        DDS_SampleStateMask sample_states,\
        DDS_ViewStateMask view_states,\
        DDS_InstanceStateMask instance_states);\

    /*e \dref_FooDataReader_take
     */
    DDS_ReturnCode_t take(\
        FooSeq& received_data,\
        DDS_SampleInfoSeq& info_seq,\
        DDS_Long max_samples,\
        DDS_SampleStateMask sample_states,\
        DDS_ViewStateMask view_states,\
        DDS_InstanceStateMask instance_states);\

    /*e \dref_FooDataReader_read_instance
     */
    DDS_ReturnCode_t read_instance(\
        FooSeq& received_data,\
        DDS_SampleInfoSeq& info_seq,\
        DDS_Long max_samples,\
        const DDS_InstanceHandle_t& a_handle,\
        DDS_SampleStateMask sample_states,\
        DDS_ViewStateMask view_states,\
        DDS_InstanceStateMask instance_states);\

    /*e \dref_FooDataReader_take_instance
     */
    DDS_ReturnCode_t take_instance(\
        FooSeq& received_data,\
        DDS_SampleInfoSeq& info_seq,\
        DDS_Long max_samples,\
        const DDS_InstanceHandle_t& a_handle,\
        DDS_SampleStateMask sample_states,\
        DDS_ViewStateMask view_states,\
        DDS_InstanceStateMask instance_states);\

  protected: \
    FooDataReader(DDS_DataReader *cDataReader); \
    virtual ~FooDataReader(); \
}

#endif /* DOXYGEN_DOCUMENTATION_ONLY */

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/* ----------------------------------------------------------------- */
/*e \dref_FooTypeSupport
 *
 * Operations of a type's TypeSupport have been renamed by the C++
 * API: all operations use the suffix "_data" to avoid collisions
 * with reserved C++ keyworkds (such as delete).
 *
 */
class   FooTypeSupport                     \
{ \
  public: \
    /*e \dref_FooTypeSupport_initialize_data
     */
    static bool initialize_data(Foo* a_data); \
    /*e \dref_FooTypeSupport_finalize_data
     */
    static bool finalize_data(Foo* a_data); \
    /*e \dref_FooTypeSupport_create_data
     */
    static Foo* create_data(); \
    /*e \dref_FooTypeSupport_delete_data
     */
    static void delete_data(Foo* a_data); \
    /*e \dref_FooTypeSupport_copy_data
     */
    static bool copy_data(Foo* a_dst, Foo* a_src); \

    /*e \dref_FooTypeSupport_register_type
     */
    static DDS_ReturnCode_t register_type(\
                    DDSDomainParticipant* participant,\
                    const char* type_name)\
    /*e \dref_FooTypeSupport_unregister_type
     */
    static DDS_ReturnCode_t unregister_type(\
                    DDSDomainParticipant* participant,\
                    const char* type_name)\
private: \
  FooTypeSupport(); \
  ~FooTypeSupport(); \
}

#endif /* DOXYGEN_DOCUMENTATION_ONLY */


#endif /* dds_cpp_data_hxx */

