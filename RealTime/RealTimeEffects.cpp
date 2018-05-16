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
		//Si termine el wav cambio el offset a 0
		if (ef->offset >= ef->wav.size())
			ef->offset = 0;
		//Envió los datos al efecto
		ef->next(ef->wav.data() + ef->offset, outputBuffer, framesPerBuffer);
		//Aumento en la cantidad de datos
		ef->offset += framesPerBuffer;
	}
	else
		//Envió los datos al efecto
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
	//Si ya estoy inicializado no me inicializo
	if (running)
		return false;
	while (mode == 0)
	{
		//Selección de modo
		system("cls");
		std::cout << "Welcome to RealTimeFX\t\t\tBy: Group 3" << std::endl;
		std::cout << "1 - Microphone input" << std::endl;
		std::cout << "2 - Wav Input" << std::endl;

		
		std::cin >> mode;
		//Si el input no fue bueno reinicio y empiezo de nuevo
		if (!std::cin.good())
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			mode = 0;
			continue;
		}
	}
	//Si el modo es wav
	if (mode == 2)
	{
		SNDFILE *sf;
		SF_INFO info;
		int num, num_items;
		int *buf;

		//Abro el wav file
		info.format = 0;
		std::string path;
		while (path.size() == 0) path = getUserPath();
		sf = sf_open(path.data(), SFM_READ, &info);
		if (sf == NULL)
		{
			printf("Failed to open the file.\n");
			exit(-1);
		}
		//Obtengo la información
		sampleRate = info.samplerate;
		num_items = info.frames * info.channels;
		inChannels = info.channels;
		//Asigno el espacio necesario
		wav= std::vector<float>(buffSize*ceil((double)num_items/buffSize),0);
		num = sf_read_float(sf, wav.data(), num_items);
		sf_close(sf);
		//Si son dos canales de input los mergeo en uno
		if (inChannels == 2)
		{
			for (unsigned i = 0; i < (info.frames); i++)
				wav[i] = (wav[2*i]+wav[2*i+1])/2;
			wav.resize(buffSize*ceil((double)info.frames / buffSize));
			for (unsigned i = info.frames; i < wav.size(); i++)
				wav[i] = 0;
			inChannels = 1;
		}
		offset = 0;
	}
	//Le asigno el sample rate a los efectos
	for (auto& e : effects)
		e->setSampleRate(sampleRate);
	//Inicializo  port audio
	err = Pa_Initialize();
	if (err != paNoError) { error=err; return false; }

	//Obtengo el input default
	inputParameters.device = Pa_GetDefaultInputDevice();
	if (inputParameters.device == paNoDevice) {
		error= "Error: No input default device.";
		if (err != paNoError) { Pa_Terminate();  return false; }
	}
	//Asigno en mono el input
	inputParameters.channelCount = inChannels;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	//Obtengo el output default
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if (outputParameters.device == paNoDevice) {
		error= "Error: No default output device.";
		if (err != paNoError) { Pa_Terminate(); return false; }
	}
	//Salida stereo
	outputParameters.channelCount = outChannels;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(&stream,
		&inputParameters,
		&outputParameters,
		sampleRate,	
		buffSize,
		paClipOff,
		callback,	
		(void*)this);
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
	//Main loop
	while (currEffect != effects.size())
	{
		//Muestro los efectos
		system("cls");
		std::cout << "The available effects are: " << std::endl;
		for (unsigned i = 0; i < effects.size(); i++)
			std::cout << i << " - " << effects[i]->getName() << std::endl;
		std::cout << effects.size() << " - Exit" << std::endl;
		std::cin >> currEffect;
		//Si el input no fue bueno reseteo 
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
			//Obtengo las propiedades
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
						std::vector<double> lim = p[prop].getLimits();
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

std::string RealTimeEffects::getUserPath()
{
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Wav Files\0*.wav\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a Wav File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (!GetOpenFileNameA(&ofn))
	{
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default:
			std::cout << "You cancelled.\n";
			return std::string();
		}
	}
	return std::string(filename);
}
