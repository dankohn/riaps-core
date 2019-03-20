//
// Created by istvan on 3/8/19.
//

#include <groups/r_group.h>
#include <groups/groupshell.h>
#include <componentmodel/r_configuration.h>

#include <spdlog_setup/conf.h>

#include <memory>
#include <utility>
#include <functional>

using namespace std;
using namespace riaps::groups;

namespace spd = spdlog;

/**
 * Sends a message to the group.
 * The message is represented as a byte array.
 */
bool send_group_message(void* group, unsigned char* message, int len) {
    auto group_ptr = static_cast<riaps::groups::Group*>(group);
    auto copy = new unsigned char[len];
    memcpy(copy, message, len);

    // CZMQ send() deletes the pointer.
    group_ptr->SendGroupMessage(copy, len);
}

/**
 * Sends a cap'n proto message to the group
 */
bool send_group_message(riaps::groups::Group* group, capnp::MallocMessageBuilder& message) {
    auto serializedMessage = capnp::messageToFlatArray(message);
    auto bytes = serializedMessage.asBytes();
    send_group_message(group, bytes.begin(), bytes.size());
}

/**
 * Sends a message to the current group leader
 */
// TODO: bool return if the leader is not available
bool send_message_to_leader(void* group, unsigned char* message, int len) {
    auto group_ptr = static_cast<riaps::groups::Group*>(group);
    auto copy = new unsigned char[len];
    memcpy(copy, message, len);

    // CZMQ send() deletes the pointer.
    group_ptr->SendMessageToLeader(copy, len);
}

/**
 * Sends a cap'n proto message to the current groupleader
 */
//TODO: bool return if the leader is not available.
bool send_message_to_leader(riaps::groups::Group* group, capnp::MallocMessageBuilder& message) {
    auto serializedMessage = capnp::messageToFlatArray(message);
    auto bytes = serializedMessage.asBytes();
    send_message_to_leader(group, bytes.begin(), bytes.size());
}

riaps::groups::Group* join_group(const char* group_type,
                                 const char* group_name,
                                 const char* application_name,
                                 const char* actor_name,
                                 const char* component_id,
                                 bool   has_consensus,
                                 bool   has_leader,
                                 bool   has_security) {
    riaps::groups::GroupId gid;
    gid.group_type_id = group_type;
    gid.group_name = group_name;

    GroupTypeConf gtc;
    gtc.group_type_id = gid.group_type_id;
    gtc.has_consensus = has_consensus;
    gtc.has_leader  = has_leader;

    auto new_group = new Group(gid,
                               gtc,
                               application_name,
                               actor_name,
                               component_id,
                               has_security);


    return new_group;
}


void leave_group(riaps::groups::Group* group) {
    delete group;
}

bool send_vote(riaps::groups::Group* group, const char* propose_id, bool accept) {
    return group->SendVote(propose_id, accept);
}