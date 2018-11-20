//
// Created by istvan on 10/17/17.
//

#ifndef RIAPS_CORE_R_SUBPORTGROUP_H
#define RIAPS_CORE_R_SUBPORTGROUP_H

#include <componentmodel/ports/r_subportbase.h>

namespace riaps{
    namespace ports{
        class GroupSubscriberPort : public SubscriberPortBase {
        public:
            GroupSubscriberPort(const component_port_sub &config, const ComponentBase* parentComponent);
            virtual ~GroupSubscriberPort() = default;

        private:
            // The group configurations may dynamic (not from the model file)
            // GroupPorts store the original config.
            const component_port_sub group_port_config_;
        };
    }
}

#endif //RIAPS_CORE_R_SUBPORTGROUP_H
