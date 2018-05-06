#pragma once

typedef enum {Sine, Triangular}waveformType;
class LFO
{
public:
	LFO();
	virtual ~LFO();
	float execute_lfo(float sampleRate, float freq, waveformType waveType);
};

