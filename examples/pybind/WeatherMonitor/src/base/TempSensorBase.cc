//
// Auto-generated by edu.vanderbilt.riaps.generator.ComponenetGenerator.xtend
//
#include <base/TempSensorBase.h>
#include <componentmodel/r_pyconfigconverter.h>

using namespace std;
using namespace riaps;

namespace weathermonitor {
    namespace components {
    	
    	TempSensorBase::TempSensorBase(const py::object *parent_actor,
									   const py::dict    actor_spec, // Actor json config
									   const py::dict    type_spec,  // component json config
									   const std::string &name,
									   const std::string &type_name,
									   const py::dict    args,
									   const std::string &application_name,
									   const std::string &actor_name): ComponentBase(application_name, actor_name) {
			auto conf = PyConfigConverter::convert(type_spec, actor_spec);
			conf.component_name = name;
			conf.component_type = type_name;
			conf.is_device=false;
			set_config(conf);
    	}
    	
    	void TempSensorBase::DispatchMessage(riaps::ports::PortBase*   port) {
    		auto portName = port->port_name();
			if (portName == PORT_TIMER_CLOCK) {
				OnClock();
			}
    	}

    	timespec TempSensorBase::RecvClock() {
            auto port = GetPortAs<riaps::ports::PeriodicTimer>(PORT_TIMER_CLOCK);
            return port->Recv();
    	}
    	
    	bool TempSensorBase::SendReady(MessageBuilder<weathermonitor::messages::TempData>& message) {
    	    return SendMessageOnPort(message.capnp_builder(), PORT_PUB_READY);
    	}

        void TempSensorBase::DispatchInsideMessage(zmsg_t* zmsg, riaps::ports::PortBase* port) {

    	}
    }
}	
