//
//  rangeScan.h
//  trilateration
//
//  Created by Randall Maas on 1/25/18.
//  Copyright © 2018 Randall Maas. All rights reserved.
//

#ifndef rangeScan_h
#define rangeScan_h
//
//  rangeScan.c
//  trilateration
//
//  Created by Randall Maas on 1/25/18.
//  Copyright © 2018 Randall Maas. All rights reserved.
//

#include <stdio.h>

/// The number of closest gotten from the rannge stream
extern int numRanges;

/// Initialize the scanning of ranges
void rangeScan_init();

/** Process the range sample
    @param range  The received range measurement
    
    This finds the ranges to 3 closest points.
    A point is a minima in the signal stream;
    To prevent multiple ranges for the same point we reset scanning for a minima after the distance goes
    back out by 10%
 */
extern void rangeScan_add(double range);


/** This is called when the end of the range streaem has been reached.
 */
extern void rangeScan_end();

#endif /* rangeScan_h */
