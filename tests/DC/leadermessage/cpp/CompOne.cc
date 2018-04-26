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
                 if (SendMessageToLeader(groupIdA, builder)){
                    _logger->info("Message sent to leader. Content: {}", msg.getMsg().cStr());
                 } else {
                     _logger->warn("Couldn't send message to leader.");
                 }
             }
         }
      }

      void CompOne::OnMessageToLeader(const riaps::groups::GroupId &groupId, capnp::FlatArrayMessageReader &message) {
          _logger->info("Messaged arrived from non-leader {}", message.getRoot<MessageType>().getMsg().cStr());
          capnp::MallocMessageBuilder builder;
          auto msg = builder.initRoot<MessageType>();
          msg.setMsg(GetLeaderId(groupId));
          _logger->error_if(!SendLeaderMessage(groupId, builder), "Couldn't send message to non leaders");
      }

      void CompOne::OnMessageFromLeader(const riaps::groups::GroupId &groupId, capnp::FlatArrayMessageReader &message) {
          std::string content = message.getRoot<MessageType>().getMsg();
          if (content == GetLeaderId(groupId))
            _logger->info("Message returned from the leader, content: {}", content);
          else
            _logger->error("Non-leader message arrived in {}", __FUNCTION__);
      }
    
      
      void CompOne::OnGroupMessage(const riaps::groups::GroupId& groupId, capnp::FlatArrayMessageReader& capnpreader, riaps::ports::PortBase* port){
      }
      
      CompOne::~CompOne() {
         
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
