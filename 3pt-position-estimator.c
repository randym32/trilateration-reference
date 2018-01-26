/*
    This takes the 3 closest positions & distances and estimates the observers position
    Created by Randall Maas on 1/25/18.
    Copyright © 2018 Randall Maas. All rights reserved.

 
         +---------+
         |Estimated+-----+
         |Position |     |
         +---------+     |
                         |  3 closest
         +---------+     v  landmarks
         | Landmark+--->(x)+---------------+
         | Table   |                       |
         +---------+                       |
                                           |              +----------+
         +---------+                  +----v--------+     |Trilateral|
         | Ranging | Three distances  |Trilateration+----->Estimated |
         |         +------------------>             |     |Position  |
         +---------+                  +-------------+     +----------+
 
*/

#include "trilateration.h"
#include "3pt-position-estimator.h"
#include <math.h>

int errCnt = 0;

/// The estimated position of the observer
point_t est_position;
/// The maximum deviation of the trilaterated point from the estimated position before it is rejected.
double maxTrilaterateError = 144;

/** Sort the 3 best nearest measurements.
    This makes it easy to match them up to the 3 landmarkers.
*/
static void sort_distances()
{
    if (ranges[2] < ranges[0])
    {
        double tmp = ranges[0];
        ranges[0]  = ranges[2];
        ranges[2]  = tmp;
    }
    if (ranges[1] < ranges[0])
    {
        double tmp = ranges[0];
        ranges[0]  = ranges[1];
        ranges[1]  = tmp;
    }
    if (ranges[2] < ranges[1])
    {
        double tmp = ranges[2];
        ranges[2]  = ranges[1];
        ranges[1]  = tmp;
    }
}

/** A helper to compute the distance (squared) between the observer and landmark point
    @param p2  Landmark point
    @returns the distance, squared.
 
    This is used to sort the points.
 */
static inline double dist2(point_t p2)
{
    return (est_position.x-p2.x)*(est_position.x-p2.x)+(est_position.y-p2.y)*(est_position.y-p2.y);
}


/** Sort the 3 landmarks, nearest to farthest way.
    This makes it easy to match them up to the 3 distances.  The 3 landmarks were selected in an earlier
    state.
 */
static void sort_points()
{
    if (dist2(c_point) < dist2(a_point))
    {
        point_t tmp = a_point;
        a_point = c_point;
        c_point = tmp;
    }
    if (dist2(b_point) < dist2(a_point))
    {
        point_t tmp = a_point;
        a_point = b_point;
        b_point = tmp;
    }
    if (dist2(c_point) < dist2(b_point))
    {
        point_t tmp = c_point;
        c_point = b_point;
        b_point = tmp;
    }
}


/** Estimates a position from landmarks and measured distances
    @returns estimated position
 
    Note: there are times that other approached can produce better results in various special cases.
      For instance, if we know that a wall is on an x axis, (or y-axis) we can improve our
      x position just from that.  That distance measure is not good for trilateration.

 */
point_t estimatePositionFromLandmarks()
{
    // 1) match up the measured distances to the points
    //   a) sort the measured distances
    sort_distances();

    //   b) find three points closest to current estimated position
    lookUpNearbyLandmarks();
    
    //   c) sort the points
    // Note: this is not done here as the points are sorted for us by lookUpNearbyLandmarks()
    //sort_points();
    
    // The points may be in the wrong order at this moment -- that is, they might be wound
    // counter-clockwise rather than clockwise.  We could look for winding and reorder, but
    // it is easier to call trilaterate() twice.

    // compute a position estimate using trilateration
    point_t est_trilaterate = trilaterate();

    // check result - is the trilateral estimate valid?  If not, try swaping the values around
    // to get the right winding, and recalculate
    if (est_trilaterate.x < 0 || est_trilaterate.y < 0)
    {
        // Swap the points
        double tmpd = ranges[1];
        ranges[1]   = ranges[2];
        ranges[2]   = tmpd;
        point_t tmpp = b_point;
        b_point     = c_point;
        c_point     = tmpp;
        
        // Recalculate the estimate
        est_trilaterate = trilaterate();
    }

    // check result - is the trilateral estimate valid and close-enough to our estimated position?
    // no: throw it out, and set it to our estimated position
    if (est_trilaterate.x < 0 || est_trilaterate.y < 0 || dist2(est_trilaterate)> maxTrilaterateError)
    {
        errCnt++;
        return est_position;
    }
    
    // the estimated position looks good, return it
    return est_trilaterate;
}




