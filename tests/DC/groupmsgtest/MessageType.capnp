@0xa15e3a45b1cda517;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("groupmsgtest::messages");

struct MessageType
{
    msg @0: Text;
}
