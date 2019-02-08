//
// Created by istvan on 5/17/17.
//

#include <componentmodel/ports/r_insideport.h>

namespace riaps{
    namespace ports{

        InsidePort::InsidePort(const ComponentPortIns &config, InsidePortMode mode,
                               bool has_security,
                               const std::string& component_name,
                               const std::string& application_name,
                               const std::string& actor_name,
                               std::shared_ptr<spd::logger>& logger)
            : PortBase(PortTypes::Inside,
                       (ComponentPortConfig*)&config,
                       has_security,
                       component_name,
                       application_name,
                       actor_name,
                       logger),
              SenderPort(this)
        {
            endpoint_ = fmt::format("inproc://inside_{}", config.port_name);

            if (mode == InsidePortMode::CONNECT){
                port_socket_ = zsock_new_pair(endpoint_.c_str());
            } else {
                port_socket_ = zsock_new(ZMQ_PAIR);
                zsock_bind(port_socket_, "%s", endpoint_.c_str());
            }

            zsock_set_rcvtimeo(port_socket_, 500);
        }

        const ComponentPortIns* InsidePort::GetConfig() const {
            return (ComponentPortIns*) config();
        }

        const std::string& InsidePort::GetEndpoint() {
            return endpoint_;
        }

        bool InsidePort::Recv(zmsg_t** insideMessage) {
            *insideMessage = zmsg_recv((void*) port_socket());
            return true;
        }

        InsidePort::~InsidePort() noexcept {

        }

    }
}