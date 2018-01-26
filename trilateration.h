//
//  trilaterationh.h
//  trilateration
//
//  Created by Randall Maas on 1/25/18.
//  Copyright © 2018 Randall Maas. All rights reserved.
//

#ifndef trilateration_h
#define trilateration_h

/// A structure to represent points on the field
typedef struct {double x, y;} point_t;

/// The 3 ranges representing the /measured/ distances to landmarks
extern double ranges[3];

/// The points on the field for the 3 nearest landmarks
extern point_t a_point,b_point,c_point;

/** This computes the point of the observer, given 
    - the distances (a_distance, b_distance, c_distance) to 3 known points (a_point, b_point, c_point).
 
    @returns The estimated point.
 
    Note: on error, x or y may -1.
 
    This doesn't work well if the area of the triangle is small, the point is not contained within
    the triangle, or if the triangle has the wrong winding -- that is, the points in the wrong order.
 
    Resources
     the first code is modified from https://gist.github.com/kdzwinel/8235348
 */
extern point_t trilaterate(void);


#endif /* trilaterationh_h */
