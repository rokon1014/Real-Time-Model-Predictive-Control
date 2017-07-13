
(cl:in-package :asdf)

(defsystem "unit-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "for_feedback" :depends-on ("_package_for_feedback"))
    (:file "_package_for_feedback" :depends-on ("_package"))
    (:file "for_double" :depends-on ("_package_for_double"))
    (:file "_package_for_double" :depends-on ("_package"))
    (:file "from_robot" :depends-on ("_package_from_robot"))
    (:file "_package_from_robot" :depends-on ("_package"))
    (:file "twoBall" :depends-on ("_package_twoBall"))
    (:file "_package_twoBall" :depends-on ("_package"))
  ))