#ifndef COORDINATES_H_
#define COORDINATES_H_

#include <vector>
#include <string>

struct Point {
        float x, y;        
        std::string description;
};

const double PI = 3.141592653589793238463;

// Predefined waypoints for camera to search at
const std::vector<Point> camera_position = {
        {-4.873, 1.701, "Top of Staircase"},
        {-1.003, 2.135, "Front of Couches"},
        {-0.448, 6.048, "Hallway Intersection"},
        {-0.383, 8.614, "Front of Table"},
        {-0.728, 13.86, "Elevator Intersection"},
        {-7.463, 13.857, "End of Hallway"},
        {4.998, 13.649, "Kitchen-EE Lab Hallway"},
        {5.394, 6.621, "B. Tracey Hallway"}
};

const double MIN_SEARCH_PROBABILITY = 0.4;
const double MIN_FOUND_PROBABILITY = 0.8;

#endif // COORDINATES_H_
