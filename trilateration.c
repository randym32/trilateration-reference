//
//  trilateration.c
//  trilateration
//
//  Created by Randall Maas on 1/25/18.
//  Copyright © 2018 Randall Maas. All rights reserved.
//

#include "trilateration.h"


/// The points on the field for the 3 nearest landmarks
point_t a_point,b_point,c_point;

/// The the /measured/ distances to those landmarks
double ranges[3];

/** This computes the point of the observer, given 
    - the distances (a_distance, b_distance, c_distance) to 3 known points (a_point, b_point, c_point).
 
    @returns The estimated point.
 
    Note: on error, x or y may -1.
 
    This doesn't work well if the area of the triangle is small, the point is not contained within
    the triangle, or if the triangle has the wrong winding -- that is, the points in the wrong order.
 
    Resources
     the first code is modified from https://gist.github.com/kdzwinel/8235348
 */
point_t trilaterate(void)
{
    double t1 = -b_point.x*b_point.x - b_point.y*b_point.y + ranges[1]*ranges[1];
    double S = (c_point.x*c_point.x + t1 + c_point.y*c_point.y - ranges[2]*ranges[2]) / 2.0;
    double T = (a_point.x*a_point.x + t1 + a_point.y*a_point.y - ranges[0]*ranges[0]) / 2.0;
    double denom = (((a_point.y - b_point.y) * (b_point.x - c_point.x)) - ((c_point.y - b_point.y) * (b_point.x - a_point.x)));
    double y = denom < -0.001 || denom > 0.001 ? ((T * (b_point.x - c_point.x)) - (S * (b_point.x - a_point.x))) / denom : -1;
    denom = (b_point.x - a_point.x);
    double x = denom < -0.001 || denom > 0.001 ? ((y * (a_point.y - b_point.y)) - T) / denom : -1;
    point_t r = {x,y};
    return r;
}
