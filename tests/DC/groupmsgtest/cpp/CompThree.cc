#include <CompThree.h>

namespace groupmsgtest {
   namespace components {
      
      CompThree::CompThree(_component_conf &config, riaps::Actor &actor) :
      CompThreeBase(config, actor) {
      }
      
      void CompThree::OnClock(riaps::ports::PortBase *port) {
         if (!m_joinedToB){
             _logger->info("Component joins to {}:{}", groupIdB.groupTypeId, groupIdB.groupName);
            auto joined = JoinToGroup(groupIdB);
            if (joined){
                m_joinedToB = true;
            }
            _logger->error_if(!joined, "Couldn't join to group {}:{}", groupIdB.groupTypeId, groupIdB.groupName);
         }
      }
      
      void CompThree::OnGroupMessage(const riaps::groups::GroupId& groupId,
      capnp::FlatArrayMessageReader& capnpreader, riaps::ports::PortBase* port){
         
      }
      
      CompThree::~CompThree() {
         
      }
   }
}

riaps::ComponentBase *create_component(_component_conf &config, riaps::Actor &actor) {
   auto result = new groupmsgtest::components::CompThree(config, actor);
   return result;
}

void destroy_component(riaps::ComponentBase *comp) {
   delete comp;
}
