#include "preprocess.h"
#include <algorithm>
#include <cmath>
double distance(Point p1, Point p2)
{
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        return std::sqrt(dx*dx + dy*dy);
}

double perm_distance(std::vector<Point> &perm)
{
        double dist = 0;
        for (unsigned i = 0; i < perm.size()-1; ++i)
                dist += distance(perm[i], perm[i+1]);
        return dist;
}

//camera_position
std::vector<Point> make_order(Point start_pos)
{
        
        std::vector<Point> perm = camera_position;
        perm.insert(perm.begin(), start_pos);

        double best_dist = std::numeric_limits<double>::max();
        std::vector<Point>  best_perm = perm;

        auto comp = [](Point a, Point p) {
                return std::sqrt(a.x*a.x+a.y*a.y) <
                std::sqrt(p.x*p.x+p.y*p.y);
        };
        while (std::next_permutation(perm.begin()+1, perm.end(), comp)) {
                double dist = perm_distance(perm);
                if (dist < best_dist) {
                        best_dist = dist;
                        best_perm = perm;
                }
        }
        return best_perm;
}

/*#include <iostream>
int main()
{
        for (auto i : make_order(camera_position.back()))
                std::cout << i.x << ", " << i.y << i.description << std::endl;
                }*/
