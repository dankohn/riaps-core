//
// Created by istvan on 2/14/19.
//

#include <iostream>
#include <dc/dc.h>

using namespace std;

static GroupPool gp;

extern "C" void* join_group(const char* group_id,
                           const char* component_uuid,
                           const char* component_name,
                           const char* application_name,
                           const char* actor_name) {
    riaps::groups::Group* gr = new riaps::groups::Group(
            {"gType", group_id},
            component_uuid,
            component_name,
            application_name,
            actor_name);
    auto rc = gr->InitGroup();
    if (!rc)cout << "ERRRORRR" << endl;
    return gr;
}

extern "C" uint16_t get_member_count(void* group_ptr) {
    return ((riaps::groups::Group*)group_ptr)->GetMemberCount();
}

extern "C" void init_group(void* group) {
    ((riaps::groups::Group*)group)->InitGroup();
}

extern "C" void leave_group(char* group_id, char* component_uuid) {
}

extern "C" void free_group(void* group_ptr) {
    delete (riaps::groups::Group*)group_ptr;
}

// returns list of group_ids
extern "C" void member_of(char* component_uuid) {

}