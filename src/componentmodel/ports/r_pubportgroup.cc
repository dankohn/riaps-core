#include <componentmodel/ports/r_pubportgroup.h>

namespace riaps{
    namespace ports{
        GroupPublisherPort::GroupPublisherPort(const GroupPortPub &config,
                                               const std::string& application_name,
                                               const std::string& actor_name,
                                               const std::string& component_name,
                                               bool has_security)
                : group_port_config_(config),
                  PublisherPortBase(&group_port_config_,
                                    application_name,
                                    actor_name,
                                    component_name,
                                    has_security) {
            InitSocket();
        }

        riaps::groups::GroupService GroupPublisherPort::GetGroupService() {
            riaps::groups::GroupService result;

            result.address     = fmt::format("{}:{}",host_,port_);
            result.message_type = GetConfig()->message_type;

            return result;
        }
    }
}