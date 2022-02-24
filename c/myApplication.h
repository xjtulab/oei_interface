#ifndef my_Application_h
#define my_Application_h

#include "oei_c.h"

struct Application {
    DDS_DomainParticipant *participant;
    char topic_name[255];
    char type_name[255];
    DDS_Long sleep_time;
    DDS_Long count;
    DDS_Topic *topic;
};

extern void 
Application_help(char *appname);


#if 1
extern struct Application*
MyApplication_create(const char *local_participant_name,
                  const char *remote_participant_name,
                  DDS_Long domain_id, char *udp_intf, char *peer, 
                  DDS_Long sleep_time, DDS_Long count);
#endif

extern void MyRegistAll();
extern void UnMyRegistAll();


extern DDS_ReturnCode_t
MyApplication_enable(struct Application *application);

extern void
MyApplication_delete(struct Application *application);

#endif
