//
// Created by istvan on 3/8/19.
//

#ifndef RIAPS_CORE_GROUPSHELL_H
#define RIAPS_CORE_GROUPSHELL_H

#include <groups/r_group.h>
#include <capnp/serialize.h>
#include <capnp/message.h>
#include <czmq.h>

namespace riaps {
    namespace groups {
        class Group;
    }
}

riaps::groups::Group* join_group(const char* group_type,
                                 const char* group_name,
                                 const char* component_id,
                                 const char* application_name,
                                 int   message_tpyesc,
                                 char** message_typesv,
                                 bool has_consensus,
                                 bool has_leader);

void leave_group(riaps::groups::Group* group);

bool send_message_to_leader(void* group, unsigned char* message, int len);
bool send_message_to_leader(riaps::groups::Group* group, capnp::MallocMessageBuilder& message);

bool send_group_message(void* group, unsigned char* message, int len);
bool send_group_message(riaps::groups::Group* group, capnp::MallocMessageBuilder& message);

void propose_value_to_leader();
bool send_vote(riaps::groups::Group* group, const char* propose_id, bool accept);
void propose_action_to_leader();
int  member_count();


#endif //RIAPS_CORE_GROUPSHELL_H
