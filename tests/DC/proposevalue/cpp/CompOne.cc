#include <CompOne.h>

namespace leadermessage {
   namespace components {
      
      CompOne::CompOne(_component_conf &config, riaps::Actor &actor) :
          CompOneBase(config, actor),
          m_joinedToA(false){
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
             if (!IsLeader(groupIdA)) {
                 capnp::MallocMessageBuilder builder;
                 auto msg = builder.initRoot<MessageType>();
                 msg.setMsg(fmt::format("{}", GetCompUuid()));
                 _logger->info("Propose sent with id: {}", SendPropose(groupIdA, builder));
             }
         }
      }

      void CompOne::OnPropose(riaps::groups::GroupId &groupId, const std::string &proposeId,
                              capnp::FlatArrayMessageReader &message) {

      }

      void CompOne::OnAnnounce(const riaps::groups::GroupId &groupId, const std::string &proposeId, bool accepted) {

      }


   }
}

riaps::ComponentBase *create_component(_component_conf &config, riaps::Actor &actor) {
   auto result = new leadermessage::components::CompOne(config, actor);
   return result;
}

void destroy_component(riaps::ComponentBase *comp) {
   delete comp;
}