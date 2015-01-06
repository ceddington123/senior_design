#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define NUM_SAMPLES (8192*4)
#define RANGE 4000ls


void fft(int32_t m, double *x, double *y);

double findMaxFrequency(double *x, int samples);

void findPitch(double *x, int samples);

int main()
{
    FILE *in  = NULL;
    FILE *out = stdout;

    in = fopen("../audio_files/e5d.wav", "r");

    uint32_t sample_rate = 0;
    uint16_t num_channels = 0, bps = 0;

    fseek(in, 22, SEEK_SET);

    fread(&num_channels, sizeof(num_channels), 1, in);
    fread(&sample_rate, sizeof(sample_rate), 1, in);

    fseek(in, 34, SEEK_SET);

    fread(&bps, sizeof(bps), 1, in);

    //printf("Num channels: %d\n", num_channels);
    //printf("Sample Rate: %d\n", sample_rate);
    //printf("BPS: %d\n", bps);

    //exit(EXIT_SUCCESS);

    if (num_channels != 1) {
        fprintf(stderr, "mono sound only\n");
        exit(EXIT_FAILURE);
    }
    if (bps != 16) {
        fprintf(stderr, "16 BPS\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    double bins[NUM_SAMPLES] = { 0.0 };
    double frequency = 0.0;   
/*
#ifndef PLOT
    for(i = 0; i < RANGE; ++i)
    {
        bins[i] = frequency;
        if(i > 900 && i < 1000)
            printf("Bin %d value: %f\n", i, bins[i]);
        frequency += (44100.0/(NUM_SAMPLES));
    }
#endif
*/

//    printf("sample_rate: %d\n", sample_rate);
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

//#ifdef PLOT
    for (i = 0; i < c/2; ++i) {
        printf("%d\n", (int)rsamples[i]);
        //if(i % 2 == 0)
        //    system("wait");
    }
//#endif

//#ifndef PLOT
//    findMaxFrequency(rsamples, c/2);
//#endif

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

double findMaxFrequency(double *x, int samples)
{
    int i = 0;
    double max = x[0];
    double max_frequency = 0.0;
    double frequency = 0.0;

    printf("Samples: %d\n", samples);

    for(i = 0; i < samples; ++i)
    {
        if(x[i] > max)
        {
            max = x[i];
            printf("Updating max_frequency.\nOld value: %f\n", max_frequency);
            max_frequency = frequency;
            printf("New value: %f\n", max_frequency);
        }
        frequency += (44100.0/(samples*2));
    }

    printf("Max value of %f at %f.\n\n", max, max_frequency);

    return max_frequency;
}

void findPitch(double *x, int samples)
{
    
}


