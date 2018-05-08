#include "ros/ros.h"
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf/transform_datatypes.h>

#include "coordinates.h"
#include "preprocess.h"
#include "movement.h"


Point curr_position = {0, 0, 0, 0, 0, "Current Position"};

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
}

int main(int argc, char *argv[])
{
    // ROS Setup
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;

    // Getting Current Position
    n.subscribe<geometry_msgs::PoseWithCovarianceStamped>("amcl_pose", 100, positionCallback);
    ros::Rate loop_rate(10);
    ros::spinOnce();
    
    // Process Optimal Route with Full Coverage
    std::vector<Point> ordered = make_order(curr_position);

    if (search()) ROS_INFO("THAT WAS EASY\n");

    for (auto it = ordered.begin()+1; it != ordered.end(); ++it) {
            go_to_point(p);
            if (search()) ROS_INFO("OBJECT FOUND!!!!\n");
    }

    ROS_INFO("UNABLE TO FIND OBJECT AFTER LOOKING ALL POINTS\n");
           
    return 0;
}
