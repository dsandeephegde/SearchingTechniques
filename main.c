#include <stdio.h>
#include <sys/time.h>
#include <memory.h>
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

int binarySearchDisk(FILE* list, int searchTerm, long first, long last)
{
    long middle = (first + last) / 2;
    if(first <= last) {
        int middleElement;
        fseek( list, middle * sizeof( int ), SEEK_SET );
        fread(&middleElement, sizeof(int), 1, list);
        if(searchTerm == middleElement) {
            return 1;
        }
        else if(searchTerm < middleElement){
            return binarySearchDisk(list, searchTerm, first, middle - 1);
        }
        else {
            return binarySearchDisk(list, searchTerm, middle + 1, last);
        }
    }
    return 0;
}

long getNumberOfIntegers(char *fileName) {
    long length = 0, temp;
    FILE *file = fopen(fileName, "rb");
    while (fread( &temp, sizeof(int), 1, file)) {
        length++;
    }
    return length;
}

int main(int argc, char *argv[]) {
    struct timeval startTime, endTime, execTime;
    FILE *keyFile, *seekFile;
    int key, seek, i, j;

    if(argc != 4) {
        printf("Usage: assn_1 searchMode keyFileName seekFileName\n");
        exit(1);
    }
    char *searchMode, *keyFileName, *seekFileName;
    searchMode = argv[1];
    keyFileName = argv[2];
    seekFileName = argv[3];
    long keysLength = getNumberOfIntegers(keyFileName), seeksLength = getNumberOfIntegers(seekFileName);
    int keys[keysLength], seeks[seeksLength], hits[seeksLength];

    keyFile = fopen(keyFileName, "rb");
    seekFile = fopen(seekFileName, "rb");

    if(keyFile == NULL || seekFile == NULL) {
        printf("Please enter the correct file");
        exit(1);
    }

    for (i = 0; fread(&seek, sizeof(int), 1, seekFile); i++) {
        seeks[i] = seek;
        hits[i] = 0;
    }

    if (strcmp(searchMode, "--mem-lin") == 0) {
        gettimeofday(&startTime, NULL);
        for (i = 0; fread(&key, sizeof(int), 1, keyFile); i++) {
            keys[i] = key;
        }

        for (i = 0; i < seeksLength; i++) {
            for (j = 0; j < keysLength; j++) {
                if (keys[j] == seeks[i]) {
                    hits[i] = 1;
                }
            }
        }
        gettimeofday(&endTime, NULL);
    }
    else if (strcmp(searchMode, "--disk-lin") == 0) {
        gettimeofday(&startTime, NULL);
        for (i = 0; i < seeksLength; i++) {
            fseek( keyFile, 0, SEEK_SET );
            while (fread(&key, sizeof(int), 1, keyFile)) {
                if (key == seeks[i]) {
                    hits[i] = 1;
                }
            }
        }
        gettimeofday(&endTime, NULL);
    }
    else if (strcmp(searchMode, "--mem-bin") == 0) {
        gettimeofday(&startTime, NULL);
        for (i = 0; fread(&key, sizeof(int), 1, keyFile); i++) {
            keys[i] = key;
        }

        for (i = 0; i < seeksLength; i++) {
            hits[i] = binarySearch(keys, seeks[i], 0, keysLength - 1 );
        }
        gettimeofday(&endTime, NULL);
    }
    else if (strcmp(searchMode, "--disk-bin") == 0) {
        gettimeofday(&startTime, NULL);
        for (i = 0; i < seeksLength; i++) {
            fseek( keyFile, sizeof( int ), SEEK_SET );
            hits[i] = binarySearchDisk(keyFile, seeks[i], 0, keysLength - 1 );
        }
        gettimeofday(&endTime, NULL);
    }
    else {
        printf("Please enter the correct option\n");
        exit(1);
    }

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
