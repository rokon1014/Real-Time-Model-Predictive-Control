; Auto-generated. Do not edit!


(cl:in-package unit-srv)


;//! \htmlinclude from_robot-request.msg.html

(cl:defclass <from_robot-request> (roslisp-msg-protocol:ros-message)
  ((position
    :reader position
    :initarg :position
    :type cl:float
    :initform 0.0)
   (angle
    :reader angle
    :initarg :angle
    :type cl:float
    :initform 0.0))
)

(cl:defclass from_robot-request (<from_robot-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <from_robot-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'from_robot-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name unit-srv:<from_robot-request> is deprecated: use unit-srv:from_robot-request instead.")))

(cl:ensure-generic-function 'position-val :lambda-list '(m))
(cl:defmethod position-val ((m <from_robot-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:position-val is deprecated.  Use unit-srv:position instead.")
  (position m))

(cl:ensure-generic-function 'angle-val :lambda-list '(m))
(cl:defmethod angle-val ((m <from_robot-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:angle-val is deprecated.  Use unit-srv:angle instead.")
  (angle m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <from_robot-request>) ostream)
  "Serializes a message object of type '<from_robot-request>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'position))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <from_robot-request>) istream)
  "Deserializes a message object of type '<from_robot-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'position) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<from_robot-request>)))
  "Returns string type for a service object of type '<from_robot-request>"
  "unit/from_robotRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'from_robot-request)))
  "Returns string type for a service object of type 'from_robot-request"
  "unit/from_robotRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<from_robot-request>)))
  "Returns md5sum for a message object of type '<from_robot-request>"
  "a20adcbdcf70fc177b6fbf8293f1e1b0")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'from_robot-request)))
  "Returns md5sum for a message object of type 'from_robot-request"
  "a20adcbdcf70fc177b6fbf8293f1e1b0")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<from_robot-request>)))
  "Returns full string definition for message of type '<from_robot-request>"
  (cl:format cl:nil "~%~%float32 position~%float32 angle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'from_robot-request)))
  "Returns full string definition for message of type 'from_robot-request"
  (cl:format cl:nil "~%~%float32 position~%float32 angle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <from_robot-request>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <from_robot-request>))
  "Converts a ROS message object to a list"
  (cl:list 'from_robot-request
    (cl:cons ':position (position msg))
    (cl:cons ':angle (angle msg))
))
;//! \htmlinclude from_robot-response.msg.html

(cl:defclass <from_robot-response> (roslisp-msg-protocol:ros-message)
  ((command
    :reader command
    :initarg :command
    :type cl:float
    :initform 0.0))
)

(cl:defclass from_robot-response (<from_robot-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <from_robot-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'from_robot-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name unit-srv:<from_robot-response> is deprecated: use unit-srv:from_robot-response instead.")))

(cl:ensure-generic-function 'command-val :lambda-list '(m))
(cl:defmethod command-val ((m <from_robot-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:command-val is deprecated.  Use unit-srv:command instead.")
  (command m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <from_robot-response>) ostream)
  "Serializes a message object of type '<from_robot-response>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'command))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <from_robot-response>) istream)
  "Deserializes a message object of type '<from_robot-response>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'command) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<from_robot-response>)))
  "Returns string type for a service object of type '<from_robot-response>"
  "unit/from_robotResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'from_robot-response)))
  "Returns string type for a service object of type 'from_robot-response"
  "unit/from_robotResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<from_robot-response>)))
  "Returns md5sum for a message object of type '<from_robot-response>"
  "a20adcbdcf70fc177b6fbf8293f1e1b0")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'from_robot-response)))
  "Returns md5sum for a message object of type 'from_robot-response"
  "a20adcbdcf70fc177b6fbf8293f1e1b0")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<from_robot-response>)))
  "Returns full string definition for message of type '<from_robot-response>"
  (cl:format cl:nil "float32 command~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'from_robot-response)))
  "Returns full string definition for message of type 'from_robot-response"
  (cl:format cl:nil "float32 command~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <from_robot-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <from_robot-response>))
  "Converts a ROS message object to a list"
  (cl:list 'from_robot-response
    (cl:cons ':command (command msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'from_robot)))
  'from_robot-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'from_robot)))
  'from_robot-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'from_robot)))
  "Returns string type for a service object of type '<from_robot>"
  "unit/from_robot")