#pragma once
#include <vector>
#include "Effect.h"
#include "portaudio.h"
class RealTimeEffects
{
public:
	RealTimeEffects(std::vector<Effect*>& eff);

	bool start();
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


	double sampleRate;
	unsigned buffSize;

	std::string error;
	std::vector<Effect*> effects;
	bool running;
	unsigned currEff;
};

