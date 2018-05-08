#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "coordinates.h"

#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/tf.h>
#include <math.h>
#include <stack>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> Client;
extern Point curr_position;
extern float confidence;

float check_image();

// Searches in all 4 directions at current (x,y) position
// Returns true if object is seen, else returns false
bool search();

// Rotates the robot to the given direction
void rotate(float degree);

// Attempt to go to given point; return if achived
bool go_to_point(Point next_location);

// Attempt to do a move goal; return if achived
bool go_to_goal(move_base_msgs::MoveBaseGoal goal);

// Dummy goal to allow object following
move_base_msgs::MoveBaseGoal move_forward_goal();


#endif // MOVEMENT_H_
