


#include <CompThree.h>
// riaps:keep_header:begin

// riaps:keep_header:end

namespace leaderwiththree {
    namespace components {

        // riaps:keep_construct:begin
        CompThree::CompThree(const py::object*  parent_actor     ,
                      const py::dict     actor_spec       ,
                      const py::dict     type_spec        ,
                      const py::list group_spec,
                      const std::string& name             ,
                      const std::string& type_name        ,
                      const py::dict     args             ,
                      const std::string& application_name ,
                      const std::string& actor_name       )
            : CompThreeBase(parent_actor, actor_spec, type_spec, group_spec, name, type_name, args, application_name, actor_name) {
            joined_a_ = false;
        }
        // riaps:keep_construct:end

        void CompThree::OnClock() {
            // riaps:keep_onclock:begin
            auto msg = RecvClock();
            // riaps:keep_onclock:end
        }

        // riaps:keep_impl:begin

        // riaps:keep_impl:end

        // riaps:keep_destruct:begin
        CompThree::~CompThree() {

        }
        // riaps:keep_destruct:end

    }
}

std::unique_ptr<leaderwiththree::components::CompThree>
create_component_py(const py::object *parent_actor,
                    const py::dict actor_spec,
                    const py::dict type_spec,
                    const py::list group_spec,
                    const std::string &name,
                    const std::string &type_name,
                    const py::dict args,
                    const std::string &application_name,
                    const std::string &actor_name) {
    auto ptr = new leaderwiththree::components::CompThree(parent_actor, actor_spec, type_spec, group_spec, name, type_name, args,
                                                                     application_name,
                                                                     actor_name);
    return std::move(std::unique_ptr<leaderwiththree::components::CompThree>(ptr));
}

PYBIND11_MODULE(libcompthree, m) {
    py::class_<leaderwiththree::components::CompThree> testClass(m, "CompThree");
    testClass.def(
            py::init<const py::object *, const py::dict, const py::dict, const py::list, const std::string &, const std::string &, const py::dict, const std::string &, const std::string &>());

    testClass.def("setup"                 , &leaderwiththree::components::CompThree::Setup);
    testClass.def("activate"              , &leaderwiththree::components::CompThree::Activate);
    testClass.def("terminate"             , &leaderwiththree::components::CompThree::Terminate);
    testClass.def("handlePortUpdate"      , &leaderwiththree::components::CompThree::HandlePortUpdate);
    testClass.def("handleCPULimit"        , &leaderwiththree::components::CompThree::HandleCPULimit);
    testClass.def("handleMemLimit"        , &leaderwiththree::components::CompThree::HandleMemLimit);
    testClass.def("handleSpcLimit"        , &leaderwiththree::components::CompThree::HandleSpcLimit);
    testClass.def("handleNetLimit"        , &leaderwiththree::components::CompThree::HandleNetLimit);
    testClass.def("handleNICStateChange"  , &leaderwiththree::components::CompThree::HandleNICStateChange);
    testClass.def("handlePeerStateChange" , &leaderwiththree::components::CompThree::HandlePeerStateChange);
    testClass.def("handleReinstate"       , &leaderwiththree::components::CompThree::HandleReinstate);

    m.def("create_component_py", &create_component_py, "Instantiates the component from python configuration");
}

