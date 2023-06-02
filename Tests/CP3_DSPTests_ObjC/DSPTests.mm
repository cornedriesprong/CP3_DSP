#import <Foundation/Foundation.h>
#import <XCTest/XCTest.h>
#import <CP3_DSP_ObjC.h>

@interface DSPTests : XCTestCase

@end

@implementation DSPTests

- (void) testKarplusVoice {

    double frequencies[13] = { 6.875, 13.75, 27.5, 55, 110, 220, 440, 880, 1760, 3520, 7040, 14080, 28160 };
    double samplesRates[2] = { 44100.0, 48000.0 };
    
    for (int i = 0; i < 13; i++) {
        double freq = frequencies[i];
        for (int j = 0; j < 2; j++) {
            double sampleRate = samplesRates[j];
            [self testKarplusWithFrequency:freq andSampleRate:sampleRate];
        }
    }
}

- (void) testKarplusWithFrequency:(double)frequency andSampleRate:(double)sampleRate {
    
    KarplusVoice *karplusVoice = new KarplusVoice();
    
    double damping = 0.5;
    double tone = 1.0;
    double excitation = 0.2;
    karplusVoice->pluck(frequency, sampleRate, damping, tone, excitation);
    NSLog(@"testing Karplus voice with frequency %f and sample rate %f", frequency, sampleRate);
   
    unsigned long seconds = 20;
    unsigned long totalSamples = (long)sampleRate * seconds;
    double signal[totalSamples];
    
    // generate n seconds of audio
    for (unsigned long i = 0; i < totalSamples; i++) {
        double sample = karplusVoice->process();
       
        // make sure samples don't clip
        XCTAssert(sample >= -1.0);
        XCTAssert(sample <= 1.0);
        
        signal[i] = sample;
    }
    
        
    for (int i = 0; i < totalSamples; i++) {
        // if 10 subsequent samples are the same,
        // we assume the signal has gone quiet
        bool equal = true;
        for (int j = i + 1; j < i + 10; ++j) {
            if (signal[j] != signal[i]) {
                equal = false;
            }
        }
        
        if (equal) {
            NSLog(@"signal quiet at sample %d (%f seconds)\n", i, (double)i / sampleRate);
            break;
        }
    }
}

@end
