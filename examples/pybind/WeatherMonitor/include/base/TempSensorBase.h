//
// Auto-generated by edu.vanderbilt.riaps.generator.ComponenetGenerator.xtend
//

#ifndef RIAPS_CORE_TEMPSENSOR_H
#define RIAPS_CORE_TEMPSENSOR_H

#include <componentmodel/r_messagebuilder.h>
#include <componentmodel/r_componentbase.h>
#include "messages/TempData.capnp.h"

// Name of the ports from the model file
constexpr auto PORT_TIMER_CLOCK = "clock";
constexpr auto PORT_PUB_READY   = "ready";

#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace weathermonitor {
    namespace components {
    	
    	class TempSensorBase : public riaps::ComponentBase {
    		
    	public:
    		TempSensorBase(const py::object *parent_actor,
						   const py::dict actor_spec, // Actor json config
						   const py::dict type_spec,  // component json config
						   const std::string &name,
						   const std::string &type_name,
						   const py::dict args,
						   const std::string &application_name,
						   const std::string &actor_name);
    		
    		virtual void OnClock()=0;
            virtual std::string RecvClock() final;
    		
    		virtual bool SendReady(MessageBuilder<weathermonitor::messages::TempData>& message);
    		
    	    virtual ~TempSensorBase() = default;
    	protected:
			virtual void DispatchMessage(riaps::ports::PortBase* port) final;

			virtual void DispatchInsideMessage(zmsg_t* zmsg,
											   riaps::ports::PortBase* port) final;
        };
    }
}
#endif //RIAPS_CORE_TEMPSENSOR_H	
