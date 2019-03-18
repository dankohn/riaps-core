#ifndef RIAPS_FW_R_RESPONSEPORT_H
#define RIAPS_FW_R_RESPONSEPORT_H

#include <componentmodel/ports/r_portbase.h>
#include "r_recvport.h"
#include <componentmodel/r_componentbase.h>
#include <componentmodel/r_configuration.h>

#include "czmq.h"

#include <string>

namespace riaps {

    class ComponentBase;

    namespace ports {

        class ResponsePort : public PortBase, public SenderPort, public RecvPort {
        public:
            ResponsePort(const ComponentPortRep &config,
                         const std::string& application_name,
                         const std::string& actor_name,
                         const std::string& component_name,
                         bool has_security);
            virtual const ComponentPortRep* GetConfig() const;
            ~ResponsePort() noexcept ;
        protected:
            int port_;
            std::string host_;
            std::string endpoint_;
            std::shared_ptr<zactor_t> auth_;
        };
    }
}

#endif //RIAPS_FW_R_RESPONSEPORT_H
