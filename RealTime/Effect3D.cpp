#include "Effect3D.h"
#include "cmath"
#define PI 3.14159265358979323846 
#define SAMPLE_FREQ 44100


bool Effect3D::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	
	std::vector<float> leftOutput = std::vector<float>((unsigned)framesPerBuffer, 0);
	std::vector<float> rightOutput = std::vector<float>((unsigned)framesPerBuffer, 0);
	
	hsFilter(inputBuffer,&leftOutput , framesPerBuffer, 0);
	hsFilter(inputBuffer, &rightOutput, framesPerBuffer, 180);
	float *out = (float*)outputBuffer;
	float *in = (float*)inputBuffer;
	//el output buffer para estereo va intercalando left y right
	for (unsigned long i = 0; i < framesPerBuffer; i++)
	{
		*out++ = leftOutput[i];
		*out++ = rightOutput[i];
	}




	return false;
}

Effect3D::Effect3D():Effect("3D")
{
	props = { Properties(std::string("ThetaLeft"),0,360),Properties(std::string("ThetaRight"),0,360) };
	props[0].setValue(0);
	props[1].setValue(180);

}


bool Effect3D::setProp(unsigned i, double v)
{
	bool ret = false;
	if (i < props.size())
	{
		ret = props[i].setValue(v);
	}
	return ret;
}

bool Effect3D::hsFilter(const void * inputBuffer, std::vector<float> * outputBuffer, unsigned long framesPerBuffer, float theta)
{
	float Fs = SAMPLE_FREQ;
	float thetaO = 150;
	float c = 334;
	float a = 0.08;
	float wo = c / a;
	float alphamin = 0.05;
	float alpha = 1 + alphamin + (1 - alphamin)*(cos( (theta/thetaO) *PI));
	float gDelay = 0;

	float *in = (float*)inputBuffer;
	std::vector<float>::iterator out= outputBuffer->begin();
	
	std::vector<float> buff= std::vector<float>((unsigned)framesPerBuffer, 0);
	std::vector<float>::iterator tempIt=buff.begin();
	
	/*--------Head shadow---------*/
	*tempIt++ = *in++*(wo + alpha * Fs);
	for (unsigned long i = 0; i < framesPerBuffer - 1; i++, tempIt++, in++)
		//*tempIt = *in;
		*tempIt =( -(wo - Fs)*(*(tempIt - 1)) + (*in)*(wo + alpha * Fs) + (wo - alpha * Fs)*(*(in - 1)) )/(wo-Fs);

	if (abs(theta) < 90)
		gDelay = -(Fs /wo)*(cos(theta*PI / 180) - 1);
	else
		gDelay = (Fs /wo )* ((abs(theta) - 90)*PI / 180 + 1);
	
	float gCoef = (1-gDelay)/(1+gDelay);
	/*------- IDT ---------------*/
	tempIt = buff.begin();
	*out++ = gCoef *(*tempIt++);
	for (unsigned long i = 0; i < framesPerBuffer - 1; i++, out++, tempIt++)
		//*out = *tempIt;
		*out = -gCoef * (*(out - 1)) + gCoef * (*tempIt) + *(tempIt - 1);

	return false;
}




Effect3D::~Effect3D()
{
}
