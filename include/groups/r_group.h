//
// Created by istvan on 9/27/17.
//

#ifndef RIAPS_CORE_R_GROUP_H_H
#define RIAPS_CORE_R_GROUP_H_H

#include <componentmodel/r_actor.h>
#include <componentmodel/r_configuration.h>
#include <componentmodel/ports/r_pubportgroup.h>
#include <componentmodel/ports/r_subportgroup.h>
#include <messaging/disco.capnp.h>
#include <messaging/distcoord.capnp.h>

#include <spdlog/spdlog.h>
#include <msgpack.hpp>
#include <czmq.h>

#include <string>
#include <vector>
#include <map>
#include <random>

#define INTERNAL_SUB_NAME "$SUB#"
#define INTERNAL_PUB_NAME "$PUB#"
#define INTERNAL_MESSAGETYPE "InternalGroupMessage"

namespace spd = spdlog;

namespace riaps {
    namespace groups {

        /**
         * The instance name of the group and the group type id (form the config) are the GroupId
         */
        struct GroupId {
            std::string groupTypeId;
            std::string groupName;


            /**
             * To use GroupId in std::map as key
             * @param other
             * @return
             */
            bool operator<(const GroupId& other) const;


            MSGPACK_DEFINE(groupName, groupTypeId);
        };

        /**
         * Description of one port in the group.
         */
        struct GroupService {
            std::string messageType;
            std::string address;
            MSGPACK_DEFINE(messageType, address);
        };

        /**
         * All the details of the group in one structure.
         */
        struct GroupDetails {
            std::string               appName;
            std::string               componentId;
            GroupId                   groupId;
            std::vector<GroupService> groupServices;
            MSGPACK_DEFINE(appName, componentId, groupId, groupServices);

        };

        /**
         * Encapsulates a RIAPS Group.
         *  - Creates/releases/stores communication ports
         *  - Registers group in the discovery service
         *  - Receives messages
         *  - Sends messages
         */
        class Group final {
        public:

            /**
             * Initializes a group, by the given groupId
             * @param groupId Must have valid configuration entry with the matching id.
             */
            Group(const GroupId& groupId, const ComponentBase* parentComponent);

            /**
             * Creates the communication ports and registers the group in the discovery service.
             * @return true if the ports were succesfully created and registered False otherwise.
             */
            bool InitGroup();

            void ConnectToNewServices(riaps::discovery::GroupUpdate::Reader& msgGroupUpdate);

            bool SendMessage(capnp::MallocMessageBuilder& message, const std::string& portName);

            ports::GroupSubscriberPort* FetchNextMessage(std::shared_ptr<capnp::FlatArrayMessageReader>& messageReader);



            bool SendPingWithPeriod();
            bool SendPing();
            bool SendPong();
            
            std::shared_ptr<std::vector<std::string>> GetKnownComponents();

            uint16_t GetMemberCount(uint16_t timeout /*msec*/) const;

            ~Group();

        private:
            bool SendHeartBeat(riaps::distrcoord::HeartBeatType type);

            const GroupId     _groupId;
            groupt_conf       _groupTypeConf;

            /**
             * Always store the communication ports in unique_ptr (self-defense)
             */
            std::shared_ptr<riaps::ports::GroupPublisherPort>    _groupPubPort;
            std::shared_ptr<riaps::ports::GroupSubscriberPort>   _groupSubPort;

            std::map<const zsock_t*, std::shared_ptr<riaps::ports::PortBase>> _groupPorts;

            /**
             * List of nodes who was a sender with timestamp (last known timepoint).
             *  key   - component id (uuid, generated runtime, when the component starts
             *  value - timepoint
             */
            std::unordered_map<std::string, int64_t> _knownNodes;

            zframe_t*  _lastFrame;
            zpoller_t* _groupPoller;
            std::shared_ptr<capnp::FlatArrayMessageReader> _lastReader;

            std::shared_ptr<spd::logger> _logger;

            int64_t  _lastPingSent;
            float    _pingPeriod;

            std::default_random_engine         rndGenerator;
            std::uniform_int_distribution<int> rndDistribution;

            const ComponentBase* _parentComponent;

            // debug
            uint16_t _pingCounter;
        };

    }
}
#endif //RIAPS_CORE_R_GROUP_H_H