//
// Created by parallels on 9/7/16.
//

#include <framework/rfw_security.h>
#include <componentmodel/r_configuration.h>
#include <componentmodel/ports/r_portbase.h>
#include <componentmodel/r_componentbase.h>
#include <capnp/message.h>

using namespace std;

namespace riaps {
    namespace ports {

        PortBase::PortBase(PortTypes port_type,
                           const ComponentPortConfig* config,
                           bool has_security,
                           const std::string& component_name,
                           const std::string& application_name,
                           const std::string& actor_name,
                           std::shared_ptr<spd::logger>& logger)
                           : port_type_(port_type),
                             has_security_(has_security),
                             component_name_(component_name),
                             application_name_(application_name),
                             actor_name_(actor_name),
                             logger_(logger) {

            config_           = config;
            port_socket_      = nullptr;
            port_certificate_ = nullptr;

            if (this->has_security()) {
                zcert_t* curve_key = riaps::framework::Security::curve_key();
                if (curve_key == nullptr)
                    this->logger()->error("Cannot open CURVE key: {}", riaps::framework::Security::curve_key_path());
                else {
                    port_certificate_ = shared_ptr<zcert_t>(curve_key, [](zcert_t* c){
                        zcert_destroy(&c);
                    });
                }
            }
        }

        bool PortBase::has_security() const {
            return has_security_;
        }

        shared_ptr<spd::logger> PortBase::logger() const {
//            // InsidePorts have no parent components
//            if (component_name_ == "") {
//                string logger_prefix = port_type_ == PortTypes::Inside?"InsidePort":"NullParent";
//                string logger_name = fmt::format("{}::{}", logger_prefix, config_->port_name);
//                if (spd::get(logger_name) == nullptr) {
//                    return spd::stdout_color_mt(logger_name);
//                }
//                return spd::get(logger_name);
//            }
           return this->logger_;
        }

        const zsock_t *PortBase::port_socket() const {
            return port_socket_;
        }

        const ComponentPortConfig* PortBase::config() const {
            return config_;
        }


        const PortTypes& PortBase::port_type() const {
            return port_type_;
        }

        const std::string PortBase::port_name() const {
            return config_->port_name;
        }

        RequestPort* PortBase::AsRequestPort()  {
            return GetPortAs<RequestPort>();
        }

        PublisherPort* PortBase::AsPublishPort()  {
            return GetPortAs<PublisherPort>();
        }

        GroupPublisherPort* PortBase::AsGroupPublishPort() {
            return GetPortAs<GroupPublisherPort>();
        }

        GroupSubscriberPort* PortBase::AsGroupSubscriberPort() {
            return GetPortAs<GroupSubscriberPort>();
        }

        ResponsePort* PortBase::AsResponsePort()  {
            return GetPortAs<ResponsePort>();
        }

        SubscriberPort* PortBase::AsSubscribePort()  {
            return GetPortAs<SubscriberPort>();
        }

        PeriodicTimer* PortBase::AsTimerPort()  {
            return GetPortAs<PeriodicTimer>();
        }

        InsidePort* PortBase::AsInsidePort()  {
            return GetPortAs<InsidePort>();
        }

        QueryPort* PortBase::AsQueryPort() {
            return GetPortAs<QueryPort>();
        }

        AnswerPort* PortBase::AsAnswerPort() {
            return GetPortAs<AnswerPort>();
        }

        RecvPort* PortBase::AsRecvPort() {
            return GetPortAs<RecvPort>();
        }

        PortBase::~PortBase() {
            if (port_socket_) {
                zsock_destroy(&port_socket_);
            }
        }
    }
}