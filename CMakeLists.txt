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

# libraries
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad glfw glm)
