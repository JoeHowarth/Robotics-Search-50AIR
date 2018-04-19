#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "coordinates.h"
#include <move_base_msgs/MoveBaseAction.h>

bool search_point(); // searches current point and all angles - false if object is not seen, else true
bool go_to_next(); // goes to next unvisited waypoint- false if finished traversing known path, else true

bool go_to_position(move_base_msgs::MoveBaseGoal goal); // false if goal not reached

#endif // MOVEMENT_H_