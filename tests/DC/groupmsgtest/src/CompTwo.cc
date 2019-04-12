#include <CompTwo.h>

namespace groupmsgtest {
   namespace components {
      
      CompTwo::CompTwo(const py::object *parent_actor,
                       const py::dict actor_spec, // Actor json config
                       const py::dict type_spec,  // component json config
                       const py::list group_spec,
                       const std::string &name,
                       const std::string &type_name,
                       const py::dict args,
                       const std::string &application_name,
                       const std::string &actor_name) :
              CompTwoBase(parent_actor, actor_spec, type_spec, group_spec, name, type_name, args, application_name,
                          actor_name), m_joinedToA(false), m_joinedToB(false) {
          component_logger()->set_pattern("[%n] %v");
      }
      
      void CompTwo::OnClock() {
          auto now = RecvClock();
         if (!m_joinedToA){
             component_logger()->info("Component joins to {}:{}", groupIdA.group_type_id, groupIdA.group_name);
            auto joined = JoinGroup(groupIdA);
            if (joined){
                m_joinedToA = true;
            } else {
                component_logger()->error("Couldn't join to group {}:{}", groupIdA.group_type_id, groupIdA.group_name);

            }
         }

//         if (!m_joinedToB){
//             component_logger()->info("Component joins to {}:{}", groupIdB.group_type_id, groupIdB.groupName);
//            auto joined = JoinGroup(groupIdB);
//            if (joined){
//                m_joinedToB = true;
//            }
//             component_logger()->error_if(!joined, "Couldn't join to group {}:{}", groupIdB.group_type_id, groupIdB.groupName);
//         }
      }
      
//      void CompTwo::OnGroupMessage(const riaps::groups::GroupId& groupId,
//      capnp::FlatArrayMessageReader& capnpreader, riaps::ports::PortBase* port){
//
//          logGroupMessage(_logger, __FUNCTION__, groupId, capnpreader.getRoot<MessageType>().getMsg());
//      }
   }
}

std::unique_ptr<groupmsgtest::components::CompTwo>
create_component_py(const py::object *parent_actor,
                    const py::dict actor_spec,
                    const py::dict type_spec,
                    const py::list group_spec,
                    const std::string &name,
                    const std::string &type_name,
                    const py::dict args,
                    const std::string &application_name,
                    const std::string &actor_name) {
    auto ptr = new groupmsgtest::components::CompTwo(parent_actor, actor_spec, type_spec, group_spec, name, type_name, args,
                                                     application_name,
                                                     actor_name);
    return std::move(std::unique_ptr<groupmsgtest::components::CompTwo>(ptr));
}

PYBIND11_MODULE(libcomptwo, m) {
    py::class_<groupmsgtest::components::CompTwo> testClass(m, "CompTwo");
    testClass.def(py::init<const py::object*, const py::dict, const py::dict, const py::list, const std::string&, const std::string&, const py::dict, const std::string&, const std::string&>());

    testClass.def("setup"                 , &groupmsgtest::components::CompTwo::Setup                );
    testClass.def("activate"              , &groupmsgtest::components::CompTwo::Activate             );
    testClass.def("terminate"             , &groupmsgtest::components::CompTwo::Terminate            );
    testClass.def("handlePortUpdate"      , &groupmsgtest::components::CompTwo::HandlePortUpdate     );
    testClass.def("handleCPULimit"        , &groupmsgtest::components::CompTwo::HandleCPULimit       );
    testClass.def("handleMemLimit"        , &groupmsgtest::components::CompTwo::HandleMemLimit       );
    testClass.def("handleSpcLimit"        , &groupmsgtest::components::CompTwo::HandleSpcLimit       );
    testClass.def("handleNetLimit"        , &groupmsgtest::components::CompTwo::HandleNetLimit       );
    testClass.def("handleNICStateChange"  , &groupmsgtest::components::CompTwo::HandleNICStateChange );
    testClass.def("handlePeerStateChange" , &groupmsgtest::components::CompTwo::HandlePeerStateChange);
    testClass.def("handleReinstate"       , &groupmsgtest::components::CompTwo::HandleReinstate      );
    testClass.def("handleGroupUpdate"     , &groupmsgtest::components::CompTwo::HandleGroupUpdate    );
    testClass.def("componentId"           , &groupmsgtest::components::CompTwo::component_id         );

    m.def("create_component_py", &create_component_py, "Instantiates the component from python configuration");
}
