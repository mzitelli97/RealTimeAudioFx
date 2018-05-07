#include "EffectWhisper.h"

const float PI = 3.141592653589793238460f;

EffectWhisper::EffectWhisper():Effect("Whisper")
{
}
bool EffectWhisper::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
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
		in[i + framesPerBuffer] = din[i] * (float)(1.0 / 2.0 * (1.0 - cos(2 * PI*i / (framesPerBuffer - 1))));

	//Primera fft
	fft(temp.data(), in.data(), framesPerBuffer);
	for (auto& c : temp)
		c = polar(abs(c), 2 * PI*(float)rand() / RAND_MAX);
	ifft(temp.data(), temp.data(), framesPerBuffer);
	for (unsigned i = 0; i < framesPerBuffer / 2; i++)
		dout[i] = temp[i + framesPerBuffer / 2].real();
	//Segunda fft
	fft(temp.data(), in.data() + framesPerBuffer / 2, framesPerBuffer);
	for (auto& c : temp)

		c = polar(abs(c), 2 * PI*(float)rand() / RAND_MAX);
	ifft(temp.data(), temp.data(), framesPerBuffer);
	for (unsigned i = 0; i < framesPerBuffer / 2; i++)
	{
		dout[i] = (dout[i] + temp[i].real());
		dout[i + framesPerBuffer / 2] = temp[i + framesPerBuffer / 2].real();
	}
	//Tercera fft
	fft(temp.data(), in.data() + framesPerBuffer, framesPerBuffer);
	for (auto& c : temp)

		c = polar(abs(c), 2 * PI*(float)rand() / RAND_MAX);
	ifft(temp.data(), temp.data(), framesPerBuffer);
	for (unsigned i = 0; i < framesPerBuffer / 2; i++)
		dout[i + framesPerBuffer / 2] = (dout[i + framesPerBuffer / 2] + temp[i].real());
	for (unsigned i = 0; i < framesPerBuffer; i += 1)
	{
		dout[2 * framesPerBuffer - 1 - 2 * i] = dout[framesPerBuffer - 1 - i];
		dout[2 * framesPerBuffer - 2 - 2 * i] = dout[framesPerBuffer - 1 - i];
	}
	return true;
}


EffectWhisper::~EffectWhisper()
{
}
