set(CMAKE_POSITION_INDEPENDENT_CODE ON)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(include)

pybind11_add_module(compone
        src/CompOne.cc
        src/base/CompOneBase.cc
        include/messages/MessageType.capnp.h
        include/messages/MessageType.capnp.c++
        )

target_link_libraries(compone PRIVATE czmq riaps dl capnp kj)
set_target_properties(compone PROPERTIES PREFIX lib SUFFIX .so)

pybind11_add_module(comptwo
        src/CompTwo.cc
        src/base/CompTwoBase.cc
        include/messages/MessageType.capnp.h
        include/messages/MessageType.capnp.c++
        )

target_link_libraries(comptwo PRIVATE czmq riaps dl capnp kj)
set_target_properties(comptwo PROPERTIES PREFIX lib SUFFIX .so)

#add_library(compone
#        SHARED ./cpp/CompOne.cc
#        ./cpp/CompOneBase.cc
#        ./cpp/include/common.h
#        )
#
#target_link_libraries(compone czmq riaps dl capnp kj)
#install(TARGETS compone DESTINATION lib)

## Creating Libraries for Component edu.vanderbilt.riaps.app.impl.ComponentImpl@215cd1bf (name: CompThree) (language: python, realtime: false)
#add_library(compthree
#        SHARED ./cpp/CompThree.cc
#        ./cpp/CompThreeBase.cc
#        ./cpp/include/common.h
#        )
#
#target_link_libraries(compthree czmq riaps dl capnp kj)
#install(TARGETS compthree DESTINATION lib)
#
## Creating Libraries for Component edu.vanderbilt.riaps.app.impl.ComponentImpl@7da7baec (name: CompTwo) (language: python, realtime: false)
#add_library(comptwo
#        SHARED ./cpp/CompTwo.cc
#        ./cpp/CompTwoBase.cc
#        ./cpp/include/common.h
#        )
#
#target_link_libraries(comptwo czmq riaps dl capnp kj)
#install(TARGETS comptwo DESTINATION lib)


