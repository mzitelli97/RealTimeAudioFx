#pragma once
class NotchFilter2Order
{
public:
	NotchFilter2Order();
	virtual ~NotchFilter2Order();
	float processSingleSampleRaw(const float sample);
	bool calculateCoeff(float centerFreq, float sampleFreq);
private:
	float a_0, a_1, a_2, b_0, b_1, b_2;	//Filter coefficient
	float x_1, x_2, y_1, y_2;	//Previous samples (x[n-1] and y[n-1])
};

