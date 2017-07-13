#include "ros/ros.h"
#include <Types.hpp>
#include "ros/console.h"

#include "iostream"
#include "signal.h"

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <boost/array.hpp>

#include "std_msgs/Empty.h"
#include "sensor_msgs/JointState.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Vector3.h"
#include "JointPositions.h"
#include "fricomm.h"

#include <std_srvs/Empty.h>
#include "KUKACommander/set_bool.h"
#include "KUKACommander/get_bool.h"
#include "KUKACommander/set_fri_ctrl.h"
#include "KUKACommander/joint_ptp_motion.h"
//#include "KUKACommander/move_to_cart_pos.h"
#include "KUKACommander/cart_ptp_motion.h"
#include "KUKACommander/get_fri_state.h"
#include "KUKACommander/move_to_jnt_pos.h"

#include <std_msgs/Float32.h>

#include <std_msgs/Int16.h>
#include "sensor_msgs/JointState.h"
#include <unit/from_robot.h>

using namespace std;
using namespace ros;
float m ;
float cam_p;
float ode_p ;
float ode_pid ;
float control ;


float last_pixel ;
float current_pixel;
float vel;



double va1 = 0.0 ;
double va2 = 0.0 ;  // 1.75433
double va3 = 0.0 ; ; //1.5708
double va4 = 0.0 ; //1.4947
double va5 = 0.0;  //-0.0195477
double va6 = 0.0 ;;  // 2.04064
double ve1 = 0;  // -0.0160571
double final ;


void cameraCallback(const std_msgs::Float32& cam)
{

 cam_p = cam.data;
//ROS_INFO("In camera: [%f]", cam.data);
 

}


int main(int argc, char **argv) {
	
	ros::init(argc, argv, "Simple");
	ros::NodeHandle nh { "Simple" };
	ros::NodeHandle n;

	ros::Rate loop_rate(40);


	std_srvs::Empty foo;
	KUKACommander::set_bool set_bool_true;
	set_bool_true.request.activate = true;


    ros::Subscriber sub_camera = n.subscribe("/from_camera_fancy", 1, cameraCallback);

	//ros::Subscriber sub_krc = n.subscribe("/iros/pbd/dmp/JointPos", 1, chatterCallback);


	 ros::Publisher chatter_pub = n.advertise<sensor_msgs::JointState >("pid", 1);

	 ros::ServiceClient setControlModeClient = nh.serviceClient<KUKACommander::set_fri_ctrl>("/KUKACommander/setControlMode");

	KUKACommander::set_fri_ctrl control_pos_mon;
	control_pos_mon.request.control = FRI_CTRL_POSITION;
	control_pos_mon.request.state = FRI_STATE_CMD;



for (int j = 0 ; j< 50; j++)

{
	ros::spinOnce();  //first callback

	printf("%d\n",j );
	printf("%f\n",cam_p );
	loop_rate.sleep();	
}


setControlModeClient.call(control_pos_mon); 



while (ros::ok())
{
  
   current_pixel = cam_p ;

   printf("current %f, last %f\n", current_pixel, last_pixel );

   if (current_pixel > last_pixel+3)

   {
   	 vel = -0.3;
   }

   if (current_pixel < last_pixel-3)
   {
   	 vel =  0.3;
   }

     if (current_pixel ==last_pixel)
   {
   	 vel = 0 ;
   }



 printf(" vel %f\n", vel);

 sensor_msgs::JointState msg;


 msg.position = {vel,va2,ve1,va3,va4,va5,va6};
chatter_pub.publish(msg);



last_pixel = current_pixel ;
ros::spinOnce();
	loop_rate.sleep();	

	
}
	return 0;


}	
