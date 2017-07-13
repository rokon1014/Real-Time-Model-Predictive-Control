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

 unit::from_robot info;


//debug variables

float d_cam= .2;

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








int main(int argc, char **argv) {
	
	ros::init(argc, argv, "Simple");
	ros::NodeHandle nh { "Simple" };
	ros::NodeHandle n;

	ros::Rate loop_rate(33);


	std_srvs::Empty foo;
	KUKACommander::set_bool set_bool_true;
	set_bool_true.request.activate = true;


//subscribers
	ros::Subscriber sub_camera = n.subscribe("/from_camera", 1, cameraCallback);
	ros::Subscriber sub_ode = n.subscribe("/from_ode", 1, odeCallback);
	ros::Subscriber sub_krc = n.subscribe("/iros/pbd/dmp/JointPos", 1, chatterCallback);
	
	 

//publishers
    ros::Publisher chatter_pub = n.advertise<sensor_msgs::JointState >("pid", 1);
    


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




//for (int k= 0 ; k<10; k++)

	while (ros::ok())
{

ros::Time begin = ros::Time::now();


info.request.position = cam_p;
info.request.angle = m;


    if (client.call(info))
  {
    ROS_INFO("command: %f", info.response.command);

  }
  else
  {
    ROS_ERROR("Failed to call service ");
    return 1;
  }




printf("a5 %f\n",a5 );
  control = a5 +  (info.response.command) ;

  printf("control %f\n", control  );

  ros::spinOnce();

float degree = (m*180)/3.1416;

  double angle_error = (m - control) ;
printf("error %f\n", angle_error );


sensor_msgs::JointState msg;




double desired_vel = angle_error/0.03;

printf(" des vel %f\n",desired_vel );
printf(" current angle  %f\n", m );






//if (   desired_angle > 30 &&  desired_angle < 102 )
if (   degree> 30 &&  degree < 102 )

{
msg.position = {va1,va2,ve1,va3,va4,desired_vel,va6};
chatter_pub.publish(msg);
}		

else 
{

msg.position = {va1,va2,ve1,va3,va4,0.0,va6};	
printf(" out  \n");
}



	ros::spinOnce();
	loop_rate.sleep();	

ros::Time end = ros::Time::now();
double dt = (begin - end).toSec();

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







	

 