//
// Created by istvan on 10/13/17.
//

#ifndef RIAPS_CORE_R_SUBPORTBASE_H
#define RIAPS_CORE_R_SUBPORTBASE_H

#include <componentmodel/ports/r_portbase.h>

#include <string>
#include "r_recvport.h"

namespace riaps{
    namespace ports{
        class SubscriberPortBase : public PortBase, public RecvPort{
        public:
            SubscriberPortBase(const ComponentPortConfig* config,
                               const std::string& application_name,
                               const std::string& actor_name,
                               const std::string& component_name,
                               bool has_security);

            virtual const ComponentPortSub* GetConfig() const;

            // Returns false, if the subscriber couldn't connect
            bool ConnectToPublihser(const std::string& endpoint);

            virtual ~SubscriberPortBase();

        protected:
            std::vector<std::string> endpoints_;

            virtual void InitSocket();
        };
    }
}

#endif //RIAPS_CORE_R_SUBPORTBASE_H
