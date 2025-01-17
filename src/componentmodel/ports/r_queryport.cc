//
// Created by istvan on 9/30/16.
//

#include <componentmodel/ports/r_queryport.h>
#include <framework/rfw_network_interfaces.h>

using namespace std;
using namespace riaps::discovery;

namespace riaps {
    namespace ports {

        QueryPort::QueryPort(const ComponentPortQry &config, const ComponentBase *component)
                : PortBase(PortTypes::Query,
                           (ComponentPortConfig*)(&config),
                           component),
                  m_capnpReader(capnp::FlatArrayMessageReader(nullptr)) {
            port_socket_ = zsock_new(ZMQ_DEALER);
            m_socketId = zuuid_new();
            int timeout = 500;//msec
            int lingerValue = 0;
            int connectTimeout = 1000; //msec
            zmq_setsockopt(port_socket_, ZMQ_SNDTIMEO, &timeout , sizeof(int));
            zmq_setsockopt(port_socket_, ZMQ_LINGER, &lingerValue, sizeof(int));
            zsock_set_identity(port_socket_, zuuid_str(m_socketId));
            m_isConnected = false;
        }

        void QueryPort::Init() {

            const ComponentPortQry* current_config = GetConfig();
            const string host = (current_config->is_local) ? "127.0.0.1" : riaps::framework::Network::GetIPAddress();

            auto results =
                    Disco::SubscribeToService(
                            parent_component()->actor()->application_name(),
                            parent_component()->component_config().component_name,
                            parent_component()->actor()->actor_name(),
                            host,
                            riaps::discovery::Kind::QRY,
                            (current_config->is_local ? riaps::discovery::Scope::LOCAL
                                                      : riaps::discovery::Scope::GLOBAL),
                            current_config->port_name, // Subscriber name
                            current_config->message_type);

            for (auto result : results) {
                string endpoint = fmt::format("tcp://{0}:{1}", result.host_name, result.port);
                ConnectToResponse(endpoint);
            }
        }

        bool QueryPort::ConnectToResponse(const std::string &ansEndpoint) {
            int rc = zsock_connect(port_socket_, "%s", ansEndpoint.c_str());

            if (rc != 0) {
                logger()->error("Queryport {} couldn't connect to {}", GetConfig()->port_name, ansEndpoint);
                return false;
            }

            m_isConnected = true;
            logger()->info("Queryport connected to: {}", ansEndpoint);
            return true;
        }

        const ComponentPortQry* QueryPort::GetConfig() const{
            return (ComponentPortQry*) config();
        }

        PortError QueryPort::SendQuery(capnp::MallocMessageBuilder &message,std::string& requestId, bool addTimestamp) const {
//            if (port_socket_ == nullptr || !m_isConnected){
//                return false;
//            }

            zframe_t* userFrame;
            userFrame << message;

            zframe_t* tsFrame = nullptr;
            if (addTimestamp){
                int64_t ztimeStamp = zclock_time();
                tsFrame = zframe_new(&ztimeStamp, sizeof(ztimeStamp));
            } else{
                tsFrame = zframe_new_empty();
            }
            // Generate uniqueId
            std::string msgId;
            {
                auto id = zuuid_new();
                msgId.assign(zuuid_str(id));
                zuuid_destroy(&id);
            }

            int rc = zsock_send(const_cast<zsock_t*>(port_socket()),
                                "sff",
                                msgId.c_str() ,
                                userFrame,
                                tsFrame
                                );

            zframe_destroy(&userFrame);
            zframe_destroy(&tsFrame);
            if (rc == 0) {
                requestId = msgId;
            }
            return PortError(rc);
        }

        QueryPort::~QueryPort() noexcept {
            zuuid_destroy(&m_socketId);
        }


    }
}
