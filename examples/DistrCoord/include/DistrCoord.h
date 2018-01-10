//
// Created by istvan on 11/11/16.
//

#ifndef RIAPS_FW_SENSOR_H
#define RIAPS_FW_SENSOR_H

#include "base/DistrCoordBase.h"

namespace dc {
    namespace components {

        class DistrCoord : public DistrCoordBase {

        public:

            DistrCoord(_component_conf &config, riaps::Actor &actor);
            virtual ~DistrCoord();

        protected:

            virtual void OnClock(riaps::ports::PortBase *port);

            void OnGroupMessage(const riaps::groups::GroupId& groupId,
                                capnp::FlatArrayMessageReader& capnpreader,
                                riaps::ports::PortBase* port);

            virtual bool SendGroupMessage(riaps::groups::GroupId&      groupId,
                                          capnp::MallocMessageBuilder& messageBuilder,
                                          const std::string&           portName);
        private:
            bool _hasJoined;

            std::random_device _rd;
            std::mt19937       _generator;
            std::uniform_int_distribution<int> _distr;
        };
    }
}

extern "C" riaps::ComponentBase* create_component(_component_conf&, riaps::Actor& actor);
extern "C" void destroy_component(riaps::ComponentBase*);


#endif //RIAPS_FW_SENSOR_H
