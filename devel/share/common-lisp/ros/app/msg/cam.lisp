; Auto-generated. Do not edit!


(cl:in-package app-msg)


;//! \htmlinclude cam.msg.html

(cl:defclass <cam> (roslisp-msg-protocol:ros-message)
  ((posx
    :reader posx
    :initarg :posx
    :type cl:float
    :initform 0.0)
   (posy
    :reader posy
    :initarg :posy
    :type cl:float
    :initform 0.0))
)

(cl:defclass cam (<cam>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <cam>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'cam)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name app-msg:<cam> is deprecated: use app-msg:cam instead.")))

(cl:ensure-generic-function 'posx-val :lambda-list '(m))
(cl:defmethod posx-val ((m <cam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader app-msg:posx-val is deprecated.  Use app-msg:posx instead.")
  (posx m))

(cl:ensure-generic-function 'posy-val :lambda-list '(m))
(cl:defmethod posy-val ((m <cam>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader app-msg:posy-val is deprecated.  Use app-msg:posy instead.")
  (posy m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <cam>) ostream)
  "Serializes a message object of type '<cam>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'posx))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'posy))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <cam>) istream)
  "Deserializes a message object of type '<cam>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'posx) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'posy) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<cam>)))
  "Returns string type for a message object of type '<cam>"
  "app/cam")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'cam)))
  "Returns string type for a message object of type 'cam"
  "app/cam")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<cam>)))
  "Returns md5sum for a message object of type '<cam>"
  "17343e858ce1baa8a0736fba55bdcc86")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'cam)))
  "Returns md5sum for a message object of type 'cam"
  "17343e858ce1baa8a0736fba55bdcc86")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<cam>)))
  "Returns full string definition for message of type '<cam>"
  (cl:format cl:nil "float64 posx~%float64 posy~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'cam)))
  "Returns full string definition for message of type 'cam"
  (cl:format cl:nil "float64 posx~%float64 posy~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <cam>))
  (cl:+ 0
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <cam>))
  "Converts a ROS message object to a list"
  (cl:list 'cam
    (cl:cons ':posx (posx msg))
    (cl:cons ':posy (posy msg))
))
