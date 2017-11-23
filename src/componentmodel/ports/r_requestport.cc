//
// Created by istvan on 9/30/16.
//

#include <componentmodel/ports/r_requestport.h>


namespace riaps {
    namespace ports {

        RequestPort::RequestPort(const _component_port_req &config, const ComponentBase *parentComponent)
                : PortBase(PortTypes::Request, (component_port_config*)(&config), parentComponent),
                  SenderPort(this),
                  _capnpReader(capnp::FlatArrayMessageReader(nullptr)) {
            _port_socket = zsock_new(ZMQ_REQ);

            auto i = zsock_rcvtimeo (_port_socket);

            int timeout = 500;//msec
            int lingerValue = 0;
            int connectTimeout = 1000; //msec
            zmq_setsockopt(_port_socket, ZMQ_SNDTIMEO, &timeout , sizeof(int));
            zmq_setsockopt(_port_socket, ZMQ_LINGER, &lingerValue, sizeof(int));

            i = zsock_rcvtimeo (_port_socket);

            _isConnected = false;
        }



        void RequestPort::Init() {

            const _component_port_req* current_config = GetConfig();

            auto results =
                    subscribeToService(GetParentComponent()->GetActor()->GetApplicationName(),
                                         GetParentComponent()->GetConfig().component_name,
                                         GetParentComponent()->GetActor()->GetActorName(),
                                         riaps::discovery::Kind::REQ,
                                         (current_config->isLocal?riaps::discovery::Scope::LOCAL:riaps::discovery::Scope::GLOBAL),
                                         current_config->portName, // Subscriber name
                                         current_config->messageType);

            for (auto result : results) {
                std::string endpoint = "tcp://" + result.host_name + ":" + std::to_string(result.port);
                ConnectToResponse(endpoint);
            }
        }

        bool RequestPort::ConnectToResponse(const std::string &rep_endpoint) {
            int rc = zsock_connect(_port_socket, "%s", rep_endpoint.c_str());

            if (rc != 0) {
                std::cout << "Request '" + GetConfig()->portName + "' couldn't connect to " + rep_endpoint
                          << std::endl;
                return false;
            }

            _isConnected = true;
            std::cout << "Request port connected to: " << rep_endpoint << std::endl;
            return true;
        }

        const _component_port_req* RequestPort::GetConfig() const{
            return (_component_port_req*)GetPortBaseConfig();
        }

        RequestPort* RequestPort::AsRequestPort() {
            return this;
        }

//        bool RequestPort::Recv(std::string& messageType, riaps::MessageBase* message) {
//            zmsg_t* msg = zmsg_recv((void*)GetSocket());
//
//            if (msg){
//                char* msgType = zmsg_popstr(msg);
//                messageType = msgType;
//                if (msgType!=NULL){
//                    zframe_t* bodyFrame = zmsg_pop(msg);
//                    size_t size = zframe_size(bodyFrame);
//                    byte* data = zframe_data(bodyFrame);
//
//                    auto capnp_data = kj::arrayPtr(reinterpret_cast<const capnp::word*>(data), size / sizeof(capnp::word));
//                   _capnpReader = capnp::FlatArrayMessageReader(capnp_data);
//                    message->InitReader(&_capnpReader);
//
//                    zframe_destroy(&bodyFrame);
//                    return true;
//                }
//                return false;
//            }
//            zmsg_destroy(&msg);
//
//            return false;
//        }


        bool RequestPort::Recv(capnp::FlatArrayMessageReader** messageReader) {
            zmsg_t* msg = zmsg_recv((void*)GetSocket());

            if (msg){
                //char* msgType = zmsg_popstr(msg);
                //messageType = msgType;
                //if (msgType!=NULL){
                    zframe_t* bodyFrame = zmsg_pop(msg);
                    size_t size = zframe_size(bodyFrame);
                    byte* data = zframe_data(bodyFrame);

                    auto capnp_data = kj::arrayPtr(reinterpret_cast<const capnp::word*>(data), size / sizeof(capnp::word));
                    _capnpReader = capnp::FlatArrayMessageReader(capnp_data);
                    *messageReader = &_capnpReader;

                    zframe_destroy(&bodyFrame);
                    return true;
                //}
                //return false;
            }
            zmsg_destroy(&msg);

            return false;
        }

        bool RequestPort::Send(capnp::MallocMessageBuilder &message) const {
            if (_port_socket == nullptr || !_isConnected){
                return false;
            }

            zmsg_t* zmsg;
            zmsg << message;

            int rc = zmsg_send(&zmsg, const_cast<zsock_t*>(GetSocket()));
            return rc==0;
        }

        RequestPort::~RequestPort() noexcept {

        }

        // Before sending the publisher sets up the message type
//        bool RequestPort::Send(zmsg_t **msg) const {
//            if (_port_socket == NULL || !_isConnected){
//                zmsg_destroy(msg);
//                return false;
//            }
//            //std::string messageType = GetConfig()->req_type;
//            //zmsg_pushstr(*msg, messageType.c_str());
//
//            int rc = zmsg_send(msg, _port_socket);
//            return rc==0;
//        }

//        bool RequestPort::Send(std::string& message) const{
//            zmsg_t* zmsg = zmsg_new();
//            zmsg_addstr(zmsg, message.c_str());
//
//            return Send(&zmsg);
//        }
//
//        bool RequestPort::Send(std::vector<std::string>& fields) const{
//            zmsg_t* zmsg = zmsg_new();
//
//            for (auto it = fields.begin(); it!=fields.end(); it++){
//                zmsg_addstr(zmsg, it->c_str());
//            }
//
//            return Send(&zmsg);
//        }
    }
}

//namespace riaps {
//
//    RequestPort::RequestPort(request_conf& config) {
//        _config = config;
//
//
//        port_socket = zsock_new(ZMQ_REQ);
//    }
//
//    service_details RequestPort::GetRemoteService(request_conf& config) {
//        // Get the target service setails
//        bool service_available = false;
//        service_details target_service;
//        while (!service_available) {
//
//            std::vector<service_details> results;
//            get_servicebyname(config.remoteservice_name, results);
//
//            // Wait if the service is not available
//            if (!results.empty()) {
//                target_service = results.front();
//                service_available = true;
//            } else {
//                zclock_sleep(SERVICE_POLLING_INTERVAL);
//            }
//        }
//
//        return target_service;
//    }
//
//    // TODO: possible memory leak, return something else
//    // TODO: destroy socket... Maybe replacing REQ/REP with REQ/ROUTER???
//    zmsg_t* RequestPort::SendMessage(zmsg_t **msg) {
//        //std::cout << "Request port sends message " << std::endl;
//        service_details target_service = GetRemoteService(_config);
//        std::string targetaddress = "tcp://" + target_service.ip_address + ":" + target_service.port;
//        zsock_connect(port_socket, targetaddress.c_str());
//
//
//        zmsg_send(msg, port_socket);
//        //std::cout << "Request port waits for results" << std::endl;
//        zmsg_t* result = zmsg_recv(port_socket);
//        //std::cout << "Results arrived from response port" << std::endl;
//
//        zsock_disconnect(port_socket, targetaddress.c_str());
//
//        return result;
//    }
//
//    RequestPort::~RequestPort() {}
//}