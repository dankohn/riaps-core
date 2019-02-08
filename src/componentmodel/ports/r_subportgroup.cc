//
// Created by istvan on 10/17/17.
//

#include <componentmodel/ports/r_subportgroup.h>

namespace riaps{
    namespace ports{

        GroupSubscriberPort::GroupSubscriberPort(const ComponentPortSub &config, bool has_security,
                                                 const std::string& component_name,
                                                 const std::string& application_name,
                                                 const std::string& actor_name,
                                                 std::shared_ptr<spd::logger>& logger)
            : group_port_config_(config), SubscriberPortBase(&group_port_config_, has_security, component_name, application_name, actor_name, logger) {
        }
    }
}