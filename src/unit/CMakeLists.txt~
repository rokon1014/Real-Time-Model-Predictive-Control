# Declare the version of the CMake API for forward-compatibility
cmake_minimum_required(VERSION 2.8)


# Declare the name of the CMake Project
project(unit)
set(DCMAKE_BUILD_TYPE=Release)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x  -fmessage-length=0 ")


#set(CMAKE_BUILD_TYPE Release)
# Find and get all the information about the roscpp package
find_package(roscpp REQUIRED)
find_package(utils REQUIRED)
find_package(main REQUIRED)
find_package(catkin REQUIRED rospy std_msgs genmsg  message_generation)
FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
#generate_messages(DEPENDENCIES std_msgs)


add_service_files(
	DIRECTORY srv
	FILES
	from_robot.srv
	for_double.srv
	for_feedback.srv
	twoBall.srv
)

generate_messages(DEPENDENCIES std_msgs)

# Declare this project as a catkin package and export the necessary build flags
catkin_package(
	#INCLUDE_DIRS include
	LIBRARIES unit
	CATKIN_DEPENDS roscpp
  )



# Add the local headers and the headers from roscpp
include_directories( include ${catkin_INCLUDE_DIRS})
include_directories(include ${roscpp_INCLUDE_DIRS}  ${utils_INCLUDE_DIRS} ${main_INCLUDE_DIRS})
include_directories ("${PROJECT_SOURCE_DIR}/eigen")
include_directories("/home/spacemaster09/ode-0.12/ode/src/joints")
include_directories("/usr/local/include")
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

link_directories("/usr/local/lib")

# Define a library target called hello_world
add_executable(unit unit_working.cpp )
add_executable(unit_double unit_double.cpp )
add_executable(test1 pide.cpp )
add_executable(test_p unit_backup.cpp )
add_executable(vel vel.cpp )
add_executable(vel_check vel_check.cpp )
add_executable(vel_double vel_double.cpp )
add_executable(unit_test unit_test.cpp )
add_executable(unit_single unit_single.cpp )
add_executable(unit_amotor unit_amotor.cpp )
add_executable(unit_obs unit_amotor_obs.cpp )
add_executable(unit_sim unit_similarity_test.cpp )
add_executable(boost_first boost_first.cpp )
add_executable(fresh fresh_short.cpp )
add_executable(fresh_single fresh_single.cpp )
add_executable(fresh_single_y fresh_single_y.cpp )
add_executable(fresh_double fresh_double.cpp )
add_executable(fresh_double_ros fresh_double_ros.cpp )
add_executable(fresh_test fresh_test.cpp )
add_executable(fresh_feedback fresh_feedback.cpp )
add_executable(fresh_feedback_double fresh_feedback_double.cpp )
add_executable(fresh_filter fresh_filter.cpp )
add_executable(fresh_cone fresh_cone.cpp )
add_executable(fresh_maze fresh_maze.cpp )

add_executable(fresh_maze_rel fresh_maze_rel.cpp )
add_executable(fresh_pieces fresh_pieces.cpp )
add_executable(fresh_cross fresh_cross.cpp )
#add_executable(unit_corrected unit_double_corrected.cpp )


target_link_libraries(unit ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(unit_test ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(test1 ${roscpp_LIBRARIES} utils ode main  )
target_link_libraries(test_p ${roscpp_LIBRARIES} utils ode main  )
target_link_libraries(vel ${roscpp_LIBRARIES} utils ode main  )
target_link_libraries(vel_check ${roscpp_LIBRARIES} utils ode main  )
target_link_libraries(vel_double ${roscpp_LIBRARIES} utils ode main  )
target_link_libraries(unit_double ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(unit_single ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(unit_amotor ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(unit_obs ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(unit_sim ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(boost_first ${roscpp_LIBRARIES} ${Boost_LIBRARIES} utils ode main)
target_link_libraries(fresh ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_single ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_single_y ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_double ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_double_ros ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_test ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_feedback ${roscpp_LIBRARIES} utils ode main)
#target_link_libraries(unit_corrected ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_feedback_double ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_filter ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_cone ${roscpp_LIBRARIES} utils ode main)


target_link_libraries(fresh_maze ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_maze_rel ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_pieces ${roscpp_LIBRARIES} utils ode main)
target_link_libraries(fresh_cross ${roscpp_LIBRARIES} utils ode main)


