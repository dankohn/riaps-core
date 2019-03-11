//
// Created by istvan on 3/8/19.
//

#ifndef RIAPS_CORE_GROUPSHELL_H
#define RIAPS_CORE_GROUPSHELL_H

#include <capnp/serialize.h>
#include <capnp/message.h>
#include <czmq.h>



void join_group();
void leave_group();
void send_message_to_leader(void* zactor, void* message, int len);
void send_message_to_leader(void* zactor, capnp::MallocMessageBuilder& message);
void send_group_message(void* group, unsigned char* message, int len);
void send_group_message(riaps::groups::Group* group, capnp::MallocMessageBuilder& message);
void propose_value_to_leader();
void send_vote();
void propose_action_to_leader();
int  member_count();


#endif //RIAPS_CORE_GROUPSHELL_H
