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
float ode_pid ;




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
double a5 = (75.43*3.1416)/180 ;   //-0.0195477
double a6 = (-23.30*3.1416)/180 ;  // 2.04064
double e1 = (-89.46*3.1416)/180 ;  // -0.0160571

double limit1 = (98*3.1416)/180 ;  // -0.0160571
double limit2 = (72*3.1416)/180 ;  // -0.0160571

double control ;
double final_control;

double last_pos = a5;

double last_camera = 0 ;


namespace cmd = ::boost::process;


void chatterCallback(const sensor_msgs::JointState& msg)
{

 m = msg.position[5];
//ROS_INFO("From krc: [%f]", msg.position[5]*180/3.1416);
//ROS_INFO("From krc: [%f]", a5*180/3.1416);
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

void odePidCallback(const std_msgs::Float32& odePid)
{

 ode_pid = odePid.data;
ROS_INFO("In ode pid: [%f]", odePid.data);
 cout<<odePid.data <<endl;

}







int main(int argc, char **argv) {
	
	ros::init(argc, argv, "Simple");
	ros::NodeHandle nh { "Simple" };
	ros::NodeHandle n;

	ros::Rate loop_rate(33);



	ros::Subscriber sub_camera = n.subscribe("/from_camera", 1, cameraCallback);
	ros::Subscriber sub_ode = n.subscribe("/from_ode", 1, odeCallback);
	ros::Subscriber sub_krc = n.subscribe("/iros/pbd/dmp/JointPos", 1, chatterCallback);
	ros::Subscriber sub_ode_pid = n.subscribe("/from_ode_pid", 1, odePidCallback);
	 




	std_srvs::Empty foo;
	KUKACommander::set_bool set_bool_true;
	set_bool_true.request.activate = true;
	




	ros::ServiceClient setControlModeClient = nh.serviceClient<KUKACommander::set_fri_ctrl>("/KUKACommander/setControlMode");
	ros::ServiceClient getCurrentStateClient = nh.serviceClient<KUKACommander::get_fri_state>("/KUKACommander/getCurrentState");
    ros::Publisher chatter_pub = n.advertise<sensor_msgs::JointState >("pid", 1);



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
	setControlModeClient.call(control_pos_mon); // setting control mode



	while (ros::ok())

//	for (int k = 0; k< 90 ; k++)
{

ros::Time begin = ros::Time::now();




    ros::spinOnce();



	KUKACommander::move_to_jnt_pos  direct;
	sensor_msgs::JointState msg;









kp = .035;
ki = .001 ;
kd =.4;



/*

kp = 1;
kd =.001;
ki = 1.9;

*/
error = (target -cam_p)/100 ;
//printf("error%f\n", error);

	//error = target -d_cam ;


sum_error = sum_error + (error * step) ;

  //printf("sum error%f\n", sum_error);

dif_error = (error-last_error)/step;

  //printf("diff error%f\n", dif_error);

vel = kp*error+ ki*sum_error+ kd*dif_error ;


 // printf("vel %f\n", vel);





// true postion m .

double degree = m*180/3.1416 ;



double change = degree -last_pos ;

double  ball_change = cam_p - last_camera ;

/*printf(" change angle : %f\n",change );
printf(" ball change %f\n",  ball_change);*/


last_pos = degree ;
last_camera = cam_p ;


//double d_m = 75 ;



//double degree = d_m ;



/*
printf("current angle %f\n",degree );
//printf("ball %f\n", cam_p );*/

printf("from ode %f\n",ode_pid );

//double desired_angle = (a5*180/3.1416) +vel;

double desired_angle = (a5*180/3.1416) + 5;


gain = .1;




double angle_error = (degree - desired_angle)*3.1416/180 ;



//double angle_error = (degree - desired_angle);



//double desired_vel = -angle_error * gain ;

double desired_vel = .5 ;
//printf(" desired velocity  %f\n", desired_vel );




//printf("desired angle %f\n",desired_angle);

//if (   desired_angle > 30 &&  desired_angle < 102 )
if (   degree> 30 &&  degree < 102 )

{


msg.position = {va1,va2,ve1,va3,va4,ode_pid*100,va6};
//msg.position = {va1,va2,ve1,va3,va4,desired_vel,va6};

		chatter_pub.publish(msg);
//		printf("inside \n");

		last_error = error ;


}		

else 
{
printf(" out  \n");
}
	ros::spinOnce();
	loop_rate.sleep();	

ros::Time end = ros::Time::now();
double dt = (begin - end).toSec();

//ROS_INFO("something");

	//ROS_INFO("dt %f", dt);

/*	int i = 0;
	loop : cin >> i;

		   if ( i != 1)
			   goto loop;*/

c = c+1;

//printf(" count : %d\n",c );
}
	return 0;


}	







	

 