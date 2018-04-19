#include "movement.h"

// searches current point and all angles - false if object is not seen, else true
bool search_point() {
    // needs to publish when need to camera and processing to occur
    // must listen for when camera module responds
    return false;
}

// goes to next unvisited waypoint- false if finished traversing known path, else true
bool go_to_next() {
    return false;
}

bool go_to_position(move_base_msgs::MoveBaseGoal goal); // false if goal not reached