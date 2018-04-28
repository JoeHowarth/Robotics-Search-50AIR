#include "ros/ros.h"
#include "movement.h"


#include <stdlib.h>
#include <time.h>
bool image_search() {
    // srand(time(NULL));
    // if (rand() % 2)
    //     return true;
    return false;
}

// Searches in all directions at current (x,y) position
// Returns true if object is seen, else returns false
bool search_point() {
    if (search360() == true) {
        return investigate_further();
    }
    return false;
}


// Searches all directions at current (x,y) poisition
// Stops at angle if object is seen and returns true
// Else, returns false
bool search360() {
    float degree = 0.0;
    while (degree <= 420) {
        rotate(45.0);
        degree += 45.0;
        if (image_search()) {
            return true;
        }
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
    if (go_to_position(goal)) {
        ROS_INFO("Succesfully rotated to %f degrees", degree);
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
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(next_location.yaw);

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

// Robot has seen something of interest in the current direction and will
// investgate more
// Returns true if object is seen, returns false otherwise
bool investigate_further() {
    std::stack<Point> searchedPoints;
    if (image_search()) {
        searchedPoints.push(curr_position);
        go_to_position(move_forward_goal());
    }
    while (!searchedPoints.empty()) {
        if (image_search()) {
            searchedPoints.push(curr_position);
            go_to_position(move_forward_goal());
        } else {
            if (search360()) {
                searchedPoints.push(curr_position);
                go_to_position(move_forward_goal());
            } else {
                Point return_point = searchedPoints.top();
                go_to_next(return_point);
                searchedPoints.pop();
            }

        }
    }
    return false;
}

move_base_msgs::MoveBaseGoal move_forward_goal()
{
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "/base_link";
        
        goal.target_pose.pose.position.x =  0.5;
        goal.target_pose.pose.position.y =  0.0;
        goal.target_pose.pose.position.z =  0.0;
        goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(0);
        return goal;
}
