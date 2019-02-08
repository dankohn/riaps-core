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
            GroupSubscriberPort(const ComponentPortSub &config,
                                bool has_security,
                                const std::string& component_name,
                                const std::string& application_name,
                                const std::string& actor_name,
                                std::shared_ptr<spd::logger>& logger);
            virtual ~GroupSubscriberPort() = default;

        private:
            // The group configurations may dynamic (not from the model file)
            // GroupPorts store the original config.
            const ComponentPortSub group_port_config_;
        };
    }
}

#endif //RIAPS_CORE_R_SUBPORTGROUP_H
