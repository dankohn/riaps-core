//
// Created by parallels on 9/29/16.
//

#include <framework/rfw_network_interfaces.h>
#include <componentmodel/ports/r_responseport.h>

namespace riaps{
    namespace ports{

        ResponsePort::ResponsePort(const component_port_rep &config, const ComponentBase *parentComponent) :
            PortBase(PortTypes::Response, (component_port_config*)&config, parentComponent),
            SenderPort(this)
        {
            m_port_socket = zsock_new(ZMQ_REP);
            zsock_set_linger(m_port_socket, 500);
            zsock_set_sndtimeo(m_port_socket, 500);
            zsock_set_rcvtimeo(m_port_socket, 500);

            if (GetConfig()->isLocal){
                m_host = "127.0.0.1";
            } else {
                m_host = riaps::framework::Network::GetIPAddress();
            }

            if (m_host == "") {
                throw std::runtime_error("Response cannot be initiated. Cannot find  available network interface.");
            }

            std::string rep_endpoint = fmt::format("tcp://{}:!", m_host);//"tcp://" + _host + ":!";
            m_port = zsock_bind(m_port_socket, "%s", rep_endpoint.c_str());


            if (m_port == -1) {
                throw std::runtime_error("Couldn't bind response port.");
            }

            std::cout << "Response is created on : " << m_host << ":" << m_port << std::endl;


            if (!registerService(riaps::Actor::GetRunningActor()->GetApplicationName(),
                                 riaps::Actor::GetRunningActor()->GetActorName(),
                                  config.messageType,
                                  m_host,
                                  m_port,
                                  riaps::discovery::Kind::REP,
                                  (config.isLocal?riaps::discovery::Scope::LOCAL:riaps::discovery::Scope::GLOBAL),
                                  {})) {
                throw std::runtime_error("Response port couldn't be registered.");
            }
        }

        const component_port_rep* ResponsePort::GetConfig() const{
            return (component_port_rep*)GetPortBaseConfig();
        }

//        bool ResponsePort::Send(zmsg_t** msg) const {
//            //zmsg_pushstr(*msg, GetConfig()->rep_type.c_str());
//
//            int rc = zmsg_send(msg, _port_socket);
//            return rc == 0;
//        }

        ResponsePort* ResponsePort::AsResponsePort() {
            return this;
        }

        ResponsePort::~ResponsePort() noexcept {

        }

    }
}

