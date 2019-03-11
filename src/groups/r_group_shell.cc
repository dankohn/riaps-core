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
void send_group_message(void* group, unsigned char* message, int len) {
    auto group_ptr = static_cast<riaps::groups::Group*>(group);
    auto copy = new unsigned char[len];
    memcpy(copy, message, len);

    // CZMQ send() deletes the pointer.
    group_ptr->SendMessageAsBytes(copy, len);
}

/**
 * Sends a cap'n proto message to the group
 */
void send_group_message(riaps::groups::Group* group, capnp::MallocMessageBuilder& message) {
    auto serializedMessage = capnp::messageToFlatArray(message);
    auto bytes = serializedMessage.asBytes();
    send_group_message(group, bytes.begin(), bytes.size());
}

/**
 * Sends a message to the current group leader
 */
// TODO: bool return if the leader is not available
void send_message_to_leader(void* group, unsigned char* message, int len) {
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
void send_message_to_leader(riaps::groups::Group* group, capnp::MallocMessageBuilder& message) {
    auto serializedMessage = capnp::messageToFlatArray(message);
    auto bytes = serializedMessage.asBytes();
    send_message_to_leader(group, bytes.begin(), bytes.size());
}