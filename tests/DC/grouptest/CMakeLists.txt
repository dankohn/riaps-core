#cmake_minimum_required(VERSION 3.0)
#set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_CXX_FLAGS "-std=c++11")
#set(CMAKE_C_FLAGS "-std=c99")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${ARCH}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
##Library Dependencies
#set(DEPENDENCIES ${RIAPS_PREFIX})
#set (LIBALLPATH_INCLUDE ${DEPENDENCIES}/include)
#set (LIBALLPATH_LIB ${DEPENDENCIES}/lib)
#link_directories(${LIBALLPATH_LIB})
#include_directories(${CMAKE_SOURCE_DIR}//messages-gen)
#include_directories(${CMAKE_SOURCE_DIR}/cpp/include)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)



include_directories(include)

# Creating Libraries for Component edu.vanderbilt.riaps.app.impl.ComponentImpl@3b12207e (name: GroupComp) (language: python, realtime: false)
pybind11_add_module(groupcomp src/GroupComp.cc
			                  src/base/GroupCompBase.cc
								include/messages/MessageType.capnp.c++
			)
			
target_link_libraries(groupcomp PRIVATE czmq riaps dl capnp kj)
set_target_properties(groupcomp PROPERTIES PREFIX lib SUFFIX .so)

