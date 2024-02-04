cmake_minimum_required(VERSION 3.3...3.27)
project(opendds_devguide_messenger CXX)
enable_testing()

find_package(OpenDDS REQUIRED)
include(opendds_testing)

# IDL TypeSupport Library
add_library(DDS_IDL_idl SHARED)
opendds_target_sources(DDS_IDL_idl PUBLIC "DDS_IDL.idl")
target_link_libraries(DDS_IDL_idl PUBLIC OpenDDS::Dcps)

set(opendds_libs
  OpenDDS::Dcps # Core OpenDDS Library
  OpenDDS::InfoRepoDiscovery OpenDDS::Tcp # For run_test.pl
  OpenDDS::Rtps OpenDDS::Rtps_Udp # For run_test.pl --rtps
  DDS_IDL_idl
)

# DDS_DLL
add_library(DDS_DLL SHARED
 	DataReaderListenerImpl.cpp
	Boilerplate.cpp
)
target_link_libraries(DDS_DLL ${opendds_libs})



