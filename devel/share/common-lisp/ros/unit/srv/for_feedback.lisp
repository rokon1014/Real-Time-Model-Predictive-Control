; Auto-generated. Do not edit!


(cl:in-package unit-srv)


;//! \htmlinclude for_feedback-request.msg.html

(cl:defclass <for_feedback-request> (roslisp-msg-protocol:ros-message)
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

(cl:defclass for_feedback-request (<for_feedback-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <for_feedback-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'for_feedback-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name unit-srv:<for_feedback-request> is deprecated: use unit-srv:for_feedback-request instead.")))

(cl:ensure-generic-function 'positionx-val :lambda-list '(m))
(cl:defmethod positionx-val ((m <for_feedback-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:positionx-val is deprecated.  Use unit-srv:positionx instead.")
  (positionx m))

(cl:ensure-generic-function 'positiony-val :lambda-list '(m))
(cl:defmethod positiony-val ((m <for_feedback-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:positiony-val is deprecated.  Use unit-srv:positiony instead.")
  (positiony m))

(cl:ensure-generic-function 'angle_a4-val :lambda-list '(m))
(cl:defmethod angle_a4-val ((m <for_feedback-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:angle_a4-val is deprecated.  Use unit-srv:angle_a4 instead.")
  (angle_a4 m))

(cl:ensure-generic-function 'angle_a5-val :lambda-list '(m))
(cl:defmethod angle_a5-val ((m <for_feedback-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:angle_a5-val is deprecated.  Use unit-srv:angle_a5 instead.")
  (angle_a5 m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <for_feedback-request>) ostream)
  "Serializes a message object of type '<for_feedback-request>"
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <for_feedback-request>) istream)
  "Deserializes a message object of type '<for_feedback-request>"
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
    (cl:setf (cl:slot-value msg 'angle_a4) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle_a5) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<for_feedback-request>)))
  "Returns string type for a service object of type '<for_feedback-request>"
  "unit/for_feedbackRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'for_feedback-request)))
  "Returns string type for a service object of type 'for_feedback-request"
  "unit/for_feedbackRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<for_feedback-request>)))
  "Returns md5sum for a message object of type '<for_feedback-request>"
  "dba432639b8d724b704ae0fbd12f2039")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'for_feedback-request)))
  "Returns md5sum for a message object of type 'for_feedback-request"
  "dba432639b8d724b704ae0fbd12f2039")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<for_feedback-request>)))
  "Returns full string definition for message of type '<for_feedback-request>"
  (cl:format cl:nil "~%~%float32 positionx~%float32 positiony~%~%float32 angle_a4~%float32 angle_a5~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'for_feedback-request)))
  "Returns full string definition for message of type 'for_feedback-request"
  (cl:format cl:nil "~%~%float32 positionx~%float32 positiony~%~%float32 angle_a4~%float32 angle_a5~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <for_feedback-request>))
  (cl:+ 0
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <for_feedback-request>))
  "Converts a ROS message object to a list"
  (cl:list 'for_feedback-request
    (cl:cons ':positionx (positionx msg))
    (cl:cons ':positiony (positiony msg))
    (cl:cons ':angle_a4 (angle_a4 msg))
    (cl:cons ':angle_a5 (angle_a5 msg))
))
;//! \htmlinclude for_feedback-response.msg.html

(cl:defclass <for_feedback-response> (roslisp-msg-protocol:ros-message)
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

(cl:defclass for_feedback-response (<for_feedback-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <for_feedback-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'for_feedback-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name unit-srv:<for_feedback-response> is deprecated: use unit-srv:for_feedback-response instead.")))

(cl:ensure-generic-function 'commandx-val :lambda-list '(m))
(cl:defmethod commandx-val ((m <for_feedback-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:commandx-val is deprecated.  Use unit-srv:commandx instead.")
  (commandx m))

(cl:ensure-generic-function 'commandy-val :lambda-list '(m))
(cl:defmethod commandy-val ((m <for_feedback-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader unit-srv:commandy-val is deprecated.  Use unit-srv:commandy instead.")
  (commandy m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <for_feedback-response>) ostream)
  "Serializes a message object of type '<for_feedback-response>"
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <for_feedback-response>) istream)
  "Deserializes a message object of type '<for_feedback-response>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<for_feedback-response>)))
  "Returns string type for a service object of type '<for_feedback-response>"
  "unit/for_feedbackResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'for_feedback-response)))
  "Returns string type for a service object of type 'for_feedback-response"
  "unit/for_feedbackResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<for_feedback-response>)))
  "Returns md5sum for a message object of type '<for_feedback-response>"
  "dba432639b8d724b704ae0fbd12f2039")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'for_feedback-response)))
  "Returns md5sum for a message object of type 'for_feedback-response"
  "dba432639b8d724b704ae0fbd12f2039")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<for_feedback-response>)))
  "Returns full string definition for message of type '<for_feedback-response>"
  (cl:format cl:nil "float32 commandx~%float32 commandy~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'for_feedback-response)))
  "Returns full string definition for message of type 'for_feedback-response"
  (cl:format cl:nil "float32 commandx~%float32 commandy~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <for_feedback-response>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <for_feedback-response>))
  "Converts a ROS message object to a list"
  (cl:list 'for_feedback-response
    (cl:cons ':commandx (commandx msg))
    (cl:cons ':commandy (commandy msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'for_feedback)))
  'for_feedback-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'for_feedback)))
  'for_feedback-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'for_feedback)))
  "Returns string type for a service object of type '<for_feedback>"
  "unit/for_feedback")