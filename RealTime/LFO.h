#pragma once

typedef enum {Sine, Triangular, Square, Sawtooth}waveformType;

//Oscilador de baja frecuencia de diferentes formas de onda
float lfo(float sampleRate, float freq, waveformType waveType);