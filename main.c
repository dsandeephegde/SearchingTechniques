#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

int binarySearch(int list[], int searchTerm, long first, long last)
{
    long middle = (first + last) / 2;
    if(first <= last) {
        if(searchTerm == list[middle]) {
            return 1;
        }
        else if(searchTerm < list[middle]){
            return binarySearch(list, searchTerm, first, middle - 1);
        }
        else {
            return binarySearch(list, searchTerm, middle + 1, last);
        }
    }
    return 0;
}

int binarySearchOnDisk(FILE *file, int searchTerm, long first, long last)
{
    long middle = (first + last) / 2;
    if(first <= last) {
        int middleElement;
        fseek( file, middle * sizeof( int ), SEEK_SET );
        fread(&middleElement, sizeof(int), 1, file);
        if(searchTerm == middleElement) {
            return 1;
        }
        else if(searchTerm < middleElement){
            return binarySearchOnDisk(file, searchTerm, first, middle - 1);
        }
        else {
            return binarySearchOnDisk(file, searchTerm, middle + 1, last);
        }
    }
    return 0;
}

long getNumberOfIntegers(FILE *file) {
    fseek( file, 0, SEEK_END );
    int length = ftell(file) / sizeof(int);
    fseek( file, 0, SEEK_SET );
    return length;
}

int main(int argc, char *argv[]) {
    struct timeval startTime, endTime, execTime;
    FILE *keyFile, *seekFile;
    int key, i, j;

    if(argc != 4) {
        printf("Usage: assn_1 searchMode keyFileName seekFileName\n");
        exit(1);
    }
    char *searchMode, *keyFileName, *seekFileName;
    searchMode = argv[1];
    keyFileName = argv[2];
    seekFileName = argv[3];

    keyFile = fopen(keyFileName, "rb");
    seekFile = fopen(seekFileName, "rb");

    long keysLength = getNumberOfIntegers(keyFile), seeksLength = getNumberOfIntegers(seekFile);
    int keys[keysLength], seeks[seeksLength], hits[seeksLength];

    if(keyFile == NULL || seekFile == NULL) {
        printf("Please enter the correct file");
        exit(1);
    }

    fread(seeks, sizeof(int), (size_t) seeksLength, seekFile);
    for (i = 0; i < seeksLength; ++i) {
        hits[i] = 0;
    }

    gettimeofday(&startTime, NULL);
    if (strcmp(searchMode, "--mem-lin") == 0) {
        fread(keys, sizeof(int), (size_t) keysLength, keyFile);
        for (i = 0; i < seeksLength; i++) {
            for (j = 0; j < keysLength; j++) {
                if (keys[j] == seeks[i]) {
                    hits[i] = 1;
                }
            }
        }
    }
    else if (strcmp(searchMode, "--disk-lin") == 0) {
        for (i = 0; i < seeksLength; i++) {
            fseek( keyFile, 0, SEEK_SET );
            while (fread(&key, sizeof(int), 1, keyFile)) {
                if (key == seeks[i]) {
                    hits[i] = 1;
                }
            }
        }
    }
    else if (strcmp(searchMode, "--mem-bin") == 0) {
        fread(keys, sizeof(int), (size_t) keysLength, keyFile);

        for (i = 0; i < seeksLength; i++) {
            hits[i] = binarySearch(keys, seeks[i], 0, keysLength - 1 );
        }
    }
    else if (strcmp(searchMode, "--disk-bin") == 0) {
        for (i = 0; i < seeksLength; i++) {
            hits[i] = binarySearchOnDisk(keyFile, seeks[i], 0, keysLength - 1);
        }
    }
    else {
        printf("Please enter the correct option\n");
        exit(1);
    }
    gettimeofday(&endTime, NULL);

    for (i = 0; i < seeksLength; ++i) {
        printf("%12d: ", seeks[i]);
        if (hits[i] == 1) {
            printf("Yes\n");
        } else {
            printf("No\n");
        }
    }
    timersub(&endTime, &startTime, &execTime);
    printf("Time: %ld.%06ld\n", execTime.tv_sec, (long) execTime.tv_usec);
    return 0;
}
