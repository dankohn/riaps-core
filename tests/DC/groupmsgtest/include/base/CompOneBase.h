#ifndef RIAPS_CORE_COMPONE_H
#define RIAPS_CORE_COMPONE_H

#include <componentmodel/r_componentbase.h>
#include <messages/MessageType.capnp.h>
#include <componentmodel/r_messagebuilder.h>
#include <componentmodel/r_messagereader.h>

#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <boost/optional.hpp>

namespace py = pybind11;

constexpr auto PORT_TIMER_CLOCK  = "clock";

namespace groupmsgtest {
  namespace components {

    class CompOneBase : public riaps::ComponentBase {

    public:
      CompOneBase(const py::object *parent_actor,
                  const py::dict actor_spec, // Actor json config
                  const py::dict type_spec,  // component json config
                  const py::list group_spec,
                  const std::string &name,
                  const std::string &type_name,
                  const py::dict args,
                  const std::string &application_name,
                  const std::string &actor_name);

      virtual void OnClock() = 0;
      virtual timespec RecvClock() final;
      virtual ~CompOneBase() = default;

    protected:
      virtual void DispatchMessage(riaps::ports::PortBase* port) final;
      virtual void DispatchInsideMessage(zmsg_t* zmsg, riaps::ports::PortBase* port) final;
    };
  } // namespace components
} // namespace groupmsgtest
#endif // RIAPS_CORE_COMPONE_H
