//
// Created by istvan on 10/4/17.
//

#ifndef RIAPS_CORE_R_PUBLISHERPORTBASE_H
#define RIAPS_CORE_R_PUBLISHERPORTBASE_H

#include <const/r_const.h>
#include <componentmodel/ports/r_portbase.h>
#include <componentmodel/ports/r_senderport.h>

namespace riaps{
    namespace ports{
        class PublisherPortBase : public PortBase, public SenderPort {
        public:
            PublisherPortBase(const ComponentPortConfig* config,
                              bool has_security,
                              const std::string& component_name,
                              const std::string& application_name,
                              const std::string& actor_name,
                              std::shared_ptr<spd::logger>& logger);
            std::string endpoint();

            virtual const ComponentPortPub* GetConfig() const;
            virtual ~PublisherPortBase() = default;

        protected:
            int port_;
            std::string host_;
            std::shared_ptr<zactor_t> auth_;
            virtual void InitSocket();
        };
    }
}

#endif //RIAPS_CORE_R_PUBLISHERPORTBASE_H
