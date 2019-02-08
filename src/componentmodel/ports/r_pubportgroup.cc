#include <componentmodel/ports/r_pubportgroup.h>

namespace riaps{
    namespace ports{
        GroupPublisherPort::GroupPublisherPort(const GroupPortPub &config,
                                               bool has_security,
                                               const std::string& component_name,
                                               const std::string& application_name,
                                               const std::string& actor_name,
                                               std::shared_ptr<spd::logger>& logger)
                : group_port_config_(config),
                  PublisherPortBase(&group_port_config_,
                                    has_security,
                                    component_name,
                                    application_name,
                                    actor_name,
                                    logger) {
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