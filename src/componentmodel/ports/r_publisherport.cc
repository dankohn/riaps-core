#include <componentmodel/ports/r_publisherport.h>
#include <framework/rfw_network_interfaces.h>
#include <componentmodel/r_discoverdapi.h>

using namespace riaps::discovery;

namespace riaps{
    namespace ports {

        PublisherPort::PublisherPort(const ComponentPortPub &config,
                                     const std::string& application_name,
                                     const std::string& actor_name,
                                     const std::string& component_name,
                                     bool has_security)
            : PublisherPortBase((ComponentPortConfig*)&config,
                                application_name,
                                actor_name,
                                component_name,
                                has_security)

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
                logger()->error("Publisher port couldn't be registered.");
            }
        }
    }
}