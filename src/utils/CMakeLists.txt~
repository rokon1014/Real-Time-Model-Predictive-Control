# Declare the version of the CMake API for forward-compatibility
cmake_minimum_required(VERSION 2.8)

# Declare the name of the CMake Project
project(utils)

# Find and get all the information about the roscpp package
find_package(roscpp REQUIRED)

# Find Catkin
find_package(catkin REQUIRED)
# Declare this project as a catkin package and export the necessary build flags
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES utils
  CATKIN_DEPENDS roscpp
  )

# Add the local headers and the headers from roscpp
include_directories(include ${roscpp_INCLUDE_DIRS})
include_directories ("${PROJECT_SOURCE_DIR}/eigen")
link_directories("/usr/local/lib")

# Define a library target called hello_world
add_library(utils src/ClippedGaussianSampling.cpp  src/DiagonalGMM.cpp  src/DynamicPdfSampler.cpp src/EigenGeometryUtils.cpp src/EigenMathUtils.cpp  src/FileUtils.cpp src/IndexRandomizer.cpp src/LinAlgUtilities.cpp src/NormalDistribution.cpp src/Optimization.cpp src/RandomForestSampler.cpp src/SamplingTree.cpp )

target_link_libraries(utils ${roscpp_LIBRARIES} ode)


