cmake_minimum_required(VERSION 3.13)

project(CupEngine VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find libraries
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_subdirectory(external)

# finde files recursively
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp )

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/external/utils
)

target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES} ecs)

###############################################################################
# Shader compiling
# from https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt  
find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

set(PROJECT_SHADER_DIR ${PROJECT_SOURCE_DIR}/src/graphics/shader)

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SHADER_DIR}/*.frag"
    "${PROJECT_SHADER_DIR}/*.vert"
    "${PROJECT_SHADER_DIR}/*.comp"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
  message(STATUS "BUILDING SHADER")
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SHADER_DIR}/bin/${FILE_NAME}.spv")
  message(STATUS ${GLSL})
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
)
