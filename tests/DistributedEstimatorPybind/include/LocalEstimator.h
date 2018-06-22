//
// Created by istvan on 11/11/16.
//

#ifndef RIAPS_FW_LOCALESTIMATOR_H
#define RIAPS_FW_LOCALESTIMATOR_H


#include "base/LocalEstimatorBase.h"

namespace distributedestimator {
    namespace components {

        class LocalEstimator : public LocalEstimatorBase {

        public:

            LocalEstimator(const py::object *parent_actor, const py::dict type_spec, const std::string &name,
                           const std::string &type_name, const py::dict args,
                           const std::string &application_name, const std::string &actor_name);

            virtual void OnReady(const messages::SensorReady::Reader &message,
                                 riaps::ports::PortBase *port);

            virtual ~LocalEstimator();

        private:
            std::unique_ptr<std::uniform_real_distribution<double>> unif;
            std::default_random_engine re;
        };
    }
}

//extern "C" riaps::ComponentBase* create_component(_component_conf&, riaps::Actor& actor);
//extern "C" void destroy_component(riaps::ComponentBase*);


#endif //RIAPS_FW_LOCALESTIMATOR_H