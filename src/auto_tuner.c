#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
//#include <floats.h>

#define NUM_SAMPLES (8192*4)
#define RANGE 4000
#define THRESHOLD 100
//#define FLT_ROUNDS 1

int16_t maxFreqs[100];
int16_t freqCount = 0;

const float E2 = 82.41;
const float A2 = 110.00;
const float D3 = 146.80;
const float G3 = 196.00;
const float B3 = 246.90;
const float E4 = 329.60;

const float MIN_FREQ = 40.0;

double bins[RANGE] = { 0.0 };

/************************
*   DEBUG DEFINITIONS   *
************************/

//#define PLOT 1
//#define DEBUG_THRESH
#define DEBUG_FREQ
//#define DEBUG_BINS

void fft(int32_t m, double *x, double *y);

double findMaxFrequencies(double *x, int *y, int samples);

void findPitch(double *x, int samples);

int main()
{
    FILE *in  = NULL;
    FILE *out = stdout;

    in = fopen("../audio_files/d3d.wav", "r");

    uint32_t sample_rate = 0;
    uint16_t num_channels = 0, bps = 0;

    fseek(in, 22, SEEK_SET);

    fread(&num_channels, sizeof(num_channels), 1, in);
    fread(&sample_rate, sizeof(sample_rate), 1, in);

    fseek(in, 34, SEEK_SET);

    fread(&bps, sizeof(bps), 1, in);

    if (num_channels != 1) {
        fprintf(stderr, "mono sound only\n");
        exit(EXIT_FAILURE);
    }
    if (bps != 16) {
        fprintf(stderr, "16 BPS\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    double frequency = 0.0;   

    int c = 0;
    int16_t sample = 0;
    double  rsamples[NUM_SAMPLES] = { 0.0 };
    double  csamples[NUM_SAMPLES] = { 0.0 };

    while(c < NUM_SAMPLES && fread(&sample, sizeof(sample), 1, in) > 0) {
        rsamples[c++] = (double)sample;
    }
		
    int pow = 0, freq = NUM_SAMPLES;

    while (freq >>= 1)
        pow++;

    fft(pow, rsamples, csamples);

#ifdef PLOT
    for (i = 0; i < c/2; ++i) {
        printf("%d\n", abs((int)rsamples[i]));
    }
#endif

int32_t average = 0;

#ifdef DEBUG_THRESH
    for (i = 0; i < c/2; ++i) {
        if(rsamples[i] > 0)
            average += rsamples[i];
    }
    printf("Average: %.2f\n", (double)average/c);  
#endif

int j = 0;
int k = 0;
int local_count = 0;
int index[100] = { 0 };
int local_index[50] = { 0 };
int local_max_index[30] = { 0 };
int all_local_counts[30]    = { 0 };
int max_count = 0;  
double temp_sample = 0.0;
double prev_freq = 0.0;
double local_responses[50] = { 0.0 };
double local_maxes[30] = { 0.0 };
double idiff = 0.0;
double adiff = 0.0;
double all_local_responses[30][50] = {{ 0.0 }};
double bins_copy[RANGE] = { 0.0 };
int all_local_indeces[30][50]   = {{ 0 }};

#ifdef DEBUG_FREQ
    frequency = 0.0;

    //create freq bins
    for(i = 0; i < RANGE; ++i) {
        bins[i] = frequency;
        frequency += (44100.0/(NUM_SAMPLES));
    }

    findMaxFrequencies(rsamples, index, c/2);

    //printf("\n\n**********MAX FREQUENCY INFORMATION**********\n\n");

    prev_freq = bins[index[j]];

    //dump frequencies and find max local frequencies
    for(j = 0; j < freqCount; j++) {

        if(bins[index[j]] - prev_freq > 50.0) {
            //printf("\n***Finding a Local Max***");
            local_maxes[max_count] = local_responses[0];
            local_max_index[max_count] = local_index[0];
            all_local_counts[max_count] = local_count;
            for(k = 0; k < local_count; k++) {
                all_local_responses[max_count][k] = local_responses[k];
                all_local_indeces[max_count][k] = local_index[k];
                if(local_responses[k] > local_maxes[max_count]) {
                    local_max_index[max_count] = local_index[k];
                    local_maxes[max_count] = local_responses[k];
                }
            }

            //printf("\nA local max of %.2f has been found at index %d with a frequency of %.2f\n", local_maxes[max_count], local_max_index[max_count], bins[local_max_index[max_count]]);

            local_count = 0;
            max_count++;

            //printf("***DONE***\n\n");
        }

        if(rsamples[index[j]] < 0.0) {
            temp_sample = rsamples[index[j]] * -1.0;
            //printf("A value of %.2f was found at frequency %.2f at index %d\n", temp_sample, bins[index[j]], index[j]);
        }
        else {
            temp_sample = rsamples[index[j]];
            //printf("A value of %.2f was found at frequency %.2f at index %d\n", rsamples[index[j]], bins[index[j]], index[j]);
        }

        local_responses[local_count] = temp_sample;
        local_index[local_count] = index[j];
        local_count++;

        prev_freq = bins[index[j]];
    }

    for(j = 0; j < max_count; j++) {
        idiff = bins[local_max_index[j]] - (E2*(j+1));
        adiff = bins[local_max_index[j]] - (bins[local_max_index[0]]*(j+1));

        //printf("A max was found at %.2f\n", bins[local_max_index[j]]);
        //printf("The difference between the ideal note and the %d overtone is: %.4f\n", j, idiff);
        //printf("The difference between the actual note and the first overtone is: %.4f\n\n", adiff);
    }

    double converted_frequency = 0.0;
    int converted_index = 0;

    for(j = 0; j < max_count; j++) {
        //printf("all_local_counts[j]: %d\n", all_local_counts[j]);
        for(k = 0; k < all_local_counts[j]; k++) {
            //printf("bins[all_local_indeces[j][k]]: %.3f\n", bins[all_local_indeces[j][k]]);
            //printf("all_local_indeces[j][k]: %d\n", all_local_indeces[j][k]);
            //printf("Frequency before change conversion: %.2f\n", bins[all_local_indeces[j][k]]);
            converted_frequency = ((bins[all_local_indeces[j][k]])*(1.0/(j+1)));
            converted_index = rint(((all_local_indeces[j][k])*(1.0/(j+1))));
            //printf("The converted frequency is %.2f at a converted index of %d with response of %.2f\n\n", converted_frequency, converted_index, all_local_responses[j][k]);
            bins_copy[converted_index] += converted_frequency;
        }
    }
    
    //printf("Index 61 is: %.2f\n", bins[61]);    
    //printf("Index 62 is: %.2f\n", bins[62]);

    for(j = 0; j < c/2; j++) {
        printf("%d\n", (int)bins_copy[j]);
    }

#endif

    return 0;
}

/*
   This computes an in-place complex-to-complex FFT 
   x and y are the real and imaginary arrays of 2^m points.
   dir =  1 gives forward transform
   dir = -1 gives reverse transform 
*/
void fft(int32_t m, double *x, double *y)
{
   long n,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   n = 1;
   for (i=0;i<m;i++) 
      n *= 2;

   /* Do the bit reversal */
   i2 = n >> 1;
   j = 0;
   for (i=0;i<n-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0; 
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<n;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1; 
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   
  for (i=0;i<n;i++) {
     x[i] /= n;
     y[i] /= n;
  }
}

double findMaxFrequencies(double *x, int *y, int samples)
{
    int i = 0;
    double temp = 0.0;
    double frequency = 0.0;
    
    //printf("Samples: %d\n", samples);

    for(i = 0; i < RANGE; ++i)
    {
        if(abs(x[i]) > THRESHOLD && bins[i] > MIN_FREQ)
        {
            //if(x[i] < 0) {
                temp = x[i] * -1.0;
            //    printf("Value over threshold:%.2f\n", temp);
            //}
            //else
            //    printf("Value over threshold:%.2f\n", x[i]);

            //printf("Index value         :%d\n", i);
            y[freqCount] = i;
            freqCount++;
        }
    }

    return x[0];
}

void findPitch(double *x, int samples)
{
    
}


