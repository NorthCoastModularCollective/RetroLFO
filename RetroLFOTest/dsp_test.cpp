// tests.cpp
#include "../RetroLFO/dsp.h"
#include "gtest/gtest.h"
#include <math.h>
 
TEST(DSPTest, SinOscillator) { 
    float sampleRate = 44100;
    float freq = 1;
    SinOsc osc = SinOsc::make(freq, sampleRate);

    ASSERT_GT(0.0001,abs(0-osc(0)));
    ASSERT_GT(0.0001,abs(1-osc(sampleRate/4)));
    ASSERT_GT(0.0001,abs(0-osc(2*sampleRate/4)));
    ASSERT_GT(0.0001,abs(-1-osc(3*sampleRate/4)));
    ASSERT_GT(0.0001,abs(0-osc(4*sampleRate/4)));
    ASSERT_GT(0.0001,abs(1-osc(5*sampleRate/4)));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}