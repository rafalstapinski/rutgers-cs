//
//  first.h
//  pa4
//
//  Created by Ryan D'souza on 4/18/17.
//  Copyright © 2017 Ryan D'souza. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "Queue.h"

/** Max number of bits for memory address */
#define MAX_MEMORY_ADDRESS_SIZE 48

/** A struct to represent the results of the Cache */
struct Results {
    int memoryReads;
    int memoryWrites;
    int cacheHits;
    int cacheMisses;
};

/** A struct to represent the integer values (in a cache) for the binary representation of a hex string */
struct SplitIndices {
    unsigned long indexValue;
    unsigned long blockSelectorValue;
    unsigned long tagValue;
};
