

#ifndef COMPTHREEBASE_H
#define COMPTHREEBASE_H

#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <componentmodel/r_componentbase.h>
#include <componentmodel/r_messagebuilder.h>
#include <componentmodel/r_messagereader.h>
#include <messages/leaderwiththree.capnp.h>

namespace py = pybind11;
constexpr auto PORT_TIMER_CLOCK = "clock";


namespace leaderwiththree {
    namespace components {
        class CompThreeBase : public riaps::ComponentBase {
        public:
            CompThreeBase(const py::object*  parent_actor     ,
                          const py::dict     actor_spec       ,
                          const py::dict     type_spec        ,
                          const py::list group_spec,
                          const std::string& name             ,
                          const std::string& type_name        ,
                          const py::dict     args             ,
                          const std::string& application_name ,
                          const std::string& actor_name       );

            virtual void OnClock()=0;

            virtual timespec RecvClock() final;


            virtual ~CompThreeBase() = default;
        protected:
            virtual void DispatchMessage(riaps::ports::PortBase* port) final;

            virtual void DispatchInsideMessage(zmsg_t* zmsg, riaps::ports::PortBase* port) final;
        };
    }
}


#endif // COMPTHREEBASE_H
