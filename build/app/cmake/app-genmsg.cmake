# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "app: 2 messages, 0 services")

set(MSG_I_FLAGS "-Iapp:/home/intelligentrobotics/rokon_clone/October/src/app/msg;-Istd_msgs:/opt/ros/groovy/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(app_generate_messages ALL)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(app
  "/home/intelligentrobotics/rokon_clone/October/src/app/msg/cam.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/app
)
_generate_msg_cpp(app
  "/home/intelligentrobotics/rokon_clone/October/src/app/msg/twoBall.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/app
)

### Generating Services

### Generating Module File
_generate_module_cpp(app
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/app
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(app_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(app_generate_messages app_generate_messages_cpp)

# target for backward compatibility
add_custom_target(app_gencpp)
add_dependencies(app_gencpp app_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS app_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(app
  "/home/intelligentrobotics/rokon_clone/October/src/app/msg/cam.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/app
)
_generate_msg_lisp(app
  "/home/intelligentrobotics/rokon_clone/October/src/app/msg/twoBall.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/app
)

### Generating Services

### Generating Module File
_generate_module_lisp(app
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/app
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(app_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(app_generate_messages app_generate_messages_lisp)

# target for backward compatibility
add_custom_target(app_genlisp)
add_dependencies(app_genlisp app_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS app_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(app
  "/home/intelligentrobotics/rokon_clone/October/src/app/msg/cam.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/app
)
_generate_msg_py(app
  "/home/intelligentrobotics/rokon_clone/October/src/app/msg/twoBall.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/app
)

### Generating Services

### Generating Module File
_generate_module_py(app
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/app
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(app_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(app_generate_messages app_generate_messages_py)

# target for backward compatibility
add_custom_target(app_genpy)
add_dependencies(app_genpy app_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS app_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/app)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/app
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(app_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/app)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/app
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(app_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/app)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/app\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/app
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(app_generate_messages_py std_msgs_generate_messages_py)
