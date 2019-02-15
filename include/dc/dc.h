//
// Created by istvan on 2/14/19.
//

#ifndef RIAPS_CORE_DC_H
#define RIAPS_CORE_DC_H

#include <groups/r_group.h>
#include <unordered_map>
#include <memory>


class GroupPool {
public:
    GroupPool()  = default;

    void JoinGroup(const std::string& group_id, const std::string& component_id);
    void LeaveGroup(const std::string& group_id, const std::string& component_id);

    ~GroupPool() = default;
private:

    std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<riaps::groups::Group>>> groups_;

};

#endif //RIAPS_CORE_DC_H
