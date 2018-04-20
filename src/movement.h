#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "coordinates.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> Client;

// Searches in all 4 directions at current (x,y) position
// Returns true if object is seen, else returns false
bool search_point();

// Continues to the next unvisited waypoint
// Returns true while unvisited points exist, else returns false
bool go_to_next();

// Robot moves to a given position
// Returns true if position is reached, else returns false
bool go_to_position(move_base_msgs::MoveBaseGoal goal);

#endif // MOVEMENT_H_