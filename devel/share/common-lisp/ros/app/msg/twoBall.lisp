; Auto-generated. Do not edit!


(cl:in-package app-msg)


;//! \htmlinclude twoBall.msg.html

(cl:defclass <twoBall> (roslisp-msg-protocol:ros-message)
  ((pos1x
    :reader pos1x
    :initarg :pos1x
    :type cl:float
    :initform 0.0)
   (pos1y
    :reader pos1y
    :initarg :pos1y
    :type cl:float
    :initform 0.0)
   (pos2x
    :reader pos2x
    :initarg :pos2x
    :type cl:float
    :initform 0.0)
   (pos2y
    :reader pos2y
    :initarg :pos2y
    :type cl:float
    :initform 0.0))
)

(cl:defclass twoBall (<twoBall>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <twoBall>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'twoBall)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name app-msg:<twoBall> is deprecated: use app-msg:twoBall instead.")))

(cl:ensure-generic-function 'pos1x-val :lambda-list '(m))
(cl:defmethod pos1x-val ((m <twoBall>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader app-msg:pos1x-val is deprecated.  Use app-msg:pos1x instead.")
  (pos1x m))

(cl:ensure-generic-function 'pos1y-val :lambda-list '(m))
(cl:defmethod pos1y-val ((m <twoBall>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader app-msg:pos1y-val is deprecated.  Use app-msg:pos1y instead.")
  (pos1y m))

(cl:ensure-generic-function 'pos2x-val :lambda-list '(m))
(cl:defmethod pos2x-val ((m <twoBall>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader app-msg:pos2x-val is deprecated.  Use app-msg:pos2x instead.")
  (pos2x m))

(cl:ensure-generic-function 'pos2y-val :lambda-list '(m))
(cl:defmethod pos2y-val ((m <twoBall>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader app-msg:pos2y-val is deprecated.  Use app-msg:pos2y instead.")
  (pos2y m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <twoBall>) ostream)
  "Serializes a message object of type '<twoBall>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'pos1x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'pos1y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'pos2x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'pos2y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <twoBall>) istream)
  "Deserializes a message object of type '<twoBall>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos1x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos1y) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos2x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'pos2y) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<twoBall>)))
  "Returns string type for a message object of type '<twoBall>"
  "app/twoBall")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'twoBall)))
  "Returns string type for a message object of type 'twoBall"
  "app/twoBall")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<twoBall>)))
  "Returns md5sum for a message object of type '<twoBall>"
  "10c63de25c74c1fafcc31a4c3b845873")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'twoBall)))
  "Returns md5sum for a message object of type 'twoBall"
  "10c63de25c74c1fafcc31a4c3b845873")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<twoBall>)))
  "Returns full string definition for message of type '<twoBall>"
  (cl:format cl:nil "float64 pos1x~%float64 pos1y~%~%float64 pos2x~%float64 pos2y~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'twoBall)))
  "Returns full string definition for message of type 'twoBall"
  (cl:format cl:nil "float64 pos1x~%float64 pos1y~%~%float64 pos2x~%float64 pos2y~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <twoBall>))
  (cl:+ 0
     8
     8
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <twoBall>))
  "Converts a ROS message object to a list"
  (cl:list 'twoBall
    (cl:cons ':pos1x (pos1x msg))
    (cl:cons ':pos1y (pos1y msg))
    (cl:cons ':pos2x (pos2x msg))
    (cl:cons ':pos2y (pos2y msg))
))
