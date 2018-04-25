#include "ros/ros.h"
#include "movement.h"
#include <cassert>

// Searches in all 4 directions at current (x,y) position
// Returns true if object is seen, else returns false

extern Point curr_position;

void rotate(float degree) {
    // TODO: have to send twist message?
    ROS_INFO("Attempting to rotate %f  degrees", degree);
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "/base_link";

    goal.target_pose.pose.position.x =  0.0;
    goal.target_pose.pose.position.y =  0.0;
    goal.target_pose.pose.position.z =  0.0;

    goal.target_pose.pose.orientation =
            tf::createQuaternionMsgFromYaw(PI*degree/180.0);
    if (go_to_position(goal)) {
        ROS_INFO("Succesfully rotated to %f  degrees", degree);
    } else {
        ROS_INFO("Unsuccessfully rotated to %f degrees", degree);
    }
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

static move_base_msgs::MoveBaseGoal move_forward_goal()
{
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "/base_link";
        
        goal.target_pose.pose.position.x =  1.0;
        goal.target_pose.pose.position.y =  0.0;
        goal.target_pose.pose.position.z =  0.0;
        goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(0);
        return goal;
}

bool search_point(Point curr)
{
        for (int i = 0; i < 4; ++i) {
                rotate(90);

                float prob = rand() % 100 /100.0; //INSERT EVALUATE FUNCTION

                if (prob > MIN_FOUND_PROBABILITY)
                        return true;
                else if (prob > MIN_SEARCH_PROBABILITY) {
                        go_to_position(move_forward_goal());
                        search_point(curr_position);
                        go_to_next(curr);
                }
        }
        return false;
}
