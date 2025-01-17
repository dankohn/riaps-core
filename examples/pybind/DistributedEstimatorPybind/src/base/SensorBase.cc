//
// Created by istvan on 3/10/17.
//

#include <base/SensorBase.h>
#include <componentmodel/r_pyconfigconverter.h>

using namespace std;
using namespace riaps::ports;

namespace distributedestimator {
    namespace components {

        SensorBase::SensorBase(const py::object *parent_actor,
                               const py::dict actor_spec, // Actor json config
                               const py::dict type_spec,  // component json config
                               const std::string &name,
                               const std::string &type_name,
                               const py::dict args,
                               const std::string &application_name,
                               const std::string &actor_name)
                : ComponentBase(application_name, actor_name) {
            auto config = PyConfigConverter::convert(type_spec, actor_spec, args);
            config.component_name = name;
            config.component_type = type_name;
            config.is_device=false;
            set_config(config);
            set_debug_level(spd::level::info);
        }

        timespec SensorBase::RecvClock() {
            auto port = GetPortAs<riaps::ports::PeriodicTimer>(PORT_TIMER_CLOCK);
            return port->Recv();
        }

        tuple<MessageReader<messages::SensorQuery>, PortError> SensorBase::RecvRequest() {
            auto port = GetPortAs<riaps::ports::ResponsePort>(PORT_REP_REQUEST);
            auto [msg_bytes, error] = port->Recv();
            MessageReader<messages::SensorQuery> reader(msg_bytes);
            return make_tuple(reader, error);
        }

        PortError SensorBase::SendRequest(MessageBuilder<messages::SensorValue>& message) {
            return SendMessageOnPort(message.capnp_builder(), PORT_REP_REQUEST);
        }

        PortError SensorBase::SendReady(MessageBuilder<messages::SensorReady>& builder) {
            return SendMessageOnPort(builder.capnp_builder(), PORT_PUB_READY);
        }

        void SensorBase::DispatchMessage(riaps::ports::PortBase* port) {
            auto portName = port->port_name();
            if (portName == PORT_TIMER_CLOCK) {
                OnClock();
            } else if (portName == PORT_REP_REQUEST) {
                OnRequest();
            }
        }

        void SensorBase::DispatchInsideMessage(zmsg_t *zmsg, riaps::ports::PortBase *port) {

        }
    }
}