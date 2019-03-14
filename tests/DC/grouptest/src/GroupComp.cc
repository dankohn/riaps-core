#include <GroupComp.h>

using namespace std;
using namespace riaps::groups;

namespace grouptest {
  namespace components {

    GroupComp::GroupComp(const py::object *parent_actor,
                         const py::dict actor_spec, // Actor json config
                         const py::dict type_spec,  // component json config
                         const std::string &name,
                         const std::string &type_name,
                         const py::dict args,
                         const std::string &application_name,
                         const std::string &actor_name)
            : GroupCompBase(parent_actor, actor_spec, type_spec, name, type_name, args, application_name,
                                 actor_name), m_joinedToA(false), m_joinedToB(false) {

      auto uid = zuuid_new();
      string uidStr = zuuid_str(uid);
      zuuid_destroy(&uid);

      m_publicGroupId = GroupId{GROUP_TYPE_GROUPA, "PublicGroup"};
      m_uniqueGroupId = GroupId{GROUP_TYPE_GROUPB, uidStr};
    }

    void GroupComp::OnClock() {
      auto _logger = component_logger();
      if (!m_joinedToA) {
        _logger->info("Component joins to {}:{}", m_publicGroupId.group_type_id, m_publicGroupId.group_name);
        auto joined = JoinGroup(m_publicGroupId);
        if (joined){
          m_joinedToA = true;
        } else {
            _logger->error("Couldn't join to group {}:{}", m_publicGroupId.group_type_id, m_publicGroupId.group_name);
        }
        
      } else {
        auto count = GetGroupMemberCount(m_publicGroupId);
        _logger->info("Number of members in groupA (including the current node): {}", count+1);
      }

      if (!m_joinedToB) {
        _logger->info("Component joins to {}:{}", m_uniqueGroupId.group_type_id, m_uniqueGroupId.group_name);
        auto joined = JoinGroup(m_uniqueGroupId);
        if (joined){
          m_joinedToB = true;
        } else {
            _logger->error("Couldn't join to group {}:{}", m_uniqueGroupId.group_type_id, m_uniqueGroupId.group_name);
        }
      } else {
        auto count = GetGroupMemberCount(m_uniqueGroupId);
        _logger->info("Number of members in groupB (including the current node): {}", count+1);
      }
    }

    void GroupComp::OnGroupMessage(const riaps::groups::GroupId &groupId,
                                   capnp::FlatArrayMessageReader &capnpreader,
                                   riaps::ports::PortBase *port) {}

    GroupComp::~GroupComp() {}
  } // namespace components
} // namespace grouptest

std::unique_ptr<grouptest::components::GroupComp>
create_component_py(const py::object *parent_actor,
                    const py::dict actor_spec,
                    const py::dict type_spec,
                    const std::string &name,
                    const std::string &type_name,
                    const py::dict args,
                    const std::string &application_name,
                    const std::string &actor_name) {
    auto ptr = new grouptest::components::GroupComp(parent_actor, actor_spec, type_spec, name, type_name, args,
                                                                    application_name,
                                                                    actor_name);
    return std::move(std::unique_ptr<grouptest::components::GroupComp>(ptr));
}

PYBIND11_MODULE(libgroupcomp, m) {
    py::class_<grouptest::components::GroupComp> testClass(m, "GroupComp");
    testClass.def(py::init<const py::object*, const py::dict, const py::dict, const std::string&, const std::string&, const py::dict, const std::string&, const std::string&>());

    testClass.def("setup"                 , &grouptest::components::GroupComp::Setup);
    testClass.def("activate"              , &grouptest::components::GroupComp::Activate);
    testClass.def("terminate"             , &grouptest::components::GroupComp::Terminate);
    testClass.def("handlePortUpdate"      , &grouptest::components::GroupComp::HandlePortUpdate);
    testClass.def("handleCPULimit"        , &grouptest::components::GroupComp::HandleCPULimit);
    testClass.def("handleMemLimit"        , &grouptest::components::GroupComp::HandleMemLimit);
    testClass.def("handleSpcLimit"        , &grouptest::components::GroupComp::HandleSpcLimit);
    testClass.def("handleNetLimit"        , &grouptest::components::GroupComp::HandleNetLimit);
    testClass.def("handleNICStateChange"  , &grouptest::components::GroupComp::HandleNICStateChange);
    testClass.def("handlePeerStateChange" , &grouptest::components::GroupComp::HandlePeerStateChange);
    testClass.def("handleReinstate"       , &grouptest::components::GroupComp::HandleReinstate);

    m.def("create_component_py", &create_component_py, "Instantiates the component from python configuration");
}