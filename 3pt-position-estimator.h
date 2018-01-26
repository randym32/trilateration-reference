//
//  lidar.h
//  trilateration
//
//  Created by Randall Maas on 1/25/18.
//  Copyright © 2018 Randall Maas. All rights reserved.
//

#ifndef lidar_h
#define lidar_h

#include "trilateration.h"

/// The estimated position of the observer
extern point_t est_position;

//   b) find three points closest to current estimated position
extern void lookUpNearbyLandmarks();

/** Estimates a position from landmarks and measured distances
    @returns estimated position
 */
extern point_t estimatePositionFromLandmarks();

#endif /* lidar_h */
