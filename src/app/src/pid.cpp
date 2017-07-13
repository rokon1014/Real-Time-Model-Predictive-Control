
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
bool toogle = false;



double va1 = 0.0 ;
double va2 = 0.0 ;  // 1.75433
double va3 = 0.0 ; ; //1.5708
double va4 = 0.0 ; //1.4947
double va5 = 0.1;  //-0.0195477
double va6 = 0.0 ;;  // 2.04064
double ve1 = 0;  // -0.0160571
double final ;



double a1 = (86.17*3.1416)/180 ; // -0.350812
double a2 = (-75.79*M_PI)/180 ;  // 1.75433
double a3 = (-3.85*3.1416)/180 ; //1.5708
double a4 = (88.36*3.1416)/180 ;  //1.4947
double a5 = (100.95*3.1416)/180 ;   //-0.0195477
double a6 = (-23.30*3.1416)/180 ;  // 2.04064
double e1 = (-89.46*3.1416)/180 ;  // -0.0160571

double limit1 = (95*3.1416)/180 ;  // -0.0160571
double limit2 = (42*3.1416)/180 ;  // -0.0160571


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
	//ros::init(argc, argv, "iros/pbd/dmp/");
	ros::init(argc, argv, "Simple");
	ros::NodeHandle nh { "Simple" };
	ros::NodeHandle n;

ros::Rate loop_rate(1);



	ros::Subscriber sub_camera = n.subscribe("/from_camera", 1, cameraCallback);
	ros::Subscriber sub_ode = n.subscribe("/from_ode", 1, odeCallback);
	ros::Subscriber sub_krc = n.subscribe("/iros/pbd/dmp/JointPos", 1, chatterCallback);
	 


	std_srvs::Empty foo;
	KUKACommander::set_bool set_bool_true;
	set_bool_true.request.activate = true;


	ros::ServiceClient activateGravityCompensation = nh.serviceClient<KUKACommander::set_bool>("/KUKACommander/activateGravityCompensation");
	ros::ServiceClient stopGravityCompensation = nh.serviceClient<std_srvs::Empty>("/KUKACommander/stopGravityCompensation");
	ros::ServiceClient moveToJointPosition = nh.serviceClient<KUKACommander::move_to_jnt_pos >("/KUKACommander/moveToJointPosition");	


	ros::ServiceClient setControlModeClient = nh.serviceClient<KUKACommander::set_fri_ctrl>("/KUKACommander/setControlMode");
	ros::ServiceClient getCurrentStateClient = nh.serviceClient<KUKACommander::get_fri_state>("/KUKACommander/getCurrentState");
  ros::Publisher chatter_pub = n.advertise<sensor_msgs::JointState >("pid", 1);



	KUKACommander::set_fri_ctrl control_pos_mon;
	control_pos_mon.request.control = FRI_CTRL_POSITION;
	control_pos_mon.request.state = FRI_STATE_CMD;
	

/*
	if(!activateGravityCompensation.exists()) {
		ROS_INFO("Waiting for KUKACommander");
		activateGravityCompensation.waitForExistence(ros::Duration(2, 0));
		if(!activateGravityCompensation.exists()) {
			ROS_ERROR("KUKACommander didn't appear");
			return 0;
		}
		ROS_INFO("KUKACommander has started");
	}
*/
ros::spinOnce();
	
setControlModeClient.call(control_pos_mon);

//sleep(4);


while (ros::ok())
{


ros::spinOnce();


//sleep(.8);
KUKACommander::move_to_jnt_pos  direct;

sensor_msgs::JointState msg;
//	if (toogle)

if (cam_p > 20 )
{
	


	a5 = a5 -.0349 ;

	if ( limit2<a5 <limit1  )

	{

ROS_INFO("diff: [%f]", (a5-m)*180/3.1416);

if (abs((m-a5)*180/3.1416) > .05 )
{

final = (a5 -m)*.1;


}

else 
{

	final = 0 ;
}


}
	else 

	{


		final = 0 ;
	}

	 msg.position = {va1,va2,ve1,va3,va4,final,va6};

	//moveToJointPosition.call(direct);


	 chatter_pub.publish(msg);
	toogle= false;
	

}




if (cam_p <=20)


{
	setControlModeClient.call(control_pos_mon);

	a5 = a5+.0649 ;
	//boost::array<double, 7> pos={a1,a2,e1,a3,a4,a5,a6};


	//moveToJointPosition.call(direct);

	if ( limit2<a5 <limit1 )

	{

	ROS_INFO("diff: [%f]", (a5-m)*180/3.1416);

if (abs((m-a5)*180/3.1416) > .05 )
{

final = (a5 -m)*.1;


}

else 
{

	final = 0 ;
}


}
else {

	final = 0;

}

	msg.position = {va1,va2,ve1,va3,va4,final,va6};


	 chatter_pub.publish(msg);


}



//else 
//{

//sleep(.8);
/*
	setControlModeClient.call(control_pos_mon);
	boost::array<double, 7> pos1={a1,a2,e1,a3,a4,a5,a6};


	direct.request.position = pos1;
	direct.request.time = .08;

	moveToJointPosition.call(direct);
	toogle= true;

cout<<"secondSS"<<endl;
*/
//}

ros::spinOnce();
 loop_rate.sleep();


}
	return 0;


}