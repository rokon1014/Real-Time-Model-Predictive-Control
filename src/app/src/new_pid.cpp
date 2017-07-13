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



using namespace std;
using namespace ros;
double m ;
float cam_p;
float ode_p ;


//debug variables

float d_cam= 5;

//velocity 

double va1 = 0.0 ;
double va2 = 0.0 ;  // 1.75433
double va3 = 0.0 ; ; //1.5708
double va4 = 0.0 ; //1.4947
double va5 = 0.1;  //-0.0195477
double va6 = 0.0 ;;  // 2.04064
double ve1 = 0;  // -0.0160571
double final ;

double error;
double sum_error;
double dif_error;
double vel ;
double last_error;
double gain;
double kp,ki ;





double a1 = (86.17*3.1416)/180 ; // -0.350812
double a2 = (-75.79*M_PI)/180 ;  // 1.75433
double a3 = (-3.85*3.1416)/180 ; //1.5708
double a4 = (88.36*3.1416)/180 ;  //1.4947
double a5 = (82.11*3.1416)/180 ;   //-0.0195477
double a6 = (-23.30*3.1416)/180 ;  // 2.04064
double e1 = (-89.46*3.1416)/180 ;  // -0.0160571

double limit1 = (92*3.1416)/180 ;  // -0.0160571
double limit2 = (72*3.1416)/180 ;  // -0.0160571

double control ;
double final_control;



namespace cmd = ::boost::process;


void chatterCallback(const sensor_msgs::JointState& msg)
{

 m = msg.position[5];
ROS_INFO("From krc: [%f]", msg.position[5]*180/3.1416);
ROS_INFO("From krc: [%f]", a5*180/3.1416);
}

void odeCallback(const std_msgs::Float32& ode)
{

 ode_p = ode.data;
ROS_INFO("In ode: [%f]", ode.data);
 

}

void cameraCallback(const std_msgs::Float32& cam)
{

 cam_p = cam.data;
ROS_INFO("In camera: [%f]", cam.data);
 

}






int main(int argc, char **argv) {
	
	ros::init(argc, argv, "Simple");
	ros::NodeHandle nh { "Simple" };
	ros::NodeHandle n;

	ros::Rate loop_rate(10);



	ros::Subscriber sub_camera = n.subscribe("/from_camera", 1, cameraCallback);
	ros::Subscriber sub_ode = n.subscribe("/from_ode", 1, odeCallback);
	ros::Subscriber sub_krc = n.subscribe("/iros/pbd/dmp/JointPos", 1, chatterCallback);
	 




	std_srvs::Empty foo;
	KUKACommander::set_bool set_bool_true;
	set_bool_true.request.activate = true;
	




	ros::ServiceClient setControlModeClient = nh.serviceClient<KUKACommander::set_fri_ctrl>("/KUKACommander/setControlMode");
	ros::ServiceClient getCurrentStateClient = nh.serviceClient<KUKACommander::get_fri_state>("/KUKACommander/getCurrentState");
    ros::Publisher chatter_pub = n.advertise<sensor_msgs::JointState >("pid", 1);



	KUKACommander::set_fri_ctrl control_pos_mon;
	control_pos_mon.request.control = FRI_CTRL_POSITION;
	control_pos_mon.request.state = FRI_STATE_CMD;


	ros::spinOnce();  //first callback

	setControlModeClient.call(control_pos_mon); // setting control mode



	while (ros::ok())
{


    ros::spinOnce();



	KUKACommander::move_to_jnt_pos  direct;
	sensor_msgs::JointState msg;



		//if (d_cam>20)

		if (cam_p < 20 )
	{

		control = ((limit1 - a5) /20) *(20-cam_p)*2 ;
		//control = ((limit1 - a5) /20) *(d_cam-20) ;


		ROS_INFO(" limit1: %f , a5 %f control: [%f]",limit1, a5, control);
		final_control = a5+control;

		ROS_INFO("final control: [%f]", final_control*180/3.1416);


			if (limit2<final_control <limit1 )
			{


				if (abs((m-final_control)*180/3.1416) > .05 )   // to reach the position
				{

				final = ( final_control-m)*3;
				}

				else 
				{

				final = 0 ;
				}


			}


		msg.position = {va1,va2,ve1,va3,va4,final,va6};

		chatter_pub.publish(msg);

	}



//end of first if.

		//if (d_cam<=20)

		if (cam_p >=20)
	{
		control = ((a5-limit2)/20)*(cam_p-20)*2 ;
		//control = ((a5-limit2)/20)*(20-d_cam) ;
		final_control = a5 - control;

		ROS_INFO(" limit2: %f , a5 %f control: [%f]",limit2, a5, control);
		ROS_INFO("final control: [%f]", final_control*180/3.1416);


			if ( limit2<final_control <limit1 ) 
			{
					if (abs((m-final_control)*180/3.1416) > .05 )

					{
						final = (final_control -m)*3;

					}	

					else 
					{

						final = 0 ;
					}	

		msg.position = {va1,va2,ve1,va3,va4,final,va6};
		chatter_pub.publish(msg);
		

			}

	}	


	ros::spinOnce();
	loop_rate.sleep();	



}
	return 0;


}	







	

 