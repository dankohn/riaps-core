//
// Auto-generated by edu.vanderbilt.riaps.generator.ComponenetGenerator.xtend
//
#ifndef RIAPS_FW_TEMPSENSOR_H
#define RIAPS_FW_TEMPSENSOR_H

#include "base/TempSensorBase.h"

namespace weathermonitor {
    namespace components {

        class TempSensor : public TempSensorBase {

        public:

            TempSensor(const py::object *parent_actor,
					   const py::dict actor_spec, // Actor json config
					   const py::dict type_spec,  // component json config
					   const std::string &name,
					   const std::string &type_name,
					   const py::dict args,
					   const std::string &application_name,
					   const std::string &actor_name);

            virtual void OnClock();

            virtual ~TempSensor() = default;

		private:
			double temperature;
        };
    }
}

std::unique_ptr<weathermonitor::components::TempSensor>
create_component_py(const py::object *parent_actor,
					const py::dict actor_spec,
					const py::dict type_spec,
					const std::string &name,
					const std::string &type_name,
					const py::dict args,
					const std::string &application_name,
					const std::string &actor_name);


#endif //RIAPS_FW_TEMPSENSOR_H		
