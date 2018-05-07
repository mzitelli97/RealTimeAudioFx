#pragma once
#include <vector>
#include <iostream>
#include "sndfile.h"
#include "Effect.h"
#include "portaudio.h"
class RealTimeEffects
{
public:
	RealTimeEffects(std::vector<Effect*>& eff,unsigned sR);

	bool start();
	bool run();
	bool stop();

	void next(const void *inputBuffer, void *outputBuffer,unsigned long framesPerBuffer);
	bool setEffect(unsigned i);
	std::vector<Effect*> getEffects();
	std::vector<Properties> getEffectProps(unsigned i);
	bool setEffectProp(unsigned i, double val);

	std::string getError();
	~RealTimeEffects();
private:
	/* Port Audio info*/
	static int callback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);
	PaStream				*stream;

	PaStreamParameters		inputParameters, outputParameters;


	float sampleRate;
	unsigned buffSize;

	std::string error;
	std::vector<Effect*> effects;
	bool running;
	unsigned currEff;
	unsigned mode; //1 audio - 2 wav
	unsigned inChannels;
	unsigned outChannels;
	//Wav data
	std::vector<float> wav;
	unsigned offset;
};

