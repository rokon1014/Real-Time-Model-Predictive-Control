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
 #include "KUKACommander/get_bool.h"
#include "KUKACommander/set_fri_ctrl.h"
#include "KUKACommander/joint_ptp_motion.h"
//#include "KUKACommander/move_to_cart_pos.h"
#include "KUKACommander/cart_ptp_motion.h"
#include "KUKACommander/get_fri_state.h"

 #include "KUKACommander/move_to_jnt_pos.h"



 #include <std_msgs/Int16.h>


using namespace std;
using namespace ros;

/*
double a1 = (90*3.1416)/180 ; // -0.350812
double a2 = (20.0*M_PI)/180 ;  // 1.75433
double a3 = (-100.00*3.1416)/180 ; //1.5708
double a4 = (0*3.1416)/180 ;  //1.4947
double a5 = (0*3.1416)/180 ;   //1.57
double a6 = (0*3.1416)/180 ;  // 2.04064
double e1 = (0*3.1416)/180 ;  // -0.0160571
*/

/*
double a1 = (-2.01*3.1416)/180 ; // -0.350812
double a2 = (100.0*M_PI)/180 ;  // 1.75433
double a3 = (90.00*3.1416)/180 ; //1.5708
double a4 = (85.64*3.1416)/180 ;  //1.4947
double a5 = (-1.12*3.1416)/180 ;   //-0.0195477
double a6 = (116.92*3.1416)/180 ;  // 2.04064
double e1 = (-0.92*3.1416)/180 ;  // -0.0160571
*/

/*

double a1 = (87.40*3.1416)/180 ; // -0.350812
double a2 = (-84.74*M_PI)/180 ;  // 1.75433
double a3 = (-4.07*3.1416)/180 ; //1.5708
double a4 = (88.36*3.1416)/180 ;  //1.4947
double a5 = (94.48*3.1416)/180 ;   //-0.0195477
double a6 = (-63.68*3.1416)/180 ;  // 2.04064
double e1 = (-89.46*3.1416)/180 ;  // -0.0160571
*/




double a1 = 0.0 ;
double a2 = 0.0 ;  // 1.75433
double a3 = 0.0 ; ; //1.5708
double a4 = 0.0 ; //1.4947
double a5 = 0.05 ;  //-0.0195477
double a6 = 0.0 ;;  // 2.04064
double e1 = 0;  // -0.0160571
namespace cmd = ::boost::process;

bool start_pos_recorded = false;
vector<double> start_pos;
void record_start_pos(const sensor_msgs::JointState::ConstPtr& jnt_state);



bool cont = true;

int m ;


void chatterCallback(const std_msgs::Int16 msg)
{

 m = msg.data;
 //ROS_INFO("In chatter: [%f]", msg.position[5]);

}


int main(int argc, char **argv) {
	//ros::init(argc, argv, "iros/pbd/dmp/");
	ros::init(argc, argv, "SimplePTP");
	ros::NodeHandle nh { "SimplePTP" };
	ros::NodeHandle n;
	

	ros::Rate rate(.08);

	 ROS_INFO("check");
     ros::Subscriber sub = n.subscribe("/cha", 1, chatterCallback);
     ros::spinOnce();
	// makes ros::spin/spinOnce obsolete
	//ros::AsyncSpinner spinner(2); // Use 2 threads
	//spinner.start();

	ROS_INFO("SimplePTP");

	std_srvs::Empty foo;
	KUKACommander::set_bool set_bool_true;
	set_bool_true.request.activate = true;
	
	// Create service clients to communicate with the KUKACommander
	ros::ServiceClient activateGravityCompensation = nh.serviceClient<KUKACommander::set_bool>("/KUKACommander/activateGravityCompensation");
	ros::ServiceClient stopGravityCompensation = nh.serviceClient<std_srvs::Empty>("/KUKACommander/stopGravityCompensation");
	ros::ServiceClient CartesianPTPMotion = nh.serviceClient<KUKACommander::cart_ptp_motion>("/KUKACommander/CartesianPTPMotion");
	ros::ServiceClient jointPTPMotion = nh.serviceClient<KUKACommander::joint_ptp_motion>("/KUKACommander/jointPTPMotion");	


	ros::ServiceClient moveToJointPosition = nh.serviceClient<KUKACommander::move_to_jnt_pos >("/KUKACommander/moveToJointPosition");	
ros::ServiceClient moveToPosition = nh.serviceClient<KUKACommander::move_to_jnt_pos >("/Commander/moveToPosition");	

	ros::ServiceClient isMovingClient = nh.serviceClient<KUKACommander::get_bool>("/KUKACommander/isMoving");	

	// Create communication objects with desired parameters
	KUKACommander::set_fri_ctrl control_pos_mon;
	control_pos_mon.request.control = FRI_CTRL_POSITION;
	control_pos_mon.request.state = FRI_STATE_CMD;



	KUKACommander::get_fri_state getState;

	KUKACommander::get_bool  moving ;


    



	ros::ServiceClient setControlModeClient = nh.serviceClient<KUKACommander::set_fri_ctrl>("/KUKACommander/setControlMode");

	ros::ServiceClient getCurrentStateClient = nh.serviceClient<KUKACommander::get_fri_state>("/KUKACommander/getCurrentState");


/*
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

	*/


	//while (ros::ok())

	//{


		//ros::spinOnce();

		int newm = m ; 
        // rate.sleep();
		setControlModeClient.call(control_pos_mon);
		 getCurrentStateClient.call(getState);

		 isMovingClient.call(moving);



   		//ROS_INFO(" state front  : [%d]", getState.response.state);
		//ROS_INFO(" moving  : [%d]", moving.response.answer);
		//ROS_INFO(" moving or not  : [%d]", m);


		//if (m==0 && cont )   

		//	if ( cont )  
		//{
			
			//ros::Duration(1).sleep();
         KUKACommander::joint_ptp_motion motion_srv;


         KUKACommander::move_to_jnt_pos  direct;
		//boost::array<double, 7> pos={0.0,1.7,-1.74,0.0,0.0,0.0,0.0};

		//boost::array<double, 7> pos={0.0,1.25,0.0,0.0,0.0,1.5,2.5};

		boost::array<double, 7> pos={a1,a2,e1,a3,a4,a5,a6};

		
		motion_srv.request.position = pos;
		motion_srv.request.speed = 15;



      direct.request.position = pos;
		direct.request.time = 14;

		// Move to start position
		//jointPTPMotion.call(motion_srv);



//while (ros::ok())
//{
		//moveToJointPosition.call(direct);

		
//moveToPosition.call(direct);
		//a5 = a5 +.1 ;

		
		ROS_INFO(" inside");
//}

		//}

		//ros::spinOnce();

		int lastm = m;

		if (newm ==lastm)

			{cont = false ;}

		else { cont = true ;}

//ROS_INFO(" moving or not  : [%d]", m);
  
	
		
//}
		


	//setControlModeClient.call(control_pos_mon);

	//ROS_INFO(" state  back : [%d]", getState.response.state);

//}


	ROS_INFO("======================================");
	ROS_INFO("             Finished!");
	ROS_INFO("======================================");

	

	//spinner.stop();
	return 0;
}


