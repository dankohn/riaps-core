#ifndef RIAPS_FW_COMPONE_H
#define RIAPS_FW_COMPONE_H

#include "base/CompOneBase.h"
#include "common.h"
#include "messages/MessageType.capnp.h"

namespace groupmsgtest {
   namespace components {
      
      class CompOne : public CompOneBase {
         
         public:
         
         CompOne(const py::object *parent_actor,
                 const py::dict actor_spec, // Actor json config
                 const py::dict type_spec,  // component json config
                 const py::list group_spec,
                 const std::string &name,
                 const std::string &type_name,
                 const py::dict args,
                 const std::string &application_name,
                 const std::string &actor_name);
         
         virtual void OnClock();
         virtual ~CompOne() = default;

         private:
         bool m_joinedToA;
      };
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
                    const std::string &actor_name);


#endif //RIAPS_FW_COMPONE_H
