

#ifndef COMPONE_H
#define COMPONE_H
#include <base/CompOneBase.h>
// riaps:keep_header:begin
#include "common.h"
// riaps:keep_header:end>>

namespace leaderwiththree {
    namespace components {
        class CompOne : public CompOneBase {
        public:
            CompOne(const py::object*  parent_actor     ,
                          const py::dict     actor_spec       ,
                          const py::dict     type_spec        ,
                          const py::list group_spec,
                          const std::string& name             ,
                          const std::string& type_name        ,
                          const py::dict     args             ,
                          const std::string& application_name ,
                          const std::string& actor_name       );


            virtual void OnClock() override;

            virtual ~CompOne();

            // riaps:keep_decl:begin
        private:
            bool joined_a_;

            // riaps:keep_decl:end
        };
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
                    const std::string &actor_name);

#endif // COMPONE_H
