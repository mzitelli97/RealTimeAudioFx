#include "Effect3D.h"
#include "cmath"
#define PI 3.14159265358979323846 



bool Effect3D::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{






	return false;
}

Effect3D::Effect3D():Effect("3D")
{

}

bool Effect3D::hsFilter(const void * inputBuffer, unsigned long framesPerBuffer, void * outputBuffer, double theta, double Fs)
{
	//theta += 90; //In degrees
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
	*it++ = *in++*(wo + alpha * Fs);
	
	for (unsigned long i = 0; i < framesPerBuffer; i++,it++,in++) 
	{
		*it =( -(wo - Fs)*(*(it - 1)) + (*in)*(wo + alpha * Fs) + (wo - alpha * Fs)*(*(in - 1)) )/(wo-Fs);
	}

	if (abs(theta) < 90)
		gDelay = -Fs / (wo*(cos(theta*PI / 180) - 1));
	else
		gDelay = Fs / wo * ((abs(theta) - 90)*PI / 180 + 1);



	/*for (unsigned long i = 0; i < framesPerBuffer; i++, out++, in++)
	{
		*out = (-(wo - Fs)*(*(out - 1)) + (*in)*(wo + alpha * Fs) + (wo - alpha * Fs)*(*(in - 1))) / (wo - Fs);
	}*/







	return false;
}




Effect3D::~Effect3D()
{
}
