#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "coordinates.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/tf.h>
#include <math.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> Client;

// Searches in all 4 directions at current (x,y) position
// Returns true if object is seen, else returns false
bool search_point();

// Rotates the robot to the given direction
void rotate(float degree);

// Goes to the given waypoint
// Returns true if reaches point, else returns false
bool go_to_next(Point next_location);

// Robot moves to a given position
// Returns true if position is reached, else returns false
bool go_to_position(move_base_msgs::MoveBaseGoal goal);

#endif // MOVEMENT_H_