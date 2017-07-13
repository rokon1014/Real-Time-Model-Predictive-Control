#include "ros/ros.h"
#include <cstdlib>
#include <ros/callback_queue.h>
#include <unit/from_robot.h>




 unit::from_robot info;
 ros::CallbackQueue my_callback_queue;


int main(int argc, char **argv)
{


  ros::init(argc, argv, "client");
ros::NodeHandle n;
ros::NodeHandle nh;


nh.setCallbackQueue(&my_callback_queue);

ros::AsyncSpinner spinner(0, &my_callback_queue);
 spinner.start();

  ros::ServiceClient client = n.serviceClient<unit::from_robot>("from_robot");

  info.request.position = 5;
  info.request.angle = 10.4;

while (ros::ok())
{

    if (client.call(info))
  {
    ROS_INFO("command: %f", info.response.command);

  }
  else
  {
    ROS_ERROR("Failed to call service ");
    return 1;
  }
}


  return 0;
}