#include <CompOne.h>

namespace groupmsgtest {
   namespace components {
      
      CompOne::CompOne(const py::object *parent_actor,
                       const py::dict actor_spec, // Actor json config
                       const py::dict type_spec,  // component json config
                       const py::list group_spec,
                       const std::string &name,
                       const std::string &type_name,
                       const py::dict args,
                       const std::string &application_name,
                       const std::string &actor_name) :
      CompOneBase(parent_actor, actor_spec, type_spec, group_spec, name, type_name, args, application_name,
                  actor_name),
                  m_joinedToA(false) {
          component_logger()->set_pattern("[%n] %v");
      }
      
      void CompOne::OnClock() {
          auto now = RecvClock();
          auto logger = component_logger();
          if (!m_joinedToA){
              logger->info("Component joins to {}:{}", groupIdA.group_type_id, groupIdA.group_name);
              auto joined = JoinGroup(groupIdA);
              if (joined)
                  m_joinedToA = true;
              else
                  logger->error("Couldn't join to group {}:{}", groupIdA.group_type_id, groupIdA.group_name);
         } else {
             MessageBuilder<messages::MessageType> builder;
             auto msg_content = fmt::format("{}", component_name());
             builder->setMsg(msg_content);

             SendGroupMessage(groupIdA, builder);
         }
      }
    
      
//      void CompOne::OnGroupMessage(const riaps::groups::GroupId& groupId, capnp::FlatArrayMessageReader& capnpreader, riaps::ports::PortBase* port){
//          logGroupMessage(_logger, __FUNCTION__, groupId, capnpreader.getRoot<MessageType>().getMsg());
//      }

   }
}

std::unique_ptr<groupmsgtest::components::CompOne>
create_component_py(const py::object *parent_actor,
                    const py::dict actor_spec,
                    const py::dict type_spec,
                    const py::list group_spec,
                    const std::string &name,
                    const std::string &type_name,
                    const py::dict args,
                    const std::string &application_name,
                    const std::string &actor_name) {
    auto ptr = new groupmsgtest::components::CompOne(parent_actor, actor_spec, type_spec, group_spec, name, type_name, args,
                                                    application_name,
                                                    actor_name);
    return std::move(std::unique_ptr<groupmsgtest::components::CompOne>(ptr));
}

PYBIND11_MODULE(libcompone, m) {
    py::class_<groupmsgtest::components::CompOne> testClass(m, "CompOne");
    testClass.def(py::init<const py::object*, const py::dict, const py::dict, const py::list, const std::string&, const std::string&, const py::dict, const std::string&, const std::string&>());

    testClass.def("setup"                 , &groupmsgtest::components::CompOne::Setup                );
    testClass.def("activate"              , &groupmsgtest::components::CompOne::Activate             );
    testClass.def("terminate"             , &groupmsgtest::components::CompOne::Terminate            );
    testClass.def("handlePortUpdate"      , &groupmsgtest::components::CompOne::HandlePortUpdate     );
    testClass.def("handleCPULimit"        , &groupmsgtest::components::CompOne::HandleCPULimit       );
    testClass.def("handleMemLimit"        , &groupmsgtest::components::CompOne::HandleMemLimit       );
    testClass.def("handleSpcLimit"        , &groupmsgtest::components::CompOne::HandleSpcLimit       );
    testClass.def("handleNetLimit"        , &groupmsgtest::components::CompOne::HandleNetLimit       );
    testClass.def("handleNICStateChange"  , &groupmsgtest::components::CompOne::HandleNICStateChange );
    testClass.def("handlePeerStateChange" , &groupmsgtest::components::CompOne::HandlePeerStateChange);
    testClass.def("handleReinstate"       , &groupmsgtest::components::CompOne::HandleReinstate      );
    testClass.def("handleGroupUpdate"     , &groupmsgtest::components::CompOne::HandleGroupUpdate    );
    testClass.def("componentId"           , &groupmsgtest::components::CompOne::component_id         );

    m.def("create_component_py", &create_component_py, "Instantiates the component from python configuration");
}
