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

void KarplusVoice::pluck(double freq,
                         double sampleRate,
                         double damping,
                         double tone,
                         double excitation) {
    // convert frequency to period in samples
    mPeriod = calculatePeriodInSamples(freq, sampleRate);
    // pitch tracking: higher pitches should ring out longer
    double pitchTracking = fmax(5.0, (double)mPeriod / 7.);
    // determine window length, bigger window = more smoothing
    double scaledDamping = pow((damping * 0.75) + 0.25, 2);
    mWindowLength = std::max(2, (int)round(scaledDamping * pitchTracking));
    mBufferPos = 0;
    
    // mix triangle vs noise
    double mix = tone / 2.;
    double pluckLength = mPeriod * excitation;
    // fill noise buffer with -1.0 and 1.0 values
    for (int i = 0; i < mPeriod; i++) {
        // pad buffer with silence after pluck length
        if (i > pluckLength) {
            mBuffer[i] = 0.;
            continue;
        }
        // generate and mix triangle wave
        double phase = (double)i / mPeriod;
        double triangle = 0;
        if (phase < 0.25) {
            triangle = 4 * phase;
        } else if (phase < 0.75) {
            triangle = 2 - 4 * phase;
        } else {
            triangle = -4 + 4 * phase;
        }
        mBuffer[i] += triangle * mix;
       
        // mix noise
        mBuffer[i] += ((rand() % 2 == 0) ? -1.0 : 1.0) * (1.0 - mix);
    }
}

double KarplusVoice::process() {
    // if buffer is empty, do nothing
    if (mPeriod == 0) return 0.0;
    
    // loop around noise buffer
    mBufferPos = (mBufferPos + 1) % mPeriod;
    
    // smooth signal using simple moving average
    double sum = 0;
    for (int i = 0; i < mWindowLength; i++) {
        int index = (mBufferPos + i) % mPeriod;
        sum += mBuffer[index];
    }
    mBuffer[mBufferPos] = sum / (double)mWindowLength;
    
    return mBuffer[mBufferPos];
}

int mPeriod = 0;
int mWindowLength = 0;
int mBufferPos = 0;
double *mBuffer;

int calculatePeriodInSamples(double frequency, double sampleRate) {
    return static_cast<int>(sampleRate / frequency);
}
