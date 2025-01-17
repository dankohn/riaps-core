//
// Created by istvan on 3/10/17.
//

#include <base/GlobalEstimatorBase.h>
#include <componentmodel/r_pyconfigconverter.h>

using namespace std;
using namespace riaps::ports;

namespace distributedestimator {
    namespace components {
        GlobalEstimatorBase::GlobalEstimatorBase(const py::object *parent_actor,
                                                 const py::dict actor_spec, // Actor json config
                                                 const py::dict type_spec,  // component json config
                                                 const std::string &name,
                                                 const std::string &type_name,
                                                 const py::dict args,
                                                 const std::string &application_name,
                                                 const std::string &actor_name)
                : ComponentBase(application_name, actor_name) {

            auto conf = PyConfigConverter::convert(type_spec, actor_spec, args);
            conf.component_name = name;
            conf.component_type = type_name;
            conf.is_device=false;
            set_config(conf);
            set_debug_level(spd::level::info);
        }

        timespec GlobalEstimatorBase::RecvWakeup() {
            auto port = GetPortAs<riaps::ports::PeriodicTimer>(PORT_TIMER_WAKEUP);
            return port->Recv();
        }

        tuple<MessageReader<messages::Estimate>, PortError> GlobalEstimatorBase::RecvEstimate() {
            auto port = GetPortAs<riaps::ports::SubscriberPort>(PORT_SUB_ESTIMATE);
            auto [msg_bytes, error] = port->Recv();
            MessageReader<messages::Estimate> reader(msg_bytes);
            return make_tuple(reader, error);
        }

        void GlobalEstimatorBase::DispatchMessage(riaps::ports::PortBase *port) {
            auto port_name = port->port_name();
            if (port_name == PORT_TIMER_WAKEUP) {
                OnWakeup();
            } else if (port_name == PORT_SUB_ESTIMATE) {
                OnEstimate();
            }
        }

        void GlobalEstimatorBase::DispatchInsideMessage(zmsg_t *zmsg, riaps::ports::PortBase *port) {

        }
    }
}