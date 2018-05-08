#include "ros/ros.h"
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf/transform_datatypes.h>
#include <unistd.h>
#include "coordinates.h"
#include "preprocess.h"
#include "movement.h"
#include <thread>

void custom_spin()
{
  ros::spin();
}

Point curr_position = {0, 0, 0, 0, 0, "UNINITIALIZED"};

void positionCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg)
{
    curr_position.x = msg -> pose.pose.position.x;
    curr_position.y = msg -> pose.pose.position.y;
    tf::Quaternion q(msg -> pose.pose.orientation.x,
                     msg -> pose.pose.orientation.y,
                     msg -> pose.pose.orientation.z,
                     msg -> pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    m.getRPY(curr_position.roll, curr_position.pitch, curr_position.yaw);
    curr_position.description = "Current Position";

    //ROS_INFO("Callback output: %lf %lf", curr_position.x,
    //	     curr_position.y);
}

int main(int argc, char *argv[])
{
    // ROS Setup
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;

    // Getting Current Position
    ros::Subscriber sub = n.subscribe<geometry_msgs::PoseWithCovarianceStamped>("amcl_pose", 100, positionCallback);

    ros::Rate loop_rate(10);
  
    std::thread spinning(custom_spin);
    usleep(1000000);
    // Process Optimal Route with Full Coverage
    std::vector<Point> ordered = make_order(curr_position);
    
    if (search()) ROS_INFO("THAT WAS EASY\n");

    for (auto it = ordered.begin()+1; it != ordered.end(); ++it) {
            go_to_point(*it);
            if (search()) ROS_INFO("OBJECT FOUND!!!!\n");
    }

    ROS_INFO("UNABLE TO FIND OBJECT AFTER LOOKING ALL POINTS\n");
  
    spinning.join();         
    return 0;
}
