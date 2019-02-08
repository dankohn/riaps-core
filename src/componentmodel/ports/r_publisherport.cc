#include <componentmodel/ports/r_publisherport.h>
#include <framework/rfw_network_interfaces.h>
#include <componentmodel/r_discoverdapi.h>

using namespace riaps::discovery;

namespace riaps{
    namespace ports {

        PublisherPort::PublisherPort(const ComponentPortPub &config,
            bool has_security,
            const std::string& component_name,
            const std::string& application_name,
            const std::string& actor_name,
            std::shared_ptr<spd::logger>& logger)
            : PublisherPortBase((ComponentPortConfig*)&config, has_security, component_name, application_name, actor_name, logger)

        {
            InitSocket();
            if (!Disco::RegisterService(
                    this->application_name(),
                    this->actor_name(),
                    config.message_type,
                    host_,
                    port_,
                    riaps::discovery::Kind::PUB,
                    (config.is_local ? riaps::discovery::Scope::LOCAL : riaps::discovery::Scope::GLOBAL),
                    {})) {
                this->logger()->error("Publisher port couldn't be registered.");
            }
        }
    }
}