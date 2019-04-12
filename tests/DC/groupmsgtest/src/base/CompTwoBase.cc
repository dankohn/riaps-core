#include <base/CompTwoBase.h>
#include <componentmodel/r_pyconfigconverter.h>

using namespace std;
using namespace riaps::ports;

namespace groupmsgtest {
   namespace components {
      
      CompTwoBase::CompTwoBase(const py::object *parent_actor,
                               const py::dict actor_spec, // Actor json config
                               const py::dict type_spec,  // component json config
                               const py::list group_spec,
                               const std::string &name,
                               const std::string &type_name,
                               const py::dict args,
                               const std::string &application_name,
                               const std::string &actor_name)
              : ComponentBase(application_name, actor_name) {
          auto conf = PyConfigConverter::convert(type_spec, actor_spec, group_spec);
          conf.component_name = name;
          conf.component_type = type_name;
          conf.is_device = false;
          set_config(conf);
          set_debug_level(spd::level::debug,spd::level::debug);
         
      }

       timespec CompTwoBase::RecvClock() {
           auto port = GetPortAs<riaps::ports::PeriodicTimer>(PORT_TIMER_CLOCK);
           return port->Recv();
       }

       void CompTwoBase::SendGroupMessage(
               riaps::groups::GroupId& group_id,
               MessageBuilder<groupmsgtest::messages::MessageType> &message) {
           capnp::MallocMessageBuilder& builder = message.capnp_builder();
           PassGroupMessage(group_id, builder);
       }

       void CompTwoBase::DispatchMessage(riaps::ports::PortBase *port) {
           auto port_name = port->port_name();
           if (port_name == PORT_TIMER_CLOCK) {
               OnClock();
           }
       }

       void CompTwoBase::DispatchInsideMessage(zmsg_t* zmsg, riaps::ports::PortBase* port) {

       }
   }
}
