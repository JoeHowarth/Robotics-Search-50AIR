#include "ros/ros.h"
#include "movement.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <time.h>
#include <string>

// Searches in all directions at current (x,y) position
// Returns if object is seen

const float MIN_PROB = 0.4;
const float MAX_PROB = 0.8;

float check_image() {
  return confidence;
}

bool search() {
	for (double degree = 0; degree < 421; degree += 45) {
                rotate(45.0);
                float probability_found = check_image();
                if (probability_found < MIN_PROB) continue;
                if (probability_found > MAX_PROB) return true;

                Point curr = curr_position;                
                go_to_goal(move_forward_goal());
                
                if (search()) return true; // recurse

                go_to_point(curr);                
        }
        return false;
}

void rotate(float degree) { 
    ROS_INFO("Attempting to rotate %f  degrees", degree);
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "/base_link";

    goal.target_pose.pose.position.x =  0.0;
    goal.target_pose.pose.position.y =  0.0;
    goal.target_pose.pose.position.z =  0.0;

    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(PI*degree/180.0);
    if (go_to_goal(goal)) {
        ROS_INFO("Succesfully rotated to %f degrees", degree);
    } else {
        ROS_INFO("Unsuccessfully rotated to %f degrees", degree);
    };
    //    ros::Duration(0.5).sleep();
}

bool go_to_point(Point next_location) {
    // Making goal
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "/map";

    goal.target_pose.pose.position.x =  next_location.x;
    goal.target_pose.pose.position.y =  next_location.y;
    goal.target_pose.pose.position.z =  0.0;
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(next_location.yaw);

    ROS_INFO("Navigating to %s", next_location.description.c_str());
    bool success = go_to_goal(goal);
    return success;
}

// Robot attempts a goal and return if reached
bool go_to_goal(move_base_msgs::MoveBaseGoal goal) {
    ros::Rate r(10);
    Client nav_client("move_base", true);
    nav_client.waitForServer();
    ros::spinOnce();
    ros::Time begin = ros::Time::now();
    nav_client.sendGoal(goal);

    while (ros::ok()) {
        // Goal cancels if not reached with 100 seconds
        if (ros::Time::now().sec - begin.sec > 100) {
            nav_client.cancelGoal();
            return false;
        }

        if (nav_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            return true;
    }
}

// simple goal for following
move_base_msgs::MoveBaseGoal move_forward_goal()
{
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "/base_link";
        
        goal.target_pose.pose.position.x =  0.1;
        goal.target_pose.pose.position.y =  0.0;
        goal.target_pose.pose.position.z =  0.0;
        goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(0);
        return goal;
}
