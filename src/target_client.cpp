#include <ros/ros.h>
#include "path_test/update_goal.h"
#include <nav_msgs/Path.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "target_client");
    ros::NodeHandle nh;
    ros::NodeHandle private_nh("~");
    ros::ServiceClient client= nh.serviceClient<path_test::update_goal>("update_goal");
    ros::Publisher pub = nh.advertise<nav_msgs::Path>("base_path", 1000);

    int target_x,target_y;
    double target_yaw;
    private_nh.param<int>("target_x", target_x, 5);
    private_nh.param<int>("target_y", target_y, 5);
    private_nh.param<double>("target_yaw", target_yaw, 0);

    path_test::update_goal srv;
    srv.request.x = target_x;
    srv.request.y = target_y;
    srv.request.yaw = target_yaw;

    nav_msgs::Path path;
    
    if(client.call(srv))
    {
        path = srv.response.path;
    }

    ros::Rate loop_rate(0.5); 
    while (ros::ok())
    {
        pub.publish(path);
        loop_rate.sleep();
        ros::spinOnce(); 
    }
    return 0;
}