//
// Created by istvan on 10/17/17.
//

#include <componentmodel/ports/r_subportgroup.h>

namespace riaps{
    namespace ports{

        GroupSubscriberPort::GroupSubscriberPort(const ComponentPortSub &config,
                                                 const std::string& application_name,
                                                 const std::string& actor_name,
                                                 const std::string& component_name,
                                                 bool has_security)
            : group_port_config_(config), SubscriberPortBase(&group_port_config_,
                                                             application_name,
                                                             actor_name,
                                                             component_name,
                                                             has_security) {
        }
    }
}