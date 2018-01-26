//
//  rangeScan.c
//  trilateration
//
//  Created by Randall Maas on 1/25/18.
//  Copyright © 2018 Randall Maas. All rights reserved.
//

#include "trilateration.h"
#include "rangeScan.h"

/// Scan distance stream to local points
static double minima = 1000*1000*1000;

/// The number of closest gotten from the rannge stream
int numRanges;



/// Initialize the scanning of ranges
void rangeScan_init()
{
    minima = 1000*1000*1000;
    numRanges = 0;
}


/** Place the minima into the list fo three closest range points
 */
static void rangeScan_emit()
{
    // Do we have three points?
    if (numRanges < 3)
    {
        ranges[numRanges++] = minima;
    }
    else
    {
        // See if the range should be kept or not
             if (minima < ranges[0]) ranges[0] = minima;
        else if (minima < ranges[1]) ranges[1] = minima;
        else if (minima < ranges[2]) ranges[2] = minima;
    }
}

/** Process the range sample
    @param range  The received range measurement
    
    This finds the ranges to 3 closest points.
    A point is a minima in the signal stream;
    To prevent multiple ranges for the same point we reset scanning for a minima after the distance goes
    back out by 10%
 */
void rangeScan_add(double range)
{
    // Has it gone back 10%?
    if (range >= minima*1.1)
    {
        // Emit the range
        rangeScan_emit();

        // Start scanning for the next minima
        minima = 1000*1000*1000;
        return;
    }
    if (range < minima && range > 0.1)
    {
        // This range is close, so track its value
        minima = range;
    }
}

/** This is called when the end of the range streaem has been reached.
 */
void rangeScan_end()
{
    rangeScan_emit();
}

