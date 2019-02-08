//
// Created by parallels on 9/29/16.
//

#include <framework/rfw_security.h>
#include <framework/rfw_network_interfaces.h>
#include <componentmodel/ports/r_responseport.h>

using namespace std;
using namespace riaps::discovery;

namespace riaps{
    namespace ports{

        ResponsePort::ResponsePort(const ComponentPortRep &config, bool has_security,
                                   const std::string& component_name,
                                   const std::string& application_name,
                                   const std::string& actor_name,
                                   std::shared_ptr<spd::logger>& logger)
                : PortBase(PortTypes::Response,
                           (ComponentPortConfig*)(&config),
                           has_security,
                           component_name,
                           application_name,
                           actor_name,
                           logger),
                SenderPort(this),
                RecvPort(this)
        {
            port_socket_ = zsock_new(ZMQ_REP);
            zsock_set_linger(port_socket_, 500);
            zsock_set_sndtimeo(port_socket_, 500);
            zsock_set_rcvtimeo(port_socket_, 500);

            if (GetConfig()->is_local) {
                host_ = "127.0.0.1";
            } else {
                host_ = riaps::framework::Network::GetIPAddress();
            }

            if (host_ == "") {
                this->logger()->error("Response cannot be initiated. Cannot find  available network interface.");
            }

            // The port is NOT local AND encrypted
            if (!GetConfig()->is_local && this->has_security()) {
//                zactor_t *auth = zactor_new (zauth, NULL);
//                auth_ = shared_ptr<zactor_t>(auth, [](zactor_t* z) {zactor_destroy(&z);});
//                //zstr_sendx (auth, "VERBOSE", NULL);
//                //zsock_wait (auth);
//                zstr_sendx (auth, "CURVE", CURVE_FOLDER, NULL);
//                zsock_wait (auth);
////                zstr_sendx (auth, "ALLOW", "192.168.1.104", "192.168.1.101", NULL);
////                zsock_wait (auth);
                if (port_certificate_ != nullptr) {
                    zcert_apply (port_certificate_.get(), port_socket_);
                    zsock_set_curve_server (port_socket_, 1);
                } else {
                    this->logger()->error("Port certificate is null, cannot create port: {}", port_name());
                    return;
                }
            }

            string rep_endpoint = fmt::format("tcp://{}:!", host_);
            port_ = zsock_bind(port_socket_, "%s", rep_endpoint.c_str());

            if (port_ == -1) {
                this->logger()->error("Couldn't bind response port.");
            }
            // TODO: spd logger
            this->logger()->debug("Response is created on {}:{}", host_, port_);
            this->logger()->debug("{}.host_ = {}", __FUNCTION__, host_);
            if (!Disco::RegisterService(
                    this->application_name(),
                    this->actor_name(),
                    config.message_type,
                    host_,
                    port_,
                    riaps::discovery::Kind::REP,
                    (config.is_local ? riaps::discovery::Scope::LOCAL : riaps::discovery::Scope::GLOBAL),
                    {})) {
                this->logger()->error("Response port couldn't be registered.");
            }
        }

        const ComponentPortRep* ResponsePort::GetConfig() const{
            return (ComponentPortRep*) config();
        }

        ResponsePort::~ResponsePort() noexcept {

        }
    }
}

