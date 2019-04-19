

#ifndef COMPTWO_H
#define COMPTWO_H
#include <base/CompTwoBase.h>
// riaps:keep_header:begin
#include "common.h"
// riaps:keep_header:end>>

namespace leaderwiththree {
    namespace components {
        class CompTwo : public CompTwoBase {
        public:
            CompTwo(const py::object*  parent_actor     ,
                          const py::dict     actor_spec       ,
                          const py::dict     type_spec        ,
                          const py::list group_spec,
                          const std::string& name             ,
                          const std::string& type_name        ,
                          const py::dict     args             ,
                          const std::string& application_name ,
                          const std::string& actor_name       );


            virtual void OnClock() override;

            virtual ~CompTwo();

            // riaps:keep_decl:begin
        private:
            bool joined_a_;

            // riaps:keep_decl:end
        };
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
                    const std::string &actor_name);

#endif // COMPTWO_H
