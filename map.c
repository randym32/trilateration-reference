//
//  map.c
//  trilateration
//
//  Created by Randall Maas on 1/25/18.
//  Copyright © 2018 Randall Maas. All rights reserved.
//

#include <stdio.h>
#include "trilateration.h"
#include "3pt-position-estimator.h"
#include <math.h>
#include <stdlib.h>

#define FIRST_WALL (2)
// This is is a simple map of local points
static point_t  map[8] =
{
    {0,100},
    {23,800},
};

static int hwall_y[] = {0,1000, 300, 700};
static int vwall_x[]  = {0,1000};

static double dist2(point_t p1)
{
    return (p1.x-est_position.x)*(p1.x-est_position.x)+(p1.y-est_position.y)*(p1.y-est_position.y);
}
static int cmp(const void* p1,const void*p2)
{
    double d= dist2(map[*(int*)p1])-dist2(map[*(int*)p2]);
    return d < -0.001 ? -1 : d>0.001?1:0;
}


/** This looks up the three nearest points to us.
    Note that this is for demonstation purposes, and is a bit incomplete.
    Large objects, like walls,  have their landmark point computed crudely here.  Arbitrary walls and
    angles would use math just like in the geometry portion of video games.
 */
void lookUpNearbyLandmarks()
{
    // Make an array of the points
    int I;
    int pointIndex[8];
    
    for (I= 0; I < 8; I++)
    {
        pointIndex[I] = I;
    }
    
    // Compute synthetic wall points
    map[FIRST_WALL+0].x = vwall_x[0]; map[FIRST_WALL+0].y = est_position.y;
    map[FIRST_WALL+1].x = vwall_x[1]; map[FIRST_WALL+1].y = est_position.y;

    map[FIRST_WALL+2].x = est_position.x; map[FIRST_WALL+2].y = hwall_y[0];
    map[FIRST_WALL+3].x = est_position.x; map[FIRST_WALL+3].y = hwall_y[1];
    map[FIRST_WALL+4].x = est_position.x >100&&est_position.x <900?est_position.x:-1; map[FIRST_WALL+4].y = hwall_y[2];
    map[FIRST_WALL+5].x = est_position.x >100&&est_position.x <900?est_position.x:-1; map[FIRST_WALL+5].y = hwall_y[3];

    
    // Sort them as distance from our estimated position
    qsort (pointIndex, 8, sizeof(pointIndex[0]), cmp);

    // keep the best three
    a_point = map[pointIndex[0]];
    b_point = map[pointIndex[1]];
    c_point = map[pointIndex[2]];
}
