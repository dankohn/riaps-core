//
// Auto-generated by edu.vanderbilt.riaps.generator.ComponenetGenerator.xtend
//
#include <base/TempSensorBase.h>

namespace weathermonitor {
    namespace components {
    	
    	TempSensorBase::TempSensorBase(_component_conf_j &config, riaps::Actor &actor) : ComponentBase(config, actor) {
    	
    	}
    	
    	void TempSensorBase::DispatchMessage(capnp::FlatArrayMessageReader* capnpreader, riaps::ports::PortBase *port) {
			std::cout << "TempSensorBase::DispatchMessage()" << std::endl;
    		auto portName = port->GetPortName();
			if (portName == PORT_TIMER_CLOCK) {
				OnClock(port);
			}
			
			
    	}
    	
    	bool TempSensorBase::SendReady(capnp::MallocMessageBuilder &messageBuilder, messages::TempData::Builder &message) {
    		std::cout<< "TempSensorBase::SendReady()"<< std::endl;
    	    return SendMessageOnPort(messageBuilder, PORT_PUB_READY);
    	}
    	
    	TempSensorBase::~TempSensorBase() {
    	
    	}
    }
}	
