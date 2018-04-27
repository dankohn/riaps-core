//
// Auto-generated by edu.vanderbilt.riaps.generator.ComponenetGenerator.xtend
//
#include <CompOneBase.h>

namespace reelectleader {
   namespace components {
      
      CompOneBase::CompOneBase(_component_conf &config, riaps::Actor &actor) : ComponentBase(config, actor) {
         
      }
      
      void CompOneBase::DispatchMessage(capnp::FlatArrayMessageReader* capnpreader, riaps::ports::PortBase *port,std::shared_ptr<riaps::MessageParams> params) {
         auto portName = port->GetPortName();
         if (portName == PORT_TIMER_CLOCK) {
            OnClock(port);
         }
         
      }
      
      void CompOneBase::DispatchInsideMessage(zmsg_t* zmsg, riaps::ports::PortBase* port) {
         //empty the header
      }
      
      
      CompOneBase::~CompOneBase() {
         
      }
   }
}
