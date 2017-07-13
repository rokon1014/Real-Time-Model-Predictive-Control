/*
 * Record.cpp
 *
 *  Created on: April 1, 2015
 *      Author: Murtaza Hazara
 */

/*
 * SimpleTrajectoryImitation.cpp

 */


#include "ros/ros.h"
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
#include "KUKACommander/set_fri_ctrl.h"
#include "KUKACommander/joint_ptp_motion.h"
//#include "KUKACommander/move_to_cart_pos.h"
#include "KUKACommander/cart_ptp_motion.h"
#include "KUKACommander/get_fri_state.h"


using namespace std;
using namespace ros;

namespace cmd = ::boost::process;

bool start_pos_recorded = false;
vector<double> start_pos;
void record_start_pos(const sensor_msgs::JointState::ConstPtr& jnt_state);

int main(int argc, char **argv) {
	//ros::init(argc, argv, "iros/pbd/dmp/");
	ros::init(argc, argv, "SimplePTP");
	ros::NodeHandle nh { "SimplePTP" };
	
	// makes ros::spin/spinOnce obsolete
	ros::AsyncSpinner spinner(2); // Use 2 threads
	spinner.start();

	ROS_INFO("SimplePTP");

	std_srvs::Empty foo;
	KUKACommander::set_bool set_bool_true;
	set_bool_true.request.activate = true;
	
	// Create service clients to communicate with the KUKACommander
	ros::ServiceClient activateGravityCompensation = nh.serviceClient<KUKACommander::set_bool>("/KUKACommander/activateGravityCompensation");
	ros::ServiceClient stopGravityCompensation = nh.serviceClient<std_srvs::Empty>("/KUKACommander/stopGravityCompensation");
	ros::ServiceClient CartesianPTPMotion = nh.serviceClient<KUKACommander::cart_ptp_motion>("/KUKACommander/CartesianPTPMotion");
	ros::ServiceClient jointPTPMotion = nh.serviceClient<KUKACommander::joint_ptp_motion>("/KUKACommander/jointPTPMotion");	

	// Create communication objects with desired parameters
	KUKACommander::set_fri_ctrl control_pos_mon;
	control_pos_mon.request.control = FRI_CTRL_POSITION;
	control_pos_mon.request.state = FRI_STATE_CMD;



KUKACommander::get_fri_state getState;

	ros::ServiceClient setControlModeClient = nh.serviceClient<KUKACommander::set_fri_ctrl>("/KUKACommander/setControlMode");

	ros::ServiceClient getCurrentStateClient = nh.serviceClient<KUKACommander::get_fri_state>("/KUKACommander/getCurrentState");

	// Wait for services to show up
	if(!activateGravityCompensation.exists()) {
		ROS_INFO("Waiting for KUKACommander");
		activateGravityCompensation.waitForExistence(ros::Duration(2, 0));
		if(!activateGravityCompensation.exists()) {
			ROS_ERROR("KUKACommander didn't appear");
			return 0;
		}
		ROS_INFO("KUKACommander has started");
	}

	


	//while (ros::ok())

	//{


	bool cont = true;

		//setControlModeClient.call(control_pos_mon);
		 getCurrentStateClient.call(getState);

ROS_INFO(" state front  : [%d]", getState.response.state);


		KUKACommander::cart_ptp_motion Cartesian_motion_PTP;
		geometry_msgs::Pose  pose;
		geometry_msgs::Point position;
		geometry_msgs::Quaternion orientation;
	



	KUKACommander::joint_ptp_motion motion_srv;
		//boost::array<double, 7> pos={0.0,1.7,-1.74,0.0,0.0,0.0,0.0};

	float a1 = (-2.01*3.1416)/180 ; // -0.350812
float a2 = (100.0*M_PI)/180 ;  // 1.75433
float a3 = (90.00*3.1416)/180 ; //1.5708
float a4 = (85.64*3.1416)/180 ;  //1.4947
float a5 = (-1.12*3.1416)/180 ;   //-0.0195477
float a6 = (116.92*3.1416)/180 ;  // 2.04064
float e1 = (-0.92*3.1416)/180 ;  // -0.0160571

		boost::array<double, 7> pos={a1,a2,e1,a3,a4,a5,a6};
		
		motion_srv.request.position = pos;
		motion_srv.request.speed = 10;
		// Move to start position
		jointPTPMotion.call(motion_srv);
		ros::Duration(1).sleep();

		
//}
		


	//setControlModeClient.call(control_pos_mon);

	ROS_INFO(" state  back : [%d]", getState.response.state);

//}


	ROS_INFO("======================================");
	ROS_INFO("             Finished!");
	ROS_INFO("======================================");

	

	//spinner.stop();
	return 0;
}


