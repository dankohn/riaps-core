


#include <CompTwo.h>
// riaps:keep_header:begin

// riaps:keep_header:end

namespace leaderwiththree {
    namespace components {

        // riaps:keep_construct:begin
        CompTwo::CompTwo(const py::object*  parent_actor     ,
                      const py::dict     actor_spec       ,
                      const py::dict     type_spec        ,
                      const py::list group_spec,
                      const std::string& name             ,
                      const std::string& type_name        ,
                      const py::dict     args             ,
                      const std::string& application_name ,
                      const std::string& actor_name       )
            : CompTwoBase(parent_actor, actor_spec, type_spec, group_spec, name, type_name, args, application_name, actor_name) {
            joined_a_ = false;
        }
        // riaps:keep_construct:end

        void CompTwo::OnClock() {
            // riaps:keep_onclock:begin
            auto msg = RecvClock();
            // riaps:keep_onclock:end
        }

        // riaps:keep_impl:begin

        // riaps:keep_impl:end

        // riaps:keep_destruct:begin
        CompTwo::~CompTwo() {

        }
        // riaps:keep_destruct:end

    }
}

std::unique_ptr<leaderwiththree::components::CompTwo>
create_component_py(const py::object *parent_actor,
                    const py::dict actor_spec,
                    const py::dict type_spec,
                    const py::list group_spec,
                    const std::string &name,
                    const std::string &type_name,
                    const py::dict args,
                    const std::string &application_name,
                    const std::string &actor_name) {
    auto ptr = new leaderwiththree::components::CompTwo(parent_actor, actor_spec, type_spec, group_spec, name, type_name, args,
                                                                     application_name,
                                                                     actor_name);
    return std::move(std::unique_ptr<leaderwiththree::components::CompTwo>(ptr));
}

PYBIND11_MODULE(libcomptwo, m) {
    py::class_<leaderwiththree::components::CompTwo> testClass(m, "CompTwo");
    testClass.def(
            py::init<const py::object *, const py::dict, const py::dict, const py::list, const std::string &, const std::string &, const py::dict, const std::string &, const std::string &>());

    testClass.def("setup"                 , &leaderwiththree::components::CompTwo::Setup);
    testClass.def("activate"              , &leaderwiththree::components::CompTwo::Activate);
    testClass.def("terminate"             , &leaderwiththree::components::CompTwo::Terminate);
    testClass.def("handlePortUpdate"      , &leaderwiththree::components::CompTwo::HandlePortUpdate);
    testClass.def("handleCPULimit"        , &leaderwiththree::components::CompTwo::HandleCPULimit);
    testClass.def("handleMemLimit"        , &leaderwiththree::components::CompTwo::HandleMemLimit);
    testClass.def("handleSpcLimit"        , &leaderwiththree::components::CompTwo::HandleSpcLimit);
    testClass.def("handleNetLimit"        , &leaderwiththree::components::CompTwo::HandleNetLimit);
    testClass.def("handleNICStateChange"  , &leaderwiththree::components::CompTwo::HandleNICStateChange);
    testClass.def("handlePeerStateChange" , &leaderwiththree::components::CompTwo::HandlePeerStateChange);
    testClass.def("handleReinstate"       , &leaderwiththree::components::CompTwo::HandleReinstate);

    m.def("create_component_py", &create_component_py, "Instantiates the component from python configuration");
}

