include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)

cmake_minimum_required(VERSION 3.15)

project(ecs 
    VERSION 0.1
    DESCRIPTION "standalone ECS implementation from cup engine"
)

# UTILS_PATH is required for compilation
if(NOT DEFINED UTILS_PATH)
    message(FATAL_ERROR "UTILS_PATH not set in .env.cmake file")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# create library target
add_library(${PROJECT_NAME} STATIC)

# find every .cpp file in src/ 
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# add sources to library
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

# src will not be accessable when included in other applications
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${UTILS_PATH}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

file(GLOB_RECURSE public_headers ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(installing)
