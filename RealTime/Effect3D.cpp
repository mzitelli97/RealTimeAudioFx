#include "Effect3D.h"
#include "cmath"
#define PI 3.14159265358979323846 
#define SAMPLE_FREQ 44100


bool Effect3D::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	hsFilter(inputBuffer, outputBuffer, framesPerBuffer, 90);

	return false;
}

Effect3D::Effect3D():Effect("3D")
{

}

bool Effect3D::hsFilter(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer, float theta)
{
	//theta += 90; //In degrees
	float Fs = SAMPLE_FREQ;
	float thetaO = 150;
	float c = 334;
	float a = 0.08;
	float wo = c / a;
	float alphamin = 0.05;
	float alpha = 1 + alphamin + (1 - alphamin)*(cos( (theta/thetaO) *PI));
	float *in = (float *)inputBuffer;
	float *out= (float *)outputBuffer;
	
	float gDelay = 0;
	std::vector<float> buff= std::vector<float>((unsigned)framesPerBuffer, 0);
	std::vector<float>::iterator it=buff.begin();
	
	/*--------Head shadow---------*/
	*it++ = *in++*(wo + alpha * Fs);
	for (unsigned long i = 0; i < framesPerBuffer; i++,it++,in++) 
		*it =( -(wo - Fs)*(*(it - 1)) + (*in)*(wo + alpha * Fs) + (wo - alpha * Fs)*(*(in - 1)) )/(wo-Fs);

	if (abs(theta) < 90)
		gDelay = -Fs / (wo*(cos(theta*PI / 180) - 1));
	else
		gDelay = Fs / wo * ((abs(theta) - 90)*PI / 180 + 1);
	
	float gCoef = (1-gDelay)/(1+gDelay);
	/*------- IDT ---------------*/
	it = buff.begin();
	*out++ = gCoef *(*it++);
	for (unsigned long i = 0; i < framesPerBuffer; i++, out++, it++)
		*out = -gCoef * (*(out - 1)) + gCoef * (*it) + *(it - 1);

	return false;
}




Effect3D::~Effect3D()
{
}
