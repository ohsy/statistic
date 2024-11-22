
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


int main() {
    int nTimesteps = 10;
    int nLevels_perTimestep = 3;
    int max_distance = nTimesteps - 2;  // for distance 0 and (nTimesteps-1)

    // double valueSequence[10] = {1.00, 1.00, 1.00, 1.00, 1.00, -1.00, -1.00, -1.00, -1.00, -1.00};
    // double valueSequence[10] = {-1.00, -1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, -1.00, -1.00};
    // double valueSequence[10] = {1.00, -1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, -1.00};
    double valueSequence[10] = {-1.00, -1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, -1.00, -1.00};
    double autocorr_forDistance[max_distance];
    double merged_autocorr;
    set_autocorrelations(valueSequence, nTimesteps, autocorr_forDistance, max_distance, &merged_autocorr); 

    printf("valueSequence=");
    for (int ix=0; ix < nTimesteps; ++ix) {
        printf("\t%.2lf", valueSequence[ix]);
    }
    printf("\n");

    for (int ix=0; ix < max_distance; ++ix) {
        printf("autocorr_forDistance[%d]=%.2lf\n", ix, autocorr_forDistance[ix]);
    }

    printf("merged_autocorr=%.2lf\n", merged_autocorr);

}


