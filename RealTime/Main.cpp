#include "RealTimeEffects.h"
#include "EffectDelay.h"
#include "EffectRobot.h"
#include "EffectVibrato.h"
#include "EffectWhisper.h"
#include "EffectDistortion.h"
#include "EffectReverbLP.h"
#include "EffectPhaser.h"
#include "Effect3D.h"
#include "EffectPlain.h"
#include "EffectReverbFull.h"
#include "EffectChorus.h"
#include "EffectFlanger.h"
#include <iostream>
using namespace std;



int main(void)
{
	//Creación de los efectos
	EffectPlain e1;
	EffectDelay e2;
	EffectReverbLP e3;
	EffectReverbFull e4;
	EffectVibrato e5;
	EffectFlanger e6;
	EffectChorus e7;
	EffectPhaser e8;
	EffectDistortion e9;
	Effect3D e10;
	EffectRobot e11;
	EffectWhisper e12;
	//Se asignan los efectos a la ClaseRealTime effects
	vector<Effect*> effs = { &e1,&e2,&e3,&e4,&e5,&e6,&e7,&e8,&e9,&e10,&e11,&e12 };
	RealTimeEffects r = RealTimeEffects(effs,DEFAULT_SAMPLE_RATE);
	//Se inicializa la Función
	if (r.start() == true)
	{
		//Se corre el main loop de la ClaseRealTime effects 
		while (r.run() == true);
		//Se descinicializa la clase
		if (r.stop() == false)
			cout << r.getError() << endl;
		else
			cout << "Stoped RealTimeAudio" << endl;
	}
	else
		cout << r.getError() << endl;
	return 0;
}