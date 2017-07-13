
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
#include "KUKACommander/cart_ptp_motion.h"
#include "KUKACommander/get_fri_state.h"
#include "KUKACommander/move_to_jnt_pos.h"
#include <std_msgs/Float32.h>
#include <std_msgs/Int16.h>
#include "sensor_msgs/JointState.h"
#include <unit/from_robot.h>
#include <iostream>
#include <fstream>




using namespace std;
using namespace ros;
float m ;
float cam_p;
float ode_p ;
float ode_pid ;
float control ;

unit::from_robot info;


//debug variables

float d_cam= .1;

//velocity 

double va1 = 0.0 ;
double va2 = 0.0 ;  // 1.75433
double va3 = 0.0 ; ; //1.5708
double va4 = 0.0 ; //1.4947
double va5 = 0.1;  //-0.0195477
double va6 = 0.0 ;;  // 2.04064
double ve1 = 0;  // -0.0160571
double final ;


double target = 20;
double error= 0;
double sum_error= 0;
double dif_error= 0;
double vel ;
double last_error= 0;
double gain ;
double kp ;
double kd ;
double ki ;
double step = .01 ;
int c = 0;


double a1 = (86.17*3.1416)/180 ; // -0.350812
double a2 = (-75.79*M_PI)/180 ;  // 1.75433
double a3 = (-3.85*3.1416)/180 ; //1.5708
double a4 = (88.36*3.1416)/180 ;  //1.4947
double a5 = (74.85*3.1416)/180 ;   //-0.0195477
double a6 = (-23.30*3.1416)/180 ;  // 2.04064
double e1 = (-89.46*3.1416)/180 ;  // -0.0160571

double limit1 = (98*3.1416)/180 ;  // -0.0160571
double limit2 = (72*3.1416)/180 ;  // -0.0160571

int tick= 0; 


namespace cmd = ::boost::process;


void chatterCallback(const sensor_msgs::JointState& msg)
{

	m = msg.position[5];
	ROS_INFO("From krc: [%f]", msg.position[5]*180/3.1416);
	//ROS_INFO("From krc: [%f]", a5*180/3.1416);
}

void odeCallback(const std_msgs::Float32& ode)
{

 	ode_p = ode.data;
	//ROS_INFO("In ode: [%f]", ode.data);
 

}

void cameraCallback(const std_msgs::Float32& cam)
{

 	cam_p = cam.data;
	ROS_INFO("In camera: [%f]", cam.data);
 

}








int main(int argc, char **argv) 
{
	
	ros::init(argc, argv, "Simple");
	ros::NodeHandle nh { "Simple" };
	ros::NodeHandle n;
	ros::Rate loop_rate(24);



	std_srvs::Empty foo;
	KUKACommander::set_bool set_bool_true;
	set_bool_true.request.activate = true;


	//subscribers
	ros::Subscriber sub_camera = n.subscribe("/from_camera", 1, cameraCallback);
	ros::Subscriber sub_ode = n.subscribe("/from_ode", 1, odeCallback);
	ros::Subscriber sub_krc = n.subscribe("/iros/pbd/dmp/JointPos", 1, chatterCallback);
	
	 

    

  	//clients  

    ros::ServiceClient client = n.serviceClient<unit::from_robot>("from_robot");
	ros::ServiceClient setControlModeClient = nh.serviceClient<KUKACommander::set_fri_ctrl>("/KUKACommander/setControlMode");
	ros::ServiceClient getCurrentStateClient = nh.serviceClient<KUKACommander::get_fri_state>("/KUKACommander/getCurrentState");

	KUKACommander::set_fri_ctrl control_pos_mon;
	control_pos_mon.request.control = FRI_CTRL_POSITION;
	control_pos_mon.request.state = FRI_STATE_CMD;



	//camera warmup

	for (int j = 0 ; j< 50; j++)

	{
		ros::spinOnce();  //first callback
		printf("%d\n",j );
		printf("%f\n",cam_p );
		loop_rate.sleep();	
	}



	//setting control mode
	setControlModeClient.call(control_pos_mon); 

		//publishers
   	ros::Publisher chatter_pub = n.advertise<sensor_msgs::JointState >("pid", 1);



	//while (ros::ok())
	for (int s=0 ; s<50; s++)
	{
	

		ros::Time begin = ros::Time::now();

		ros::spinOnce();

		double ball_pos;
		ball_pos = (cam_p-20)/100 ;

		printf("ball pos %f \n", ball_pos );


		info.request.position = ball_pos;
		info.request.angle = m-a5;




		if (client.call(info))
		{
			//ROS_INFO("command: %f", info.response.command);

		}
		else
		{
			ROS_ERROR("Failed to call service ");
			return 1;
		}





		

		sensor_msgs::JointState msg;
		float degree = m*180/3.1416 ;
		double  desired_vel = info.response.command  ;	

		if (tick< 25)
		{

	
			//if (degree> 70 &&  degree < 110 )

			//{
				msg.position = {va1,va2,ve1,va3,va4,0.1,va6};
				chatter_pub.publish(msg);
				tick = tick+1 ;
				ROS_INFO("velocity 0.1 positive \n" );
			//}		

			//else 
			//{

			//	msg.position = {va1,va2,ve1,va3,va4,0.0,va6};	
			//	printf(" out  \n");
				//chatter_pub.publish(msg);
			//}

		}

		else 
		{

			//if (degree> 70 &&  degree < 110 )

			//{
		
				msg.position = {va1,va2,ve1,va3,va4,-0.1,va6};

				chatter_pub.publish(msg);
				tick = tick+1 ;
				ROS_INFO("velocity -0.1 Negative \n");
			//}		
	
			//else 
			//{

			//	msg.position = {va1,va2,ve1,va3,va4,0.0,va6};	
				//printf(" out  \n");
				//chatter_pub.publish(msg);
			//}


		}

		
		loop_rate.sleep();	

		ros::Time end = ros::Time::now();
		double dt = (begin - end).toSec();

		ROS_INFO("dt %f", dt);
		ROS_INFO("tick %d", tick);
	
	}

	return 0;


}	







	

 
