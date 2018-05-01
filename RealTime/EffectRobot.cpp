#include "EffectRobot.h"

const float PI = 3.141592653589793238460;

EffectRobot::EffectRobot():Effect("Robot")
{
}

bool EffectRobot::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	vector<complex<float>> in(framesPerBuffer);
	float* din = (float*)inputBuffer,*dout= (float*)outputBuffer;
	//Ventaneo con ventana hanning
	for (unsigned i = 0; i < framesPerBuffer; i++)
		in[i] = din[i]*(float)(1.0 / 2.0 * (1.0 - cos(2 * PI*i / (framesPerBuffer - 1))));
	fft(in.data(), in.data(), in.size());
	for (auto& c : in)
		c = abs(c);
	ifft(in.data(), in.data(), in.size());
	for (unsigned i = 0; i < framesPerBuffer; i++)
		dout[(i)] = in[i].real();
		//dout[(i+framesPerBuffer/2)%framesPerBuffer] = in[i].real();

	return true;
}


EffectRobot::~EffectRobot()
{
}
