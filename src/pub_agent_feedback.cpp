#include <ros/ros.h>
#include <iostream>
#include "path_test/agent_feedback.h"

using namespace std;

int main(int argc,char **argv)
{
    ros::init(argc,argv,"pub_agent_feedback");
    ros::NodeHandle nh;
    ros::NodeHandle private_nh("~");
    
    std::string id;
    int start_x,start_y;
    double start_yaw;

    private_nh.param<std::string>("id", id, "agent_1");
    private_nh.param<int>("start_x", start_x, 0);
    private_nh.param<int>("start_y", start_y, 0);
    private_nh.param<double>("start_yaw", start_yaw, 0);

    ros::Publisher pub = nh.advertise<path_test::agent_feedback>("agent_feedback", 1000);
    
    ros::Rate loop_rate(10);           
	while (ros::ok()) 
	{ 
        path_test::agent_feedback msg;
        msg.x = start_x;
        msg.y = start_y;
        msg.yaw = start_yaw;
        msg.serial_id = id;
        
		pub.publish(msg);
	    ros::spinOnce(); 
		loop_rate.sleep(); 
	 } 
    return 0;
}
