//
//  first.c
//  pa4
//
//  Created by Ryan D'souza on 4/18/17.
//  Copyright © 2017 Ryan D'souza. All rights reserved.
//

#include "first.h"

struct Queue* createQueue(int maxSize)
{
    struct Queue *queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    queue->MAX_SIZE = maxSize;
    queue->enabled = '1';

    return queue;
}

/** Returns 1 if the Queue contains the value. Returns -1 if the Queue does not contain the value */
int queueContainsValue(struct Queue *queue, unsigned long value)
{
    if(queue == NULL || queue->enabled != '1') {
        return -1;
    }

    struct Node *iterator = queue->front;

    while(iterator != NULL) {
        if(iterator->enabled == '1' && value == iterator->value) {
            return 1;
        }
        iterator = iterator->next;
    }

    return -1;
}

/** Dequeues the first item in the Queue */
void dequeueValue(struct Queue *queue)
{
    if(queue->front == NULL || queue->front->enabled != '1') {
        return;
    }

    struct Node *oldFront = queue->front;

    queue->front = queue->front->next;

    free(oldFront);

    queue->size--;
}

/** Enqueues the value. If the Queue is greater than the MAX_SIZE, automatically dequeues */
void enqueueValue(struct Queue *queue, unsigned long value)
{
    struct Node *node = (struct Node*) malloc(sizeof(struct Node));
    node->value = value;
    node->next = NULL;
    node->enabled = '1';

    if(queue->front == NULL || queue->front->enabled != '1') {
        queue->front = node;
        queue->rear = queue->front;
    }

    else {
        queue->rear->next = node;
        queue->rear = queue->rear->next;
    }

    queue->size++;

    if(queue->size > queue->MAX_SIZE) {
        dequeueValue(queue);
    }
}

/** Allocates a Results struct, initializes values */
struct Results *createResults()
{
    struct Results *cacheResults = malloc(sizeof(struct Results));
    cacheResults->cacheHits = 0;
    cacheResults->cacheMisses = 0;
    cacheResults->memoryReads = 0;
    cacheResults->memoryWrites = 0;

    return cacheResults;
}

/** Prints the results in the correct format */
void printResults(struct Results *results)
{
    printf("Memory reads: %d\n", results->memoryReads);
    printf("Memory writes: %d\n", results->memoryWrites);
    printf("Cache hits: %d\n", results->cacheHits - 1);
    printf("Cache misses: %d\n", results->cacheMisses);
}

/** Returns the binary values for a Hex character */
char* getBinaryArrayForHexCharacter(char hexCharacter)
{
    if(isalpha(hexCharacter)) {
        hexCharacter = toupper(hexCharacter);
    }

    switch(hexCharacter) {
        case '0':
            return "0000";
        case '1':
            return "0001";
        case '2':
            return "0010";
        case '3':
            return "0011";
        case '4':
            return "0100";
        case '5':
            return "0101";
        case '6':
            return "0110";
        case '7':
            return "0111";
        case '8':
            return "1000";
        case '9':
            return "1001";
        case 'A':
            return "1010";
        case 'B':
            return "1011";
        case 'C':
            return "1100";
        case 'D':
            return "1101";
        case 'E':
            return "1110";
        case 'F':
            return "1111";
        default:
            return "0000";
    }
}

/** Returns a binary array (as an array of characters) for the hex string */
char* getBinaryArrayForHexString(char *hexString)
{
    int counter = 0;

    //Size of the hex string in binary
    int binarySize = (((int) strlen(hexString)) * sizeof(char) * 4 + 1);
    char *binaryResult = malloc(binarySize);

    //Initialize all values to the null character
    memset(binaryResult, '\0', binarySize);

    //Go through every hex character, store the results into our binary character array
    while(hexString[counter]) {
        char *hexResult = getBinaryArrayForHexCharacter(hexString[counter]);
        strcat(binaryResult, hexResult);

        counter++;
    }

    //Memory address must be 48 bits --> left pad if we have fewer than 48 bits
    int resultSize = (MAX_MEMORY_ADDRESS_SIZE * sizeof(char) + 1);
    char *charResult = malloc(resultSize);
    memset(charResult, '\0', resultSize);

    //Left-padding --> everything to the left of the last digit = 0
    int offset = resultSize - binarySize;
    memset(charResult, '0', offset);
    strcat(charResult, binaryResult);

    free(binaryResult);

    return charResult;
}

/** Returns a long (numerical value) given a binary array and start and end indices */
unsigned long getNumericalValueFromBinaryArray(char *binary, int start, int end)
{
    int i = 0;
    unsigned long result = 0;

    for(i = end; i >= start; i--) {
        int binaryValue = binary[i] - '0';
        result += (pow(2, (end - i)) * binaryValue);
    }

    return result;
}

