#include <componentmodel/ports/r_subscriberport.h>
#include <framework/rfw_network_interfaces.h>

using namespace std;
using namespace riaps::discovery;

namespace riaps{
    namespace ports {
        SubscriberPort::SubscriberPort(const ComponentPortSub &config,
                                       const std::string& application_name,
                                       const std::string& actor_name,
                                       const std::string& component_name,
                                       bool has_security)
                : SubscriberPortBase((ComponentPortConfig*)&config,
                                     application_name,
                                     actor_name,
                                     component_name,
                                     has_security) {

        }

        void SubscriberPort::Init() {
            ComponentPortSub* current_config = (ComponentPortSub*)GetConfig();
            const string host = (current_config->is_local) ? "127.0.0.1" : riaps::framework::Network::GetIPAddress();

            auto results =
                    Disco::SubscribeToService(
                            this->application_name(),
                            this->component_name(),
                            this->actor_name(),
                            host,
                            riaps::discovery::Kind::SUB,
                            (current_config->is_local ? riaps::discovery::Scope::LOCAL
                                                      : riaps::discovery::Scope::GLOBAL),
                            current_config->port_name, // Subscriber name
                            current_config->message_type);
            for (auto& result : results) {
                string endpoint = fmt::format("tcp://{}:{}", result.host_name, result.port);
                ConnectToPublihser(endpoint);
            }
        }
    }

}