
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//  #define TEST

/*
    Sets a double array autocorr_forDistance whose size is max_distance(which is usually nTimesteps - 2) 
    and 'distance-1'th element value is autocorrelation(distance).
*/
void set_autocorrelations(double valueSequence[], int nTimesteps, double autocorr_forDistance[], int max_distance, double *merged_autocorr) {
    double mean = 0;
    double sum = 0;
    for (int ix = 0; ix < nTimesteps; ++ix) {
        sum += valueSequence[ix];
    }
    mean = sum / nTimesteps;
#ifdef TEST
    printf("mean=%.2lf\n", mean);
#endif

    *merged_autocorr = 0.;
    for (int distance=1; distance <= max_distance; ++distance) {
        sum = 0;
        for (int firstIx=0; firstIx < nTimesteps - distance; ++firstIx) {
            sum += (valueSequence[firstIx] - mean) * (valueSequence[firstIx + distance] - mean);
#ifdef TEST
            printf("sum=%.2lf\n", sum);
#endif
        }
        autocorr_forDistance[distance-1] = sum / (nTimesteps - distance); 
        *merged_autocorr += autocorr_forDistance[distance-1];
#ifdef TEST
        printf("autocorr[%d]=%.2lf\n", distance-1, autocorr_forDistance[distance-1]);
        printf("merged_autocorr=%.2lf\n", *merged_autocorr);
#endif
    }

    return;
}


void test_set_autocorrelations() {
    int nTimesteps = 10;
    int nLevels_perTimestep = 3;
    int max_distance = nTimesteps - 2;  // for distance 0 and (nTimesteps-1)

    // double valueSequence[10] = {1.00, 1.00, 1.00, 1.00, 1.00, -1.00, -1.00, -1.00, -1.00, -1.00};
    double valueSequence[10] = {-1.00, -1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, -1.00, -1.00};
    double autocorr_forDistance[max_distance];
    double merged_autocorr;
    set_autocorrelations(valueSequence, nTimesteps, autocorr_forDistance, max_distance, &merged_autocorr); 
}


int main() {
    int nTimesteps = 10;
    int nLevels_perTimestep = 5;
    int max_distance = nTimesteps - 2;  // for distance 0 and (nTimesteps-1)

    int levels[nLevels_perTimestep];
    for (int ix=0; ix < nLevels_perTimestep; ++ix) {
        levels[ix] = ix * (2.0 / (nLevels_perTimestep -1)) - 1.0;
    }

    double max_valueSequence_forDistance[max_distance][nTimesteps];
    double max_autocorr_forDistance[max_distance];
    memset(max_autocorr_forDistance, 0, sizeof(double) * max_distance); 

    double valueSequence[nTimesteps];
    int indexSequence[nTimesteps];
    for (int ix=0; ix < nTimesteps; ++ix) {
        indexSequence[ix] = 0;
    }

    double max_merged_valueSequence[nTimesteps];
    for (int ix=0; ix < nTimesteps; ++ix) {
        max_merged_valueSequence[ix] = 0;
    }
    double merged_autocorr = 0.;
    double max_merged_autocorr = 0.;

    double progressUnit = 1.0/100;
    double progress = 0;
    long long nSequences = (long long) pow(nLevels_perTimestep, nTimesteps);
    double nSequences_float = (double) nSequences;
    printf("number of sequences = %lld\n", nSequences);
    printf("progress:\t");

    for (long long ix=0; ix < nSequences; ++ix) {
        if ((ix / nSequences_float) <= progress && ((ix +1) / nSequences_float) > progress) {
            printf("%.2lf\t", progress);
            fflush(stdout);
            progress += progressUnit;
        } 

        for (int timestep=0; timestep < nTimesteps; ++timestep) {
            valueSequence[timestep] = levels[indexSequence[timestep]];
        }
       
        double autocorr_forDistance[max_distance];
        set_autocorrelations(valueSequence, nTimesteps, autocorr_forDistance, max_distance, &merged_autocorr); 
        if (merged_autocorr > max_merged_autocorr) {
            max_merged_autocorr = merged_autocorr;
            memcpy(max_merged_valueSequence, valueSequence, sizeof(double) * nTimesteps);
        }

        for (int distance=1; distance <= max_distance; ++distance) {
            if (autocorr_forDistance[distance-1] > max_autocorr_forDistance[distance-1]) {
                max_autocorr_forDistance[distance-1] = autocorr_forDistance[distance-1];
                memcpy(max_valueSequence_forDistance[distance-1], valueSequence, sizeof(double) * nTimesteps);
            }
        }    

        indexSequence[0]++;
        int idx = 0;
        while (indexSequence[idx] == nLevels_perTimestep && idx < nTimesteps-1) { // overflow to higher positions
            indexSequence[idx] = 0;
            indexSequence[idx+1]++;
            idx++;
        }
    }

    printf("\nsequences with max autocorrelation\n"); 
    char buffer[10000];
    int nChars = 0;

    nChars += sprintf(buffer + nChars, "distance\t"); 
    for (int timestep=0; timestep < nTimesteps; ++timestep) {
        nChars += sprintf(buffer + nChars, "t%d\t", timestep); 
    }
    nChars += sprintf(buffer + nChars, "autocorr\n"); 

    for (int distance=1; distance <= max_distance; ++distance) {
        nChars += sprintf(buffer + nChars, "%d\t", distance);
        for (int timestep=0; timestep < nTimesteps; ++timestep) {
            nChars += sprintf(buffer + nChars, "%.2lf\t", max_valueSequence_forDistance[distance-1][timestep]);
        }
        nChars += sprintf(buffer + nChars, "%.2lf\n", max_autocorr_forDistance[distance-1]);
    }

    nChars += sprintf(buffer + nChars, "merged\t");
    for (int timestep=0; timestep < nTimesteps; ++timestep) {
        nChars += sprintf(buffer + nChars, "%.2lf\t", max_merged_valueSequence[timestep]);
    }
    nChars += sprintf(buffer + nChars, "%.2lf\n", max_merged_autocorr);


    FILE *fp = fopen("ac.csv", "wt");
    fprintf(fp, "%s", buffer);
    printf("%s", buffer);
}