/** Returns a SplitIndices struct with the cache values given the parameters for Type A cache indexing scheme */
struct SplitIndices getTypeASplitIndices(char *hexString, int cacheSizeBytes, int association, int blockSizeBytes)
{
    char *binaryArray = getBinaryArrayForHexString(hexString);

    int blockSizeBits = (int) log2(blockSizeBytes);
    int numberOfBlocks = cacheSizeBytes / blockSizeBytes;

    struct SplitIndices splitIndices;

    // FULLY ASSOCIATIVE CACHE
    if(association == 0) {

        int endBlockSelectorIndex = ((int) strlen(binaryArray)) - 1;
        int startBlockSelectorIndex = endBlockSelectorIndex - blockSizeBits + 1;
        splitIndices.blockSelectorValue = getNumericalValueFromBinaryArray(binaryArray, startBlockSelectorIndex, endBlockSelectorIndex);

        int endTagSelectorIndex = startBlockSelectorIndex - 1;
        int startTagSelectorIndex = 0;
        splitIndices.tagValue = getNumericalValueFromBinaryArray(binaryArray, startTagSelectorIndex, endTagSelectorIndex);

        splitIndices.indexValue = 0;
    }

    // DIRECT AND SET ASSOCIATIVE CACHE
    else {

        int indexSizeBits = (int) log2(numberOfBlocks / association);

        int endBlockSelectorIndex = ((int) strlen(binaryArray)) - 1;
        int startBlockSelectorIndex = endBlockSelectorIndex - blockSizeBits + 1;
        splitIndices.blockSelectorValue = getNumericalValueFromBinaryArray(binaryArray, startBlockSelectorIndex, endBlockSelectorIndex);

        int endIndexSelectorIndex = startBlockSelectorIndex - 1;
        int startIndexSelectorIndex = endIndexSelectorIndex - indexSizeBits + 1;
        splitIndices.indexValue = getNumericalValueFromBinaryArray(binaryArray, startIndexSelectorIndex, endIndexSelectorIndex);

        int endTagSelectorIndex = startIndexSelectorIndex;
        int startTagSelectorIndex = 0;
        splitIndices.tagValue = getNumericalValueFromBinaryArray(binaryArray, startTagSelectorIndex, endTagSelectorIndex);
    }

    free(binaryArray);

    return splitIndices;
}

/** Returns a SplitIndices struct with the cache values given the parameters for Type A cache indexing scheme */
struct SplitIndices getTypeBSplitIndices(char *hexString, int cacheSizeBytes, int association, int blockSizeBytes)
{
    char *binaryArray = getBinaryArrayForHexString(hexString);

    int blockSizeBits = (int) log2(blockSizeBytes);
    int numberOfBlocks = cacheSizeBytes / blockSizeBytes;

    struct SplitIndices splitIndices;

    // FULLY ASSOCIATIVE CACHE
    if(association == 0) {

        int endBlockSelectorIndex = ((int) strlen(binaryArray)) - 1;
        int startBlockSelectorIndex = endBlockSelectorIndex - blockSizeBits + 1;
        splitIndices.blockSelectorValue = getNumericalValueFromBinaryArray(binaryArray, startBlockSelectorIndex, endBlockSelectorIndex);

        int endTagSelectorIndex = startBlockSelectorIndex - 1;
        int startTagSelectorIndex = 0;
        splitIndices.tagValue = getNumericalValueFromBinaryArray(binaryArray, startTagSelectorIndex, endTagSelectorIndex);

        splitIndices.indexValue = 0;
    }

    // DIRECT AND SET ASSOCIATIVE CACHE
    else {

        int indexSizeBits = (int) log2(numberOfBlocks / association);
        int tagSizeBits = ((int) strlen(binaryArray)) - (indexSizeBits + blockSizeBits);

        int endBlockSelectorIndex = ((int) strlen(binaryArray)) - 1;
        int startBlockSelectorIndex = endBlockSelectorIndex - blockSizeBits + 1;
        splitIndices.blockSelectorValue = getNumericalValueFromBinaryArray(binaryArray, startBlockSelectorIndex, endBlockSelectorIndex);

        int endTagSelectorIndex = startBlockSelectorIndex - 1;
        int startTagSelectorIndex = endTagSelectorIndex - tagSizeBits + 1;
        splitIndices.tagValue = getNumericalValueFromBinaryArray(binaryArray, startTagSelectorIndex, endTagSelectorIndex);

        int endIndexSelectorIndex = startTagSelectorIndex;
        int startIndexSelectorIndex = 0;
        splitIndices.indexValue = getNumericalValueFromBinaryArray(binaryArray, startIndexSelectorIndex, endIndexSelectorIndex);
    }

    free(binaryArray);

    return splitIndices;
}

