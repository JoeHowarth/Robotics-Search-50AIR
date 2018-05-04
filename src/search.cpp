#include "ros/ros.h"
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf/transform_datatypes.h>
#include <std_msgs/Int8.h>

#include "coordinates.h"
#include "preprocess.h"
#include "movement.h"


Point curr_position = {0, 0, 0, 0, 0, "Current Position"};
bool isFound = false;

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

 void cameraCallback(const std_msgs::Int8::ConstPtr& msg) {
    if (msg->data < 2) { 
	    ROS_INFO("not seen %d", msg->data);
	    return;
    }
    isFound = true; 
    ROS_INFO("seen %d\n", msg->data);
    // 1 is not seen
    // not 1 is seen
}

int main(int argc, char *argv[])
{
    // ROS Setup
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;

    // Getting Current Position
    ros::Subscriber sub  = n.subscribe<geometry_msgs::PoseWithCovarianceStamped>("amcl_pose", 100, positionCallback);
    ros::Subscriber sub1 = n.subscribe<std_msgs::Int8>("/visp_auto_tracker/status", 100, cameraCallback);
    ros::Rate loop_rate(10);
    ros::spinOnce();    

    // Process Optimal Route with Full Coverage
    std::vector<Point> ordered = make_order(curr_position);

    // Movement
    int current_search_index = 1; // index of point to search next; skipping starting location
    bool isFound_routine = search_point();
    while (isFound_routine == false) {
	//ros::spinOnce();        
	if (go_to_next(ordered[current_search_index]) == false) { // false if unable to reach point
            ROS_INFO("Unable to reach point, skipping to next point\n");
        }
	    isFound_routine = search_point();
        if (isFound_routine || isFound) {
	        break;
        }
        if (current_search_index + 1 < ordered.size()) {
            current_search_index ++;
        } else {
            ROS_INFO("Finished Searching - Unable to Find Object :(\n");
            break;
        }
    }
    ROS_INFO("Object has been Found!\n");
    return 0;
}
