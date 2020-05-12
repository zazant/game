# Project definition
cmake_minimum_required(VERSION 3.1)
project(game)

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

# Copy shader files
file(COPY "${SRC_DIR}/shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/res")

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
add_definitions(-DGLFW_INCLUDE_NONE)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DGL_SILENCE_DEPRECATION)
    target_sources(${PROJECT_NAME} PRIVATE "${SRC_DIR}/glad.c")
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# libraries
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_package(glad CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad glm)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE glfw nlohmann_json::nlohmann_json)
