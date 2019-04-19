


#include <CompOne.h>
// riaps:keep_header:begin

// riaps:keep_header:end

namespace leaderwiththree {
    namespace components {

        // riaps:keep_construct:begin
        CompOne::CompOne(const py::object*  parent_actor     ,
                      const py::dict     actor_spec       ,
                      const py::dict     type_spec        ,
                      const py::list group_spec,
                      const std::string& name             ,
                      const std::string& type_name        ,
                      const py::dict     args             ,
                      const std::string& application_name ,
                      const std::string& actor_name       )
            : CompOneBase(parent_actor, actor_spec, type_spec, group_spec, name, type_name, args, application_name, actor_name) {
            joined_a_ = false;
        }
        // riaps:keep_construct:end

        void CompOne::OnClock() {
            // riaps:keep_onclock:begin
            auto msg = RecvClock();
            auto logger = component_logger();
            if (!joined_a_){
                logger->info("Component joins {}:{}", GroupIdA.group_type_id, GroupIdA.group_name);
                auto joined = JoinGroup(GroupIdA);
                if (joined)
                    joined_a_ = true;
                else
                    logger->error("Couldn't join group {}:{}", GroupIdA.group_type_id, GroupIdA.group_name);
            }

            logger->info("The leader is: {}", GetLeaderId(GroupIdA));
            // riaps:keep_onclock:end
        }

        // riaps:keep_impl:begin

        // riaps:keep_impl:end

        // riaps:keep_destruct:begin
        CompOne::~CompOne() {

        }
        // riaps:keep_destruct:end

    }
}

std::unique_ptr<leaderwiththree::components::CompOne>
create_component_py(const py::object *parent_actor,
                    const py::dict actor_spec,
                    const py::dict type_spec,
                    const py::list group_spec,
                    const std::string &name,
                    const std::string &type_name,
                    const py::dict args,
                    const std::string &application_name,
                    const std::string &actor_name) {
    auto ptr = new leaderwiththree::components::CompOne(parent_actor, actor_spec, type_spec, group_spec, name, type_name, args,
                                                                     application_name,
                                                                     actor_name);
    return std::move(std::unique_ptr<leaderwiththree::components::CompOne>(ptr));
}

PYBIND11_MODULE(libcompone, m) {
    py::class_<leaderwiththree::components::CompOne> testClass(m, "CompOne");
    testClass.def(
            py::init<const py::object *, const py::dict, const py::dict, const py::list, const std::string &, const std::string &, const py::dict, const std::string &, const std::string &>());

    testClass.def("setup"                 , &leaderwiththree::components::CompOne::Setup);
    testClass.def("activate"              , &leaderwiththree::components::CompOne::Activate);
    testClass.def("terminate"             , &leaderwiththree::components::CompOne::Terminate);
    testClass.def("handlePortUpdate"      , &leaderwiththree::components::CompOne::HandlePortUpdate);
    testClass.def("handleCPULimit"        , &leaderwiththree::components::CompOne::HandleCPULimit);
    testClass.def("handleMemLimit"        , &leaderwiththree::components::CompOne::HandleMemLimit);
    testClass.def("handleSpcLimit"        , &leaderwiththree::components::CompOne::HandleSpcLimit);
    testClass.def("handleNetLimit"        , &leaderwiththree::components::CompOne::HandleNetLimit);
    testClass.def("handleNICStateChange"  , &leaderwiththree::components::CompOne::HandleNICStateChange);
    testClass.def("handlePeerStateChange" , &leaderwiththree::components::CompOne::HandlePeerStateChange);
    testClass.def("handleReinstate"       , &leaderwiththree::components::CompOne::HandleReinstate);

    m.def("create_component_py", &create_component_py, "Instantiates the component from python configuration");
}

