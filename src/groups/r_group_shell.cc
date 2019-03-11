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

void send_group_message(void* group, unsigned char* message, int len) {
    auto group_ptr = static_cast<riaps::groups::Group*>(group);
    auto copy = new unsigned char[len];
    memcpy(copy, message, len);

    // CZMQ send() deletes the pointer.
    group_ptr->SendMessageAsBytes(copy, len);
}

void send_group_message(riaps::groups::Group* group, capnp::MallocMessageBuilder& message) {
    auto serializedMessage = capnp::messageToFlatArray(message);
    auto bytes = serializedMessage.asBytes();
    send_group_message(group, bytes.begin(), bytes.size());
}

void send_message_to_leader(void* zactor, void* message, int len) {

}