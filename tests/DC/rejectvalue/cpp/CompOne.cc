#include <CompOne.h>

namespace rejectvalue {
   namespace components {
      
      CompOne::CompOne(_component_conf &config, riaps::Actor &actor) :
          CompOneBase(config, actor),
          m_joinedToA(false),
          m_index(0) {
          _logger->set_pattern("[%T] [%n] %v");
      }
      
      void CompOne::OnClock(riaps::ports::PortBase *port) {
         if (!m_joinedToA){
             _logger->info("Component joins to {}:{}", groupIdA.groupTypeId, groupIdA.groupName);
            auto joined = JoinGroup(groupIdA);
            if (joined){
                m_joinedToA = true;
            }
            _logger->error_if(!joined, "Couldn't join to group {}:{}", groupIdA.groupTypeId, groupIdA.groupName);
         } else {
             if (!IsLeader(groupIdA) && IsLeaderAvailable(groupIdA)) {
                 capnp::MallocMessageBuilder builder;
                 auto msg = builder.initRoot<MessageType>();
                 msg.setValue(m_index++);
                 SendPropose(groupIdA, builder);
             }
         }
      }

      void CompOne::OnPropose(riaps::groups::GroupId &groupId, const std::string &proposeId,
                              capnp::FlatArrayMessageReader &message) {
          SendVote(groupId, proposeId, true);
      }

      void CompOne::OnAnnounce(const riaps::groups::GroupId &groupId, const std::string &proposeId, bool accepted) {
          _logger->info("Propose with id {} was {} and announced", proposeId, accepted?"accepted":"rejected");
      }


   }
}

riaps::ComponentBase *create_component(_component_conf &config, riaps::Actor &actor) {
   auto result = new rejectvalue::components::CompOne(config, actor);
   return result;
}

void destroy_component(riaps::ComponentBase *comp) {
   delete comp;
}
