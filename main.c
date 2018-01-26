//
//  main.c
//  trilateration
//
//  Created by Randall Maas on 1/24/18.
//  Copyright © 2018 Randall Maas. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "trilateration.h"
#include "3pt-position-estimator.h"
#include "rangeScan.h"
#include <math.h>

// get three closest independent points

static double dist(point_t p1)
{
        double r = (p1.x-est_position.x)*(p1.x-est_position.x)+(p1.y-est_position.y)*(p1.y-est_position.y);
        return sqrt(r);
}


/// Tests the trilaterate procedure
int test1()
{
    srand(123);
    int I;
    double err=0;
    a_point=(point_t){3,7};b_point=(point_t){1000,767};c_point=(point_t){800, 37};
    for (I=0; I < 1000; I++)
    {
        est_position.x= 1000*((double)rand())/RAND_MAX;
        est_position.y= 1000*((double)rand())/RAND_MAX;
        ranges[0]=dist(a_point); ranges[1]= dist(b_point); ranges[2]= dist(c_point);
        point_t est = trilaterate();
        err+=dist(est);
    }
    return err < 1;
}

/// Tests the locate from 3 range measurements and table of points
int test2()
{
    double err=0;
    est_position= (point_t) {7,8};
    lookUpNearbyLandmarks();
    ranges[0]=  dist(a_point);
    ranges[1]=  dist(b_point);
    ranges[2]=  dist(c_point);
    point_t est = estimatePositionFromLandmarks();
    
    // Sanity check the distances
    err = ranges[0]-  dist(a_point);
    err += ranges[1]- dist(b_point);
    err += ranges[2]-  dist(c_point);

    
    err+=dist(est);
    return err < 1;
}

/// Tests the locate from 3 range measurements and table of points
int test3()
{
    int I;
    double err=0;
    srand(123);
    for (I=0; I < 1000; I++)
    {
    est_position= (point_t) {2+(996.0*rand())/RAND_MAX,2+(996.0*rand())/RAND_MAX};
    lookUpNearbyLandmarks();
        ranges[0]=  dist(a_point)+(-1+(2*rand())/RAND_MAX);
        ranges[1]=  dist(b_point)+(-1+(2*rand())/RAND_MAX);
        ranges[2]=  dist(c_point)+(-1+(2*rand())/RAND_MAX);
    point_t est = estimatePositionFromLandmarks();
    err+=dist(est);
    }
    return err < 2000;
}

/// Tests the scanning of the stream of ranges
int test4()
{
    rangeScan_init();
    est_position= (point_t) {7,8};
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    if (numRanges != 0) return 0;
    double tmp =dist(a_point);
    rangeScan_add(tmp);
    rangeScan_add(tmp);
    rangeScan_add(tmp+1);
    rangeScan_add(tmp+1);
    rangeScan_add(tmp+1);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    if (numRanges != 1) return 0;
    tmp =dist(b_point);
    rangeScan_add(tmp);
    rangeScan_add(tmp);
    rangeScan_add(tmp+1);
    rangeScan_add(tmp+1);
    rangeScan_add(tmp+1);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    if (numRanges != 2) return 0;
    tmp =dist(c_point);
    rangeScan_add(tmp);
    rangeScan_add(tmp);
    rangeScan_add(tmp+1);
    rangeScan_add(tmp+1);
    rangeScan_add(tmp+1);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    rangeScan_add(10000);
    if (numRanges != 3) return 0;

    rangeScan_end();
    point_t est = estimatePositionFromLandmarks();
    double err=dist(est);
    return err < 1;
}

int main(int argc, const char * argv[])
{
    if (!test1())
        fprintf(stderr, "test 1: failed");
    if (!test2())
        fprintf(stderr, "test 2: failed");
    if (!test3())
        fprintf(stderr, "test 3: failed");
    if (!test4())
        fprintf(stderr, "test 4: failed");
    return 0;
}
