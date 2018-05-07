#include "RealTimeEffects.h"

int RealTimeEffects::callback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	RealTimeEffects* ef = (RealTimeEffects*)userData;
	if (ef->mode == 2)
	{
		
		if (ef->offset >= ef->wav.size())
			ef->offset = 0;
		//memcpy(outputBuffer,ef->wav.data()+ef->offset , sizeof(float)*framesPerBuffer);
		ef->next(ef->wav.data() + ef->offset, outputBuffer, framesPerBuffer);
		ef->offset += framesPerBuffer;
	}
	else
		ef->next(inputBuffer, outputBuffer, framesPerBuffer);
	return paContinue;
}

RealTimeEffects::RealTimeEffects(std::vector<Effect*>& eff,unsigned sR)
{
	effects = eff;
	running = false;
	currEff = 0;
	sampleRate = sR;
	buffSize = 512;
	mode = 0;
	inChannels = 1;
	outChannels = 2;
}

bool RealTimeEffects::start()
{
	PaError	err;
	if (running)
		return false;
	while (mode == 0)
	{
		system("cls");
		std::cout << "Welcome to RealTimeFX\t\t\tBy: Group 3" << std::endl;
		std::cout << "1 - Microphone input" << std::endl;
		std::cout << "2 - Wav Input" << std::endl;

		
		std::cin >> mode;
		if (!std::cin.good())
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			mode = 0;
			continue;
		}
	}
	if (mode == 2)
	{
		SNDFILE *sf;
		SF_INFO info;
		int num, num_items;
		int *buf;

		/* Open the WAV file. */
		info.format = 0;
		sf = sf_open("./Wavs/test.wav", SFM_READ, &info);
		if (sf == NULL)
		{
			printf("Failed to open the file.\n");
			exit(-1);
		}
		/* Print some of the info, and figure out how much data to read. */
		sampleRate = info.samplerate;
		num_items = info.frames * info.channels;
		inChannels = info.channels;
		/* Allocate space for the data to be read, then read it. */
		wav= std::vector<float>(buffSize*ceil((float)num_items/buffSize));
		num = sf_read_float(sf, wav.data(), num_items);
		sf_close(sf);
	
		offset = 0;
	}
	for (auto& e : effects)
		e->setSampleRate(sampleRate);
	/* initialise portaudio subsytem */
	err = Pa_Initialize();
	if (err != paNoError) { error=err; return false; }

	inputParameters.device = Pa_GetDefaultInputDevice();	/* default input device */
	if (inputParameters.device == paNoDevice) {
		error= "Error: No input default device.";
		if (err != paNoError) { Pa_Terminate();  return false; }
	}
	inputParameters.channelCount = inChannels;						/* mono input */
	inputParameters.sampleFormat = paFloat32;				/* 32 bit floating point input */
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = Pa_GetDefaultOutputDevice();	/* default output device */
	if (outputParameters.device == paNoDevice) {
		error= "Error: No default output device.";
		if (err != paNoError) { Pa_Terminate(); return false; }
	}
	outputParameters.channelCount = outChannels;				/* stereo output */
	outputParameters.sampleFormat = paFloat32;		/* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(&stream,
		&inputParameters,
		&outputParameters,
		sampleRate,	/* Samplerate in Hertz. */
		buffSize,			/* Samples per window */
		paClipOff,				/* We won't output out of range samples so don't bother clipping them. */
		callback,	
		(void*)this);		//rdata
	if (err != paNoError) { Pa_Terminate(); error = err; return false; }

	err = Pa_StartStream(stream);
	if (err != paNoError) { Pa_Terminate(); error = err; return false; }
	running = true;
	return true;
}

bool RealTimeEffects::run()
{
	system("cls");
	std::cout << "Starting RealTimeAudio" << std::endl;
	int currEffect = 0;
	while (currEffect != effects.size())
	{
		system("cls");
		std::cout << "The available effects are: " << std::endl;
		for (unsigned i = 0; i < effects.size(); i++)
			std::cout << i << " - " << effects[i]->getName() << std::endl;
		std::cout << effects.size() << " - Exit" << std::endl;
		std::cin >> currEffect;
		if (!std::cin.good())
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			currEffect = 0;
			continue;
		}
		if (currEffect >= 0 && currEffect < effects.size())
		{
			setEffect(currEffect);
			std::vector<Properties> p = effects[currEffect]->getProps();
			int prop = 0;
			double val;
			while (prop < p.size())
			{
				system("cls");
				std::cout << "Now on effect: " << effects[currEffect]->getName() << std::endl;
				for (unsigned j = 0; j < p.size(); j++)
				{
					std::vector<double> lim = p[j].getLimits();
					std::cout << j << " - " << p[j].getName() << "\t\t\t[" << lim[0] << " , " << lim[1] << "]\t\t\t\t" << p[j].getValue() << std::endl;
				}
				std::cout << p.size() << " - Back" << std::endl;
				std::cout << "Enter the prop and value you want to modify. Ex: 1  0.4" << std::endl;
				std::cin >> prop;
				if (!std::cin.good())
				{
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
					prop = 0;
					continue;
				}
					
				if (prop >= 0 && prop < p.size())
				{
					std::cin >> val;
					if (!std::cin.good())
					{
						std::cin.clear();
						std::cin.ignore(INT_MAX, '\n');
						val = 0;
						continue;
					}

					if (!effects[currEffect]->setProp(prop, val))
					{
						std::vector<double> lim = p[currEffect].getLimits();
						char dummy;
						std::cout << "Values must be in range: [" << lim[0] << " , " << lim[1] << "]" << std::endl;
						std::cout << "Press enter to continue" << std::endl;
						std::cin >> dummy;
					}
					else
						p = effects[currEffect]->getProps();
				}
			}



		}
	}
	return false;
}

bool RealTimeEffects::stop()
{
	PaError	err;
	if (!running)
		return false;
	err = Pa_AbortStream(stream);
	if (err != paNoError) { Pa_Terminate(); error=err; return false; }

	/* sleep until playback has finished */
	while ((err = Pa_IsStreamActive(stream)) == 1) Pa_Sleep(1000);
	if (err < 0) { Pa_Terminate(); error=err; return false; }

	err = Pa_CloseStream(stream);
	if (err != paNoError) { Pa_Terminate(); error=err; return 0; }
	running = false;
	return true;
}


void RealTimeEffects::next(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer)
{
	if (effects.size() > 0)
		effects[currEff]->next(inputBuffer, outputBuffer, framesPerBuffer);
	else
		memcpy(outputBuffer, inputBuffer, sizeof(float)*framesPerBuffer);
}

bool RealTimeEffects::setEffect(unsigned i)
{
	if (i < effects.size())
	{
		currEff = i;
		return true;
	}
	return false;
}

std::vector<Effect*> RealTimeEffects::getEffects()
{
	return effects;
}

std::vector<Properties> RealTimeEffects::getEffectProps(unsigned i)
{
	if (i < effects.size())
		return effects[i]->getProps();
	return std::vector<Properties>();
}

bool RealTimeEffects::setEffectProp(unsigned i, double val)
{
	if (i < effects.size())
		return effects[i]->setProp(i, val);
	return false;
}

std::string RealTimeEffects::getError()
{
	return error;
}

RealTimeEffects::~RealTimeEffects()
{
	stop();
}
