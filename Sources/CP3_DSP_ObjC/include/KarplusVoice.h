//
//  KarplusVoice.h
//  karpluck
//
//  Created by Corné Driesprong on 30/05/2023.
//

#pragma once

#ifdef __cplusplus

class KarplusVoice {
public:
    KarplusVoice();
    void pluck(double freq, double sampleRate, double damping, double tone, double excitation);
    double process();
    
private:
    int mPeriod = 0;
    int mWindowLength = 0;
    int mBufferPos = 0;
    double *mBuffer;
    
    int calculatePeriodInSamples(double frequency, double sampleRate) {
        return static_cast<int>(sampleRate / frequency);
    }
};

#endif