/** Main method - entry point */
int main(int argc, char * argv[])
{
    if(argc < 5) {
        printf("Not enough parameters\n");
        exit(0);
    }

    //Input parameters
    const int cacheSize = atoi(argv[1]);
    char *cacheType = argv[2];
    const int blockSize = atoi(argv[3]);
    const char *traceFileName = argv[4];

    int association = 0;

    if(strcmp(cacheType, "direct") == 0) {
        association = 1;
    } else if(strcmp(cacheType, "assoc") == 0) {
        association = cacheSize / blockSize;
    } else {
        memset(cacheType, '0', strlen("assoc:"));
        association = atoi(cacheType);
    }

    int numberOfSets = 1;

    if(association == 0) {
        numberOfSets = cacheSize / blockSize;
    } else {
        numberOfSets = cacheSize / (association * blockSize);
    }

    struct Queue *cacheA = (struct Queue*) malloc(numberOfSets * sizeof(struct Queue));
    struct Queue *cacheB = (struct Queue*) malloc(numberOfSets * sizeof(struct Queue));

    //Initialize our Queues
    int i = 0;
    for(i = 0; i < numberOfSets; i++) {
        cacheA[i] = *createQueue(association);
        cacheB[i] = *createQueue(association);
    }

    //Represent our results
    struct Results *cacheAResults = createResults();
    struct Results *cacheBResults = createResults();

    FILE *traceFile = fopen(traceFileName, "r");

    if(traceFile == NULL) {
        printf("Error reading trace file\n");
        printf("cache A\n");
        printResults(cacheAResults);
        printf("cache B\n");
        printResults(cacheBResults);
        exit(0);
    }

    //For reading in the input
    char *programCounter = (char *) malloc(50 * sizeof(char));
    char commandType;
    char *memoryAddress = (char *) malloc(50 * sizeof(char));

    while(fscanf(traceFile, "%s %c %s", programCounter, &commandType, memoryAddress) != EOF) {

        //Point past "0x" to get the actual hex value --> "0xbf8ef49c" to "bf8ef49c"
        memoryAddress += 2;

        struct SplitIndices typeAIndices = getTypeASplitIndices(memoryAddress, cacheSize, association, blockSize);
        struct SplitIndices typeBIndices = getTypeBSplitIndices(memoryAddress, cacheSize, association, blockSize);

        //Point back to the beginning
        memoryAddress -= 2;

        struct Queue *relevantQueueCacheA = &cacheA[typeAIndices.indexValue];
        struct Queue *relevantQueueCacheB = &cacheB[typeBIndices.indexValue];

        const int containsCacheA = queueContainsValue(relevantQueueCacheA, typeAIndices.tagValue);
        const int containsCacheB = queueContainsValue(relevantQueueCacheB, typeBIndices.tagValue);

        //READ COMMAND
        if(commandType == 'R') {

            //NOT IN CACHE
            if(containsCacheA == -1) {
                cacheAResults->cacheMisses += 1;
                cacheAResults->memoryReads += 1;
                enqueueValue(relevantQueueCacheA, typeAIndices.tagValue);
            }

            //IN CACHE
            else if(containsCacheA == 1) {
                cacheAResults->cacheHits += 1;
            }

            //NOT IN CACHE
            if(containsCacheB == -1) {
                cacheBResults->cacheMisses += 1;
                cacheBResults->memoryReads += 1;
                enqueueValue(relevantQueueCacheB, typeBIndices.tagValue);
            }

            //IN CACHE
            else if(containsCacheB == 1) {
                cacheBResults->cacheHits += 1;
            }
        }

        else if(commandType == 'W') {

            //NOT IN CACHE
            if(containsCacheA == -1) {
                cacheAResults->cacheMisses++;
                cacheAResults->memoryReads++;
                cacheAResults->memoryWrites++;
                enqueueValue(relevantQueueCacheA, typeAIndices.tagValue);
            }

            //IN CACHE
            else if(containsCacheA == 1) {
                cacheAResults->cacheHits++;
                cacheAResults->memoryWrites++;
            }

            //NOT IN CACHE
            if(containsCacheB == -1) {
                cacheBResults->cacheMisses++;
                cacheBResults->memoryReads++;
                cacheBResults->memoryWrites++;
                enqueueValue(relevantQueueCacheB, typeBIndices.tagValue);
            }

            //IN CACHE
            else if(containsCacheB == 1) {
                cacheBResults->cacheHits++;
                cacheBResults->memoryWrites++;
            }
        }
    }

    fclose(traceFile);

    printf("cache A\n");

    if(strcmp(cacheType, "assoc") == 0) {
        printResults(cacheBResults);
    }

    else {
        printResults(cacheAResults);
    }

    printf("cache B\n");
    printResults(cacheBResults);

    return 0;
}
