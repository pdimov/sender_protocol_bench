# Copyright 2026 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.14...4.2)

project(corosio_protocol_bench LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

# Boost::capy

message(STATUS "Fetching Boost::capy")

FetchContent_Declare(capy
    GIT_REPOSITORY https://github.com/cppalliance/capy.git
    GIT_TAG        develop)

FetchContent_MakeAvailable(capy)

# Boost::corosio

message(STATUS "Fetching Boost::corosio")

FetchContent_Declare(corosio
    GIT_REPOSITORY https://github.com/cppalliance/corosio.git
    GIT_TAG        develop)

FetchContent_MakeAvailable(corosio)

# Boost

message(STATUS "Fetching Boost")

FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.90.0/boost-1.90.0-cmake.tar.xz
  URL_HASH SHA256=aca59f889f0f32028ad88ba6764582b63c916ce5f77b31289ad19421a96c555f
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)

set(BOOST_INCLUDE_LIBRARIES container_hash describe assert)

FetchContent_MakeAvailable(Boost)

# beman::task

FetchContent_Declare(
    beman-task
    GIT_REPOSITORY https://github.com/bemanproject/task
    GIT_TAG 6163df9
    SYSTEM
    FIND_PACKAGE_ARGS
        NAMES beman.task
)

FetchContent_MakeAvailable(beman-task)

# sender_protocol_bench

add_executable(sender_protocol_bench)

target_sources(sender_protocol_bench PRIVATE main.cpp)

target_link_libraries(sender_protocol_bench PRIVATE beman::task Boost::container_hash Boost::describe Boost::assert)
