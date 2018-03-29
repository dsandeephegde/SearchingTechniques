#include <stdio.h>
#include <sys/time.h>
#include <memory.h>

int main(int argc, char *argv[])
{
    struct timeval startTime, endTime;
    FILE *keyFile, *seekFile;
    int key, seek;
    int keys[10000], seeks[10000], hits[10000]; //dynamic allocation
    int keysLength = 0, seeksLength = 0;
    int i, j;

    keyFile = fopen( "key.db", "rb" ); //from cmd arg
    seekFile = fopen( "seek.db", "rb" ); //from cmd arg

    for ( i = 0; fread( &seek, sizeof( int ), 1, seekFile ); i++)
    {
        seeks[i] = seek;
        hits[i] = 0;
        seeksLength++;
    }

    if(strcmp(argv[1], "--mem-lin") == 0) {
        gettimeofday( &startTime, NULL);
        for ( i = 0; fread( &key, sizeof( int ), 1, keyFile ); i++)
        {
            keys[i] = key;
            keysLength++;
        }

        for ( i = 0; i < seeksLength; i++)
        {
            for (j = 0; j < keysLength; j++) {
                if(keys[j] == seeks[i])
                {
                    hits[i] = 1;
                }
            }
        }
        gettimeofday( &endTime, NULL);

        for (i = 0; i < seeksLength; ++i) {
            printf( "%12d: ", seeks[i]);
            if(hits[i] == 1)
            {
                printf( "Yes\n");
            } else
            {
                printf( "No\n");
            }
        }
        printf("Time: %lf\n", (double) (endTime.tv_usec - startTime.tv_usec) / 1000000);
    }

    else if(strcmp(argv[1], "--disk-lin") == 0) {
        gettimeofday(&startTime, NULL);
        for (i = 0; i < seeksLength; i++) {
            while (fread(&key, sizeof(int), 1, keyFile)) {
                if (key == seeks[i]) {
                    hits[i] = 1;
                }
            }
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
        printf("Time: %lf\n", (double) (endTime.tv_usec - startTime.tv_usec) / 1000000);
    }
    return 0;
}
