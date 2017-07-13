#include "ros/ros.h"
#include "sensor_msgs/JointState.h"
#include "std_msgs/Int16.h"

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const std_msgs::Int16& msg)
{
  ROS_INFO("I heard: [%d]", msg.data);
ROS_INFO("I heard nothing");
}



int main(int argc, char **argv)
{



  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

    //ros::Rate loop_rate(1);

//while (ros::ok())
//{


   ros::Subscriber sub = n.subscribe("/cha", 1000, chatterCallback);
ROS_INFO("I did something");


   ros::spin();

    //loop_rate.sleep();

//}

  return 0;
}
