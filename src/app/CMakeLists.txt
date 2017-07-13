cmake_minimum_required(VERSION 2.8.3)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)


ADD_DEFINITIONS(
    -std=gnu++0x
)

project(app)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
sensor_msgs


geometry_msgs

std_srvs
 message_generation

)


#rosbuild_init()


#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/home/intelligentrobotics/ws/orocos/KUKACommander/srv_gen/cpp/include")

rosbuild_add_boost_directories()

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependencies might have been
##     pulled in transitively but can be declared for certainty nonetheless:
##     * add a build_depend tag for "message_generation"
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
 add_message_files(
   DIRECTORY msg
   FILES
   cam.msg
   twoBall.msg	
 )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
 )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES app
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories( include
  ${catkin_INCLUDE_DIRS}
)

include_directories (/home/intelligentrobotics/ws/orocos/motion_control/motion_control_msgs/msg_gen/cpp/include/motion_control_msgs)

include_directories (/home/intelligentrobotics/Alberto/fri/fri_remote)


rosbuild_add_executable(SimplePTP src/SimplePTP.cpp)

rosbuild_add_executable(SimplePTP1 src/SimplePTP1.cpp)

rosbuild_add_executable(sub src/sub.cpp)

rosbuild_add_executable(new src/simple_new.cpp)

rosbuild_add_executable(pub src/publisher.cpp)

rosbuild_add_executable(move src/move.cpp)
rosbuild_add_executable(move1 src/move1.cpp)

rosbuild_add_executable(pid src/pid.cpp)

rosbuild_add_executable(new_pid src/new_pid.cpp)
rosbuild_add_executable(ppid src/proper_pid.cpp)
rosbuild_add_executable(bup src/SimplePTP_backup.cpp)
rosbuild_add_executable(testing src/testing.cpp)
rosbuild_add_executable(pbp src/with_pbp.cpp)

rosbuild_add_executable(client src/client.cpp)
rosbuild_add_executable(pbp_vel src/pbp_vel.cpp)

rosbuild_add_executable(fancy src/fancy.cpp)
rosbuild_add_executable(pbp_double src/pbp_double.cpp)
rosbuild_add_executable(pbp_test src/pbp_test.cpp)
rosbuild_add_executable(pbp_sim_test src/pbp_sim_test.cpp)
rosbuild_add_executable(pbp_feedback src/pbp_feedback.cpp)
rosbuild_add_executable(pbp_cross src/pbp_cross.cpp)





## Declare a cpp library
# add_library(app
#   src/${PROJECT_NAME}/app.cpp
# )

## Declare a cpp executable
# add_executable(app_node src/app_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(app_node app_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(SimplePTP
  ${catkin_LIBRARIES}
 )

target_link_libraries(SimplePTP1
  ${catkin_LIBRARIES}
 )
target_link_libraries(sub
  ${catkin_LIBRARIES}
 )

target_link_libraries(new
  ${catkin_LIBRARIES}
 )

target_link_libraries(pub
  ${catkin_LIBRARIES}
 )

target_link_libraries(move
  ${catkin_LIBRARIES}
 )


target_link_libraries(move1
  ${catkin_LIBRARIES}
 )

target_link_libraries(pid
  ${catkin_LIBRARIES}
 )

target_link_libraries(new_pid
  ${catkin_LIBRARIES}
 )

target_link_libraries(ppid
  ${catkin_LIBRARIES}
 )

target_link_libraries(bup
  ${catkin_LIBRARIES}
 )

target_link_libraries(testing
  ${catkin_LIBRARIES}
 )

target_link_libraries(pbp
  ${catkin_LIBRARIES}
 )

target_link_libraries(client
  ${catkin_LIBRARIES}
 )

target_link_libraries(pbp_vel
  ${catkin_LIBRARIES}
 )

target_link_libraries(fancy
  ${catkin_LIBRARIES}
 )

target_link_libraries(pbp_double
  ${catkin_LIBRARIES}
 )
target_link_libraries(pbp_test
  ${catkin_LIBRARIES}
 )
target_link_libraries(pbp_sim_test
  ${catkin_LIBRARIES}
 )

target_link_libraries(pbp_feedback
  ${catkin_LIBRARIES}
 )

target_link_libraries(pbp_cross
 ${catkin_LIBRARIES}
)

#target_link_libraries(pub
 # ${catkin_LIBRARIES}
 #)
#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS app app_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_app.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
