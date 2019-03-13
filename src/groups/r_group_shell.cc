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
                                 const char* component_id,
                                 const char* application_name,
                                 int   message_tpyesc,
                                 char** message_typesv,
                                 bool has_consensus,
                                 bool has_leader) {
    riaps::groups::GroupId gid;
    gid.group_type_id = group_type;
    gid.group_name = group_name;
    riaps::groups::GroupDetails gd;
    gd.component_id = component_id;
    gd.app_name = application_name;
    gd.group_id = gid;

    for (int i = 0; i<message_tpyesc; i++) {
        GroupService gs;
        gs.message_type = message_typesv[i];
        gd.group_services.push_back(gs);
    }

    auto new_group = new Group(gd, has_leader, has_consensus);

    if (new_group->InitGroup()) {
        return new_group;
    }
    return nullptr;
}

void leave_group(riaps::groups::Group* group) {
    delete group;
}

bool send_vote(riaps::groups::Group* group, const char* propose_id, bool accept) {
    return group->SendVote(propose_id, accept);
}