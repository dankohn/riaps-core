//
// Created by parallels on 9/6/16.
//

#ifndef RIAPS_R_SUBSCRIBERPORT_H
#define RIAPS_R_SUBSCRIBERPORT_H

#include <memory>
#include <componentmodel/r_componentbase.h>
#include <componentmodel/r_configuration.h>
#include <componentmodel/ports/r_subportbase.h>

namespace riaps {

    class ComponentBase;

    namespace ports {
        class SubscriberPort : public SubscriberPortBase {
        public:

            SubscriberPort(const ComponentPortSub &config,
                           bool has_security,
                           const std::string& component_name,
                           const std::string& application_name,
                           const std::string& actor_name,
                           std::shared_ptr<spd::logger>& logger);
            virtual void Init();
            virtual ~SubscriberPort() = default;
        };
    }

}

#endif //RIAPS_R_SUBSCRIBERPORT_H
