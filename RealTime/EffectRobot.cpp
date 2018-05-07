#include "EffectRobot.h"

const float PI = 3.141592653589793238460f;

EffectRobot::EffectRobot():Effect("Robot")
{
}

bool EffectRobot::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	if (in.size() == 0)
	{
		in = vector<complex<float>>(framesPerBuffer * 2);
		temp = vector<complex<float>>(framesPerBuffer);
	}

	float* din = (float*)inputBuffer, *dout = (float*)outputBuffer;
	//Copio los datos para avanzar con el overlap
	memcpy(in.data(), in.data() + framesPerBuffer, sizeof(complex<float>)*framesPerBuffer);
	//Ventaneo con ventana hanning y guardo los valores en el segundo segmento de la memoria
	for (unsigned i = 0; i < framesPerBuffer; i++)
		in[i + framesPerBuffer] = din[i];

	//Primera fft
	//Copio los datos ventaneando
	for (unsigned i = 0; i<framesPerBuffer; i++)
		temp[i] = in[i] * (float)(1.0 / 2.0 * (1.0 - cos(2 * PI*i / (framesPerBuffer - 1))));
	fft(temp.data(), in.data(), framesPerBuffer);
	for (auto& c : temp)
		c = abs(c);
	ifft(temp.data(), temp.data(), framesPerBuffer);
	for (unsigned i = 0; i < framesPerBuffer / 2; i++)
		dout[i] = temp[i + framesPerBuffer / 2].real();
	//Segunda fft
	//Copio los datos ventaneando
	for (unsigned i = 0; i<framesPerBuffer; i++)
		temp[i] = in[i + framesPerBuffer / 2] * (float)(1.0 / 2.0 * (1.0 - cos(2 * PI*i / (framesPerBuffer - 1))));
	fft(temp.data(), in.data() + framesPerBuffer / 2, framesPerBuffer);
	for (auto& c : temp)
		c = abs(c);
	ifft(temp.data(), temp.data(), framesPerBuffer);
	for (unsigned i = 0; i < framesPerBuffer / 2; i++)
	{
		dout[i] = (dout[i] + temp[i].real());
		dout[i + framesPerBuffer / 2] = temp[i + framesPerBuffer / 2].real();
	}
	//Tercera fft
	//Copio los datos ventaneando
	for (unsigned i = 0; i<framesPerBuffer; i++)
		temp[i] = in[i + framesPerBuffer] * (float)(1.0 / 2.0 * (1.0 - cos(2 * PI*i / (framesPerBuffer - 1))));
	fft(temp.data(), in.data() + framesPerBuffer, framesPerBuffer);
	for (auto& c : temp)
		c = abs(c);
	ifft(temp.data(), temp.data(), framesPerBuffer);
	for (unsigned i = 0; i < framesPerBuffer / 2; i++)
		dout[i + framesPerBuffer / 2] = (dout[i + framesPerBuffer / 2] + temp[i].real());
	for (unsigned i = 0; i < framesPerBuffer / 8; i++)
	{
		dout[i]=dout[i]*(float)(1.0 / 2.0 * (1.0 - cos(2 * PI*i / (framesPerBuffer/4 - 1))));
		dout[framesPerBuffer-1-i] = dout[framesPerBuffer - 1 - i] * (float)(1.0 / 2.0 * (1.0 - cos(2 * PI*i / (framesPerBuffer / 4 - 1))));
	}
	for (unsigned i = 2; i < framesPerBuffer; i++)
		dout[i] = (dout[i] + dout[i - 1] + dout[i - 2]) / 4;
	for (unsigned i = 0; i < framesPerBuffer; i += 1)
	{
		dout[2 * framesPerBuffer - 1 - 2 * i] = dout[framesPerBuffer - 1 - i];
		dout[2 * framesPerBuffer - 2 - 2 * i] = dout[framesPerBuffer - 1 - i];
	}
	return true;
}


EffectRobot::~EffectRobot()
{
}
