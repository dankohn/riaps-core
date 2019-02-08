//
// Created by parallels on 9/6/16.
//

#ifndef RIAPS_R_PUBLISHERPORT_H
#define RIAPS_R_PUBLISHERPORT_H

#include <componentmodel/r_configuration.h>
#include <componentmodel/ports/r_pubportbase.h>

#include <czmq.h>

#include <string>

namespace riaps {

    class ComponentBase;

    namespace ports {

       class PublisherPort : public PublisherPortBase {
       public:
           /**
            * Component publisher port, the port registers itself in the discovery service
            * @param config
            * @param parent_component
            */
           PublisherPort(const ComponentPortPub& config,
                         bool has_security,
                         const std::string& component_name,
                         const std::string& application_name,
                         const std::string& actor_name,
                         std::shared_ptr<spd::logger>& logger);
           ~PublisherPort() = default;
       };
   }
}

#endif //RIAPS_R_PUBLISHERPORT_H
