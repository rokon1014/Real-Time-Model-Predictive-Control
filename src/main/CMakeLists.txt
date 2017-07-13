# Declare the version of the CMake API for forward-compatibility
cmake_minimum_required(VERSION 2.8)


# Declare the name of the CMake Project
project(main)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fPIC -std=c++0x    -fmessage-length=0 ")

# Find and get all the information about the roscpp package
find_package(roscpp REQUIRED)

find_package(utils REQUIRED)

# Find Catkin
find_package(catkin REQUIRED)
# Declare this project as a catkin package and export the necessary build flags
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES main
  CATKIN_DEPENDS roscpp
  )

# Add the local headers and the headers from roscpp
include_directories(include ${roscpp_INCLUDE_DIRS}  ${utils_INCLUDE_DIRS})
include_directories ("${PROJECT_SOURCE_DIR}/eigen")
include_directories("/home/intelligentrobotics/ode-0.12/ode/src/joints")
include_directories("/usr/local/include")

link_directories("/usr/local/lib")

# Define a library target called hello_world
add_library(main src/ControlPBP.cpp src/UnityOde.cpp )
target_link_libraries(main ${roscpp_LIBRARIES} utils ode)


