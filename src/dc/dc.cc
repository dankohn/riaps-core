//
// Created by istvan on 2/14/19.
//

#include <iostream>
#include <dc/dc.h>

using namespace std;

static GroupPool gp;

extern "C" void join_group(char* group_id, char* component_uuid) {
    cout << group_id << " " << component_uuid << endl;
}

extern "C" void leave_group(char* group_id, char* component_uuid) {
}

// returns list of group_ids
extern "C" void member_of(char* component_uuid) {

}