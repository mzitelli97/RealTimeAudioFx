#pragma once
class AllPassFilter
{
public:
	AllPassFilter();
	virtual ~AllPassFilter();
	float processSingleSampleRaw(const float sample);
	bool calculateCoeff(float centerFreq, float sampleFreq);
private:
	float b_0, b_1, a_1;	//Filter coefficients
	float x_1, y_1;	//Previous samples (x[n-1] and y[n-1])
};

