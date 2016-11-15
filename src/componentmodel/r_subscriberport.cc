//
// Created by parallels on 9/6/16.
//

#include "componentmodel/r_subscriberport.h"

namespace riaps{

    SubscriberPort::SubscriberPort(_component_port_sub_j& config, const ComponentBase* component) {
        _config = config;
        _parent_component = component;
        _port_socket = zsock_new(ZMQ_SUB);
    }

    void SubscriberPort::Init() {
        auto results =
                subscribe_to_service(_parent_component->GetActor()->GetApplicationName(),
                             _parent_component->GetConfig().component_name,
                             Kind::SUB,
                             Scope::GLOBAL, //TODO: pass in argument
                             _config.subscriber_name,
                             _config.message_type);

        for (auto result : results){
            std::string endpoint = "tcp://" + result.host_name + ":" + std::to_string(result.port);
            int rc = zsock_connect(_port_socket, endpoint.c_str());

            if (rc!=0) {
                std::cout << "Subscriber '" + _config.subscriber_name + "' couldn't connect to " + endpoint << std::endl;
            }
            else {
                std::cout << "Subscriber connected to: " <<endpoint <<std::endl;
            }
        }
    }

    /*std::unique_ptr<SubscriberPort> SubscriberPort::InitFromServiceDetails(service_details& target_service) {
        std::unique_ptr<SubscriberPort> result(new SubscriberPort());

        std::string targetaddress = "tcp://" + target_service.ip_address + ":" + target_service.port;

        std::cout << "Subscriber connects to: " << targetaddress << std::endl;
        result->port_socket = zsock_new_sub(targetaddress.c_str(), "");

        if (result->port_socket!=NULL) {
            std::cout << "Subscriber connected" << std::endl;
        }
        else {
            std::cout << "Subscriber couldn't connect" << std::endl;
        }

        return std::move(result);
    }

    void SubscriberPort::GetRemoteServiceAsync(subscriber_conf &config, std::string asyncendpoint) {
        std::cout << "Async request sent" << std::endl;
        get_servicebyname_async(config.remoteservice_name, asyncendpoint);
    }

    service_details SubscriberPort::GetRemoteService(subscriber_conf &config) {
        // Get the target service setails
        bool service_available = false;
        service_details target_service;
        while (!service_available) {

            std::vector<service_details> results;
            get_servicebyname(config.remoteservice_name, results);

            // Wait if the service is not available
            if (!results.empty()){
                target_service = results.front();
                service_available = true;
            } else{
                zclock_sleep(SERVICE_POLLING_INTERVAL);
            }
        }

        return target_service;
    }*/

    SubscriberPort::~SubscriberPort() {

    }

}