# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "unit: 0 messages, 4 services")

set(MSG_I_FLAGS "-Istd_msgs:/opt/ros/groovy/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(unit_generate_messages ALL)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages

### Generating Services
_generate_srv_cpp(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/for_feedback.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/unit
)
_generate_srv_cpp(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/from_robot.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/unit
)
_generate_srv_cpp(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/twoBall.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/unit
)
_generate_srv_cpp(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/for_double.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/unit
)

### Generating Module File
_generate_module_cpp(unit
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/unit
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(unit_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(unit_generate_messages unit_generate_messages_cpp)

# target for backward compatibility
add_custom_target(unit_gencpp)
add_dependencies(unit_gencpp unit_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS unit_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages

### Generating Services
_generate_srv_lisp(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/for_feedback.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/unit
)
_generate_srv_lisp(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/from_robot.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/unit
)
_generate_srv_lisp(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/twoBall.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/unit
)
_generate_srv_lisp(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/for_double.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/unit
)

### Generating Module File
_generate_module_lisp(unit
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/unit
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(unit_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(unit_generate_messages unit_generate_messages_lisp)

# target for backward compatibility
add_custom_target(unit_genlisp)
add_dependencies(unit_genlisp unit_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS unit_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages

### Generating Services
_generate_srv_py(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/for_feedback.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/unit
)
_generate_srv_py(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/from_robot.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/unit
)
_generate_srv_py(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/twoBall.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/unit
)
_generate_srv_py(unit
  "/home/intelligentrobotics/rokon_clone/October/src/unit/srv/for_double.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/unit
)

### Generating Module File
_generate_module_py(unit
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/unit
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(unit_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(unit_generate_messages unit_generate_messages_py)

# target for backward compatibility
add_custom_target(unit_genpy)
add_dependencies(unit_genpy unit_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS unit_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/unit)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/unit
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(unit_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/unit)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/unit
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(unit_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/unit)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/unit\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/unit
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(unit_generate_messages_py std_msgs_generate_messages_py)
