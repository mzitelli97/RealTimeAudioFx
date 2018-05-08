#pragma once

typedef enum {Sine, Triangular, Square, Sawtooth}waveformType;
float lfo(float sampleRate, float freq, waveformType waveType);