#ifndef __DPDE_SUBSCRIBER__
#define __DPDE_SUBSCRIBER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oei_c.h"
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"

#include "HelloWorld.h"
#include "HelloWorldSupport.h"
#include "HelloWorldApplication.h"


void init_subscriber(const char *local_name, const char *remote_name, DDS_Long domain_id, 
                        char *udp_intf, char *peer, DDS_Long sleep_time, DDS_Long count);
char *subscribe_msg(int seconds);



#endif

