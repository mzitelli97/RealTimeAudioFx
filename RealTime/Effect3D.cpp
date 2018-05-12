#include "Effect3D.h"
#include "cmath"
#define PI 3.14159265358979323846 
#define SAMPLE_FREQ 44100.0


bool Effect3D::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	
	std::vector<float> leftOutput = std::vector<float>((unsigned)framesPerBuffer, 0);
	std::vector<float> rightOutput = std::vector<float>((unsigned)framesPerBuffer, 0);
	
	hsFilter(inputBuffer,&leftOutput , framesPerBuffer, props[0].getValue(),'L');
	hsFilter(inputBuffer, &rightOutput, framesPerBuffer, 180-props[0].getValue(),'R');
	float *out = (float*)outputBuffer;
	float *in = (float*)inputBuffer;
	//el output buffer para estereo va intercalando left y right
	for (unsigned long i = 0; i < framesPerBuffer; i++)
	{
		*out++ = leftOutput[i];
		*out++ = rightOutput[i];
	}




	return true;
}

Effect3D::Effect3D():Effect("3D")
{
	props = { Properties(std::string("LeftAngle"),0,360) };
	props[0].setValue(0);
	prevHSoutL = 0;
	prevHSoutL = 0;
	prevInL = 0;
	prevInR = 0;
	prevOutL = 0;
	prevOutR = 0;

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

bool Effect3D::hsFilter(const void * inputBuffer, std::vector<float> * outputBuffer, unsigned long framesPerBuffer, float theta, char channel)
{
	float Fs = sampleRate;
	float thetaO = 150;
	float c = 334.0;
	float a = 0.08;
	float wo = c / a;
	float alphamin = 0.05;
	float alpha = 1 + alphamin/2 + (1 - alphamin/2)*(cos( (theta/thetaO)*PI));
	float gDelay = 0;

	float *in = (float*)inputBuffer;
	std::vector<float>::iterator out= outputBuffer->begin();
	
	std::vector<float> buff= std::vector<float>((unsigned)framesPerBuffer, 0);
	std::vector<float>::iterator tempIt=buff.begin();

	
	float* prevIn = &prevInR;
	float* prevHSout = &prevHSoutR;
	float* prevOut = &prevOutR;
	
	if (channel == 'L')
	{
		prevIn = &prevInL;
		prevHSout = &prevHSoutL;
		prevOut = &prevOutL;
	}
	
	
	/*--------Head shadow---------*/
	*tempIt++ = (-(wo - Fs)*(*prevHSout) + (*in++)*(wo + alpha * Fs) + (wo - alpha * Fs)*(*prevIn)) / (wo + Fs);
	//*tempIt++ = *in++;
	for (unsigned long i = 0; i < framesPerBuffer - 1; i++, tempIt++, in++)
		//*tempIt = *in;
		*tempIt =( -(wo - Fs)*(*(tempIt - 1)) + (*in)*(wo +alpha*Fs) + (wo - alpha * Fs) * (*(in - 1)) )/(wo+Fs);
	
	*prevIn = *(--in);
	/*-------------Group Delay-------------------*/
	if (abs(theta) < 90)
		gDelay = -(Fs /wo)*(cos(theta*PI / 180) - 1);
	else
		gDelay = (Fs /wo )* ((abs(theta) - 90)*PI / 180 + 1);

	float gCoef = (1-gDelay)/(1+gDelay);
	/*------- IDT ---------------*/
	tempIt = buff.begin();
	//*out++ = *tempIt++;
	*out++ = -gCoef * (*prevOut) + gCoef * (*(tempIt++)) + *prevHSout;
	for (unsigned long i = 0; i < framesPerBuffer - 1; i++, out++, tempIt++)
		//*out = *tempIt;
		*out = -gCoef * (*(out - 1)) + gCoef * (*tempIt) + *(tempIt - 1);
	*prevHSout = *(--tempIt);
	*prevOut = *(--out);

	return false;
}




Effect3D::~Effect3D()
{
}



	/*
	float* prevIn = &prevInR;
	float* prevHSout = &prevHSoutR;
	float* prevOut = &prevOutR;
	/*--------Head shadow---------*/
	/*if (channel == 'L')
	{
		prevIn = &prevInL;
		prevHSout = &prevHSoutL;
		prevOut = &prevOutL;
	}*/