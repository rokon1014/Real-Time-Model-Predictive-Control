; Auto-generated. Do not edit!


(cl:in-package unit-srv)


;//! \htmlinclude for_double-request.msg.html

(cl:defclass <for_double-request> (roslisp-msg-protocol:ros-message)
  ((positionx
    :reader positionx
    :initarg :positionx
    :type cl:float
    :initform 0.0)
   (positiony
    :reader positiony
    :initarg :positiony
    :type cl:float
    :initform 0.0)
   (angle
    :reader angle
    :initarg :angle
    :type cl:float
    :initform 0.0))
)

(cl:defclass for_double-request (<for_double-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <for_double-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'for_double-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name unit-srv:<for_double-request> is deprecated: use unit-srv:for_double-request instead.")))

(cl:ensure-generic-function 'positionx-val :lambda-list '(m))
(cl:defmethod positionx-val ((m <for_double-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:positionx-val is deprecated.  Use unit-srv:positionx instead.")
  (positionx m))

(cl:ensure-generic-function 'positiony-val :lambda-list '(m))
(cl:defmethod positiony-val ((m <for_double-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:positiony-val is deprecated.  Use unit-srv:positiony instead.")
  (positiony m))

(cl:ensure-generic-function 'angle-val :lambda-list '(m))
(cl:defmethod angle-val ((m <for_double-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:angle-val is deprecated.  Use unit-srv:angle instead.")
  (angle m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <for_double-request>) ostream)
  "Serializes a message object of type '<for_double-request>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'positionx))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'positiony))))
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <for_double-request>) istream)
  "Deserializes a message object of type '<for_double-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'positionx) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'positiony) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<for_double-request>)))
  "Returns string type for a service object of type '<for_double-request>"
  "unit/for_doubleRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'for_double-request)))
  "Returns string type for a service object of type 'for_double-request"
  "unit/for_doubleRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<for_double-request>)))
  "Returns md5sum for a message object of type '<for_double-request>"
  "50593c5e0bad1ac0da1ec8e0a05f42b7")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'for_double-request)))
  "Returns md5sum for a message object of type 'for_double-request"
  "50593c5e0bad1ac0da1ec8e0a05f42b7")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<for_double-request>)))
  "Returns full string definition for message of type '<for_double-request>"
  (cl:format cl:nil "~%~%float32 positionx~%float32 positiony~%~%float32 angle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'for_double-request)))
  "Returns full string definition for message of type 'for_double-request"
  (cl:format cl:nil "~%~%float32 positionx~%float32 positiony~%~%float32 angle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <for_double-request>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <for_double-request>))
  "Converts a ROS message object to a list"
  (cl:list 'for_double-request
    (cl:cons ':positionx (positionx msg))
    (cl:cons ':positiony (positiony msg))
    (cl:cons ':angle (angle msg))
))
;//! \htmlinclude for_double-response.msg.html

(cl:defclass <for_double-response> (roslisp-msg-protocol:ros-message)
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

(cl:defclass for_double-response (<for_double-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <for_double-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'for_double-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name unit-srv:<for_double-response> is deprecated: use unit-srv:for_double-response instead.")))

(cl:ensure-generic-function 'commandx-val :lambda-list '(m))
(cl:defmethod commandx-val ((m <for_double-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:commandx-val is deprecated.  Use unit-srv:commandx instead.")
  (commandx m))

(cl:ensure-generic-function 'commandy-val :lambda-list '(m))
(cl:defmethod commandy-val ((m <for_double-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:commandy-val is deprecated.  Use unit-srv:commandy instead.")
  (commandy m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <for_double-response>) ostream)
  "Serializes a message object of type '<for_double-response>"
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <for_double-response>) istream)
  "Deserializes a message object of type '<for_double-response>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<for_double-response>)))
  "Returns string type for a service object of type '<for_double-response>"
  "unit/for_doubleResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'for_double-response)))
  "Returns string type for a service object of type 'for_double-response"
  "unit/for_doubleResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<for_double-response>)))
  "Returns md5sum for a message object of type '<for_double-response>"
  "50593c5e0bad1ac0da1ec8e0a05f42b7")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'for_double-response)))
  "Returns md5sum for a message object of type 'for_double-response"
  "50593c5e0bad1ac0da1ec8e0a05f42b7")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<for_double-response>)))
  "Returns full string definition for message of type '<for_double-response>"
  (cl:format cl:nil "float32 commandx~%float32 commandy~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'for_double-response)))
  "Returns full string definition for message of type 'for_double-response"
  (cl:format cl:nil "float32 commandx~%float32 commandy~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <for_double-response>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <for_double-response>))
  "Converts a ROS message object to a list"
  (cl:list 'for_double-response
    (cl:cons ':commandx (commandx msg))
    (cl:cons ':commandy (commandy msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'for_double)))
  'for_double-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'for_double)))
  'for_double-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'for_double)))
  "Returns string type for a service object of type '<for_double>"
  "unit/for_double")