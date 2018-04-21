#include "ros/ros.h"
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include "coordinates.h"
#include "preprocess.h"
#include "movement.h"


Point curr_position = {0, 0, "current position"};

void positionCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg)
{
    curr_position.x = msg -> pose.pose.position.x;
    curr_position.y = msg -> pose.pose.position.y;
    curr_position.description = "Starting Position";
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


    // Movement
    // bool isFound = false;
    // while (isFound == false) {
    //     isFound = search_point(); // function within that follows if anything of interest is seen
    //     if (go_to_next()) { // go_to_next returns false if no more places to go, else returns true
    //         ROS_INFO("Finished Searching - Unable to Find Object :(\n");
    //         break;
    //     };
    // }


    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "/map";

    goal.target_pose.pose.position.x =  -4.873;
    goal.target_pose.pose.position.y =  1.701;
    goal.target_pose.pose.position.z =  0.0;
    double yaw = 0.0;
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(yaw);


    go_to_position(goal);
    // while true:
        // go on standard path
        // at each point - call vision module at 45 degree increments
        //     if value returned is beyond threshold, follow in tree (DFS)

    return 0;
}
