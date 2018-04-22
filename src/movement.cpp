#include "ros/ros.h"
#include "movement.h"

// Searches in all 4 directions at current (x,y) position
// Returns true if object is seen, else returns false
bool search_point() {
    for (int i = 0; i < 4; i++) {
        rotate(90.0);
    }
    // needs to publish when need to camera and processing to occur
    // must listen for when camera module responds
    return false;
}

void rotate(float degree) {
    // TODO: have to send twist message?
    ROS_INFO("Attempting to rotate %f  degrees", degree);
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "/base_link";

    goal.target_pose.pose.position.x =  0.0;
    goal.target_pose.pose.position.y =  0.0;
    goal.target_pose.pose.position.z =  0.0;
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(PI*degree/180.0);
    if (go_to_position(goal)) {
        ROS_INFO("Succesfully rotated to %f  degrees", degree);
    } else {
        ROS_INFO("Unsuccessfully rotated to %f degrees", degree);
    };
}

// Continues to the given unvisited waypoint
// Returns true while unvisited points exist, else returns false
bool go_to_next(Point next_location) {
    // Making goal
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "/map";

    goal.target_pose.pose.position.x =  next_location.x;
    goal.target_pose.pose.position.y =  next_location.y;
    goal.target_pose.pose.position.z =  0.0;
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(0.0);

    ROS_INFO("Navigating to %s", next_location.description.c_str());
    bool success = go_to_position(goal);
    return success;
}

// Robot moves to a given position
// Returns true if position is reached, else returns false
bool go_to_position(move_base_msgs::MoveBaseGoal goal) {
    ros::Rate r(10);
    Client nav_client("move_base", true);
    nav_client.waitForServer();
    ros::spinOnce();
    ros::Time begin = ros::Time::now();
    nav_client.sendGoal(goal);

    while (ros::ok()) {
        // Goal cancels if not reached with 30 seconds
        if (ros::Time::now().sec - begin.sec > 30) {
            nav_client.cancelGoal();
            return false;
        }

        if (nav_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            return true;
    }
}