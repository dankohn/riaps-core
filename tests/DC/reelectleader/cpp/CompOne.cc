#include <CompOne.h>

using namespace std;

namespace reelectleader {
    namespace components {

        CompOne::CompOne(_component_conf &config, riaps::Actor &actor) :
                CompOneBase(config, actor),
                m_joinedToA(false),
                m_timeout(nullptr){
            _logger->set_pattern("[%T] [%n] %v");
        }

        void CompOne::OnClock(riaps::ports::PortBase *port) {
            if (!m_joinedToA) {
                _logger->info("Component joins to {}:{}", groupIdA.groupTypeId, groupIdA.groupName);
                auto joined = JoinGroup(groupIdA);
                if (joined) {
                    m_joinedToA = true;
                }
                _logger->error_if(!joined, "Couldn't join to group {}:{}", groupIdA.groupTypeId, groupIdA.groupName);
            } else if (IsMemberOf(groupIdA)){
                // if the current component is the leader
                if (IsLeader(groupIdA)) {
                    // Thel eader has just elected, create the timer.
                    if (m_timeout == nullptr) {
                        m_timeout = std::shared_ptr<Timeout<std::ratio<1>>>(new Timeout<std::ratio<1>>(duration<int, std::ratio<1>>(20)));
                    }

                    // The leader elected previously, check the timer, leave the group if 20 seconds has reached
                    else {
                        if (m_timeout->IsTimeout()) {
                            LeaveGroup(groupIdA);
                            m_timeout.reset();
                        }
                    }
                }

                _logger->info("The leader is: {}", GetLeaderId(groupIdA));
            }


        }


        void CompOne::OnGroupMessage(const riaps::groups::GroupId &groupId, capnp::FlatArrayMessageReader &capnpreader,
                                     riaps::ports::PortBase *port) {
        }

        CompOne::~CompOne() {

        }
    }
}

riaps::ComponentBase *create_component(_component_conf &config, riaps::Actor &actor) {
    auto result = new reelectleader::components::CompOne(config, actor);
    return result;
}

void destroy_component(riaps::ComponentBase *comp) {
    delete comp;
}
