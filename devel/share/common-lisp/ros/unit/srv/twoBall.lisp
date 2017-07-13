; Auto-generated. Do not edit!


(cl:in-package unit-srv)


;//! \htmlinclude twoBall-request.msg.html

(cl:defclass <twoBall-request> (roslisp-msg-protocol:ros-message)
  ((position1x
    :reader position1x
    :initarg :position1x
    :type cl:float
    :initform 0.0)
   (position1y
    :reader position1y
    :initarg :position1y
    :type cl:float
    :initform 0.0)
   (position2x
    :reader position2x
    :initarg :position2x
    :type cl:float
    :initform 0.0)
   (position2y
    :reader position2y
    :initarg :position2y
    :type cl:float
    :initform 0.0)
   (angle_a4
    :reader angle_a4
    :initarg :angle_a4
    :type cl:float
    :initform 0.0)
   (angle_a5
    :reader angle_a5
    :initarg :angle_a5
    :type cl:float
    :initform 0.0))
)

(cl:defclass twoBall-request (<twoBall-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <twoBall-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'twoBall-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name unit-srv:<twoBall-request> is deprecated: use unit-srv:twoBall-request instead.")))

(cl:ensure-generic-function 'position1x-val :lambda-list '(m))
(cl:defmethod position1x-val ((m <twoBall-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:position1x-val is deprecated.  Use unit-srv:position1x instead.")
  (position1x m))

(cl:ensure-generic-function 'position1y-val :lambda-list '(m))
(cl:defmethod position1y-val ((m <twoBall-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:position1y-val is deprecated.  Use unit-srv:position1y instead.")
  (position1y m))

(cl:ensure-generic-function 'position2x-val :lambda-list '(m))
(cl:defmethod position2x-val ((m <twoBall-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:position2x-val is deprecated.  Use unit-srv:position2x instead.")
  (position2x m))

(cl:ensure-generic-function 'position2y-val :lambda-list '(m))
(cl:defmethod position2y-val ((m <twoBall-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:position2y-val is deprecated.  Use unit-srv:position2y instead.")
  (position2y m))

(cl:ensure-generic-function 'angle_a4-val :lambda-list '(m))
(cl:defmethod angle_a4-val ((m <twoBall-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:angle_a4-val is deprecated.  Use unit-srv:angle_a4 instead.")
  (angle_a4 m))

(cl:ensure-generic-function 'angle_a5-val :lambda-list '(m))
(cl:defmethod angle_a5-val ((m <twoBall-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:angle_a5-val is deprecated.  Use unit-srv:angle_a5 instead.")
  (angle_a5 m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <twoBall-request>) ostream)
  "Serializes a message object of type '<twoBall-request>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'position1x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'position1y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'position2x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'position2y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angle_a4))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angle_a5))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <twoBall-request>) istream)
  "Deserializes a message object of type '<twoBall-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'position1x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'position1y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'position2x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'position2y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle_a4) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle_a5) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<twoBall-request>)))
  "Returns string type for a service object of type '<twoBall-request>"
  "unit/twoBallRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'twoBall-request)))
  "Returns string type for a service object of type 'twoBall-request"
  "unit/twoBallRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<twoBall-request>)))
  "Returns md5sum for a message object of type '<twoBall-request>"
  "7b48d08338b2f5afc2a84ccbb423f69e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'twoBall-request)))
  "Returns md5sum for a message object of type 'twoBall-request"
  "7b48d08338b2f5afc2a84ccbb423f69e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<twoBall-request>)))
  "Returns full string definition for message of type '<twoBall-request>"
  (cl:format cl:nil "~%float32 position1x~%float32 position1y~%~%~%~%float32 position2x~%float32 position2y~%~%~%float32 angle_a4~%float32 angle_a5~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'twoBall-request)))
  "Returns full string definition for message of type 'twoBall-request"
  (cl:format cl:nil "~%float32 position1x~%float32 position1y~%~%~%~%float32 position2x~%float32 position2y~%~%~%float32 angle_a4~%float32 angle_a5~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <twoBall-request>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <twoBall-request>))
  "Converts a ROS message object to a list"
  (cl:list 'twoBall-request
    (cl:cons ':position1x (position1x msg))
    (cl:cons ':position1y (position1y msg))
    (cl:cons ':position2x (position2x msg))
    (cl:cons ':position2y (position2y msg))
    (cl:cons ':angle_a4 (angle_a4 msg))
    (cl:cons ':angle_a5 (angle_a5 msg))
))
;//! \htmlinclude twoBall-response.msg.html

(cl:defclass <twoBall-response> (roslisp-msg-protocol:ros-message)
  ((commandx
    :reader commandx
    :initarg :commandx
    :type cl:float
    :initform 0.0)
   (commandy
    :reader commandy
    :initarg :commandy
    :type cl:float
    :initform 0.0))
)

(cl:defclass twoBall-response (<twoBall-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <twoBall-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'twoBall-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name unit-srv:<twoBall-response> is deprecated: use unit-srv:twoBall-response instead.")))

(cl:ensure-generic-function 'commandx-val :lambda-list '(m))
(cl:defmethod commandx-val ((m <twoBall-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:commandx-val is deprecated.  Use unit-srv:commandx instead.")
  (commandx m))

(cl:ensure-generic-function 'commandy-val :lambda-list '(m))
(cl:defmethod commandy-val ((m <twoBall-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:commandy-val is deprecated.  Use unit-srv:commandy instead.")
  (commandy m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <twoBall-response>) ostream)
  "Serializes a message object of type '<twoBall-response>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'commandx))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'commandy))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <twoBall-response>) istream)
  "Deserializes a message object of type '<twoBall-response>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'commandx) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'commandy) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<twoBall-response>)))
  "Returns string type for a service object of type '<twoBall-response>"
  "unit/twoBallResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'twoBall-response)))
  "Returns string type for a service object of type 'twoBall-response"
  "unit/twoBallResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<twoBall-response>)))
  "Returns md5sum for a message object of type '<twoBall-response>"
  "7b48d08338b2f5afc2a84ccbb423f69e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'twoBall-response)))
  "Returns md5sum for a message object of type 'twoBall-response"
  "7b48d08338b2f5afc2a84ccbb423f69e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<twoBall-response>)))
  "Returns full string definition for message of type '<twoBall-response>"
  (cl:format cl:nil "float32 commandx~%float32 commandy~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'twoBall-response)))
  "Returns full string definition for message of type 'twoBall-response"
  (cl:format cl:nil "float32 commandx~%float32 commandy~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <twoBall-response>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <twoBall-response>))
  "Converts a ROS message object to a list"
  (cl:list 'twoBall-response
    (cl:cons ':commandx (commandx msg))
    (cl:cons ':commandy (commandy msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'twoBall)))
  'twoBall-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'twoBall)))
  'twoBall-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'twoBall)))
  "Returns string type for a service object of type '<twoBall>"
  "unit/twoBall")