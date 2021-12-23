cmake_minimum_required(VERSION 3.12)

project(VALUE)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options( -std=c++14 )
    message("VALUE Debug.build")
ELSE()
    add_compile_options( -O3 -std=c++14 )
    message("VALUE Release.build")
ENDIF()

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(DownloadProject.cmake)
download_project(PROJ                catchtest
                 GIT_REPOSITORY      https://github.com/catchorg/Catch2.git
                 GIT_TAG             v2.x
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Disable building tests for GoogleTest
set(gtest_build_tests OFF CACHE BOOL "" FORCE)
set(gtest_build_samples OFF CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

download_project(PROJ                googlebench
                 GIT_REPOSITORY      https://github.com/google/benchmark.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Disable building tests for GoogleBenchmark
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(${googlebench_SOURCE_DIR} ${googlebench_BINARY_DIR})

#FIND_PACKAGE(Boost)
#IF (Boost_FOUND)
#    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
#    ADD_DEFINITIONS( "-DHAS_BOOST" )
#ENDIF()

#add_executable(foo foo.cc)
#target_link_libraries(foo Boost::date_time Boost::filesystem
#                          Boost::iostreams)

#add_subdirectory(boost-cmake)


add_executable(qs_tests tests/qs_tests.cpp)
##target_sources(qs_tests PUBLIC $<BUILD_INTERFACE:${QS_SOURCE_DIR}/src>)
#target_link_libraries(qs_tests Boost::boost)
##add_test(NAME qs_tests COMMAND qs_tests)

function(build_source_file base_name )
  message(STATUS ${base_name})
  add_executable(${base_name} ${base_name}.cpp)
  target_sources(${base_name} PUBLIC $<BUILD_INTERFACE:${QS_SOURCE_DIR}/src>)
endfunction(build_source_file)


list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
#include (gcovr)

#add_subdirectory("example")