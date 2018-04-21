#include "ros/ros.h"
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include "coordinates.h"
#include "preprocess.h"
#include "movement.h"


Point curr_position = {0, 0, "Current Position"};

void positionCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg)
{
    curr_position.x = msg -> pose.pose.position.x;
    curr_position.y = msg -> pose.pose.position.y;
    curr_position.description = "Current Position";
}


int main(int argc, char *argv[])
{
    // ROS Setup
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;

    // Getting Initial Position
    n.subscribe<geometry_msgs::PoseWithCovarianceStamped>("amcl_pose", 1000, positionCallback);
    ros::Rate loop_rate(10);
    ros::spinOnce();
    
    // Process Optimal Route with Full Coverage
    std::vector<Point> ordered = make_order(curr_position);


    // Movement - TODO: currently written to follow standard path
    int current_search_index = 0; // index of point to search next
    bool isFound = false;
    while (isFound == false) {
        // isFound = search_point(); // function within that follows if anything of interest is seen
        if (go_to_next(ordered[current_search_index]) == false) { // false if unable to reach point
            ROS_INFO("Unable to reach point, skipping to next point\n");
        }
        // TODO - SEARCH CURRENT POINT
        if (current_search_index + 1 < ordered.size()) {
            current_search_index ++;
        } else {
            ROS_INFO("Finished Searching - Unable to Find Object :(\n");
            break;
        }
    }

    return 0;
}
