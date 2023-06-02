//
//  KarplusVoice.cpp
//  
//
//  Created by Corné Driesprong on 02/06/2023.
//

#import "KarplusVoice.h"

#include <random>

// this is the number of samples we need to represent a full period
// of the lowest possible MIDI pitch, i.e., C-1 at 48000 Kz = 5871.3 samples
#define MAX_BUFFER_SIZE (5872)

KarplusVoice::KarplusVoice() {
    // allocate noise buffer
    // WARNING: don't call on the audio thread
    mBuffer = new double[MAX_BUFFER_SIZE];
    for (int i = 0; i < MAX_BUFFER_SIZE; i++)
        mBuffer[i] = 0.0;
}

void KarplusVoice::pluck(double freq, double sampleRate) {
    // convert frequency to period in samples
    mPeriod = calculatePeriodInSamples(freq, sampleRate);
    // pitch tracking: higher pitches should ring out longer
    mPitchTracking = fmax(5.0, (double)mPeriod / 7.);
    
    mBufferPos = 0;
    
    // fill noise buffer with -1.0 and 1.0 values
    for (int i = 0; i < mPeriod; i++) {
        mBuffer[i] = (rand() % 2 == 0) ? -1.0 : 1.0;
    }
}

double KarplusVoice::process(double damping) {
    // if buffer is empty, do nothing
    if (mPeriod == 0) return 0.0;
    
    // loop around noise buffer
    mBufferPos = (mBufferPos + 1) % mPeriod;
    
    // determine window length, bigger window = more smoothing
    int windowLength = std::max(2, (int)round(damping * mPitchTracking));
    
    // smooth signal using simple moving average
    double sum = 0;
    for (int i = 0; i < windowLength; i++) {
        int index = (mBufferPos + i) % mPeriod;
        sum += mBuffer[index];
    }
    mBuffer[mBufferPos] = sum / windowLength;
    
    return mBuffer[mBufferPos];
}

int mPeriod = 0;
double mPitchTracking = 0;
int mBufferPos = 0;
double *mBuffer;

int calculatePeriodInSamples(double frequency, double sampleRate) {
    return static_cast<int>(sampleRate / frequency);
}
