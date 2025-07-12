cmake_minimum_required(VERSION 3.16)

project(CupEngine VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find libraries
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# finde files recursively
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp )

add_executable(${PROJECT_NAME} ${SOURCES})

include(FetchContent)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Set destination for external headers
set(EXTERNAL_DIR ${CMAKE_BINARY_DIR}/external)

# Download the header if it doesn't exist
if (NOT EXISTS "${EXTERNAL_DIR}/stb_image.h")
    file(DOWNLOAD
        https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
        ${EXTERNAL_DIR}/stb_image.h
        STATUS download_status
        SHOW_PROGRESS
    )
    list(GET download_status 0 code)
    if (NOT code EQUAL 0)
        message(FATAL_ERROR "Failed to download stb_image.h")
    endif()
endif()

target_include_directories(${PROJECT_NAME} 
PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${EXTERNAL_DIR}
    ${Vulkan_INCLUDE_DIRS}
)


target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES})

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
