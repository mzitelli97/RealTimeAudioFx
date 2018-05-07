#include "RealTimeEffects.h"
#include "EffectDelay.h"
#include "EffectRobot.h"
#include "EffectVibrato.h"
#include "EffectWhisper.h"
#include "EffectDistortionHC.h"
#include "EffectReverbLP.h"
#include "Effect3D.h"
#include "EffectPlain.h"
#include <iostream>
using namespace std;



int main(void)
{
	EffectDelay e3;
	EffectRobot e8;
	EffectVibrato e2;
	EffectWhisper e7;
	EffectDistortionHC e5;
	EffectReverbLP e6;
	Effect3D e4;
	EffectPlain e1;
	vector<Effect*> effs = { &e1,&e2,&e3,&e4,&e5,&e6,&e7,&e8 };
	RealTimeEffects r = RealTimeEffects(effs,DEFAULT_SAMPLE_RATE);
	if (r.start() == true)
	{
		while (r.run() == true);
		if (r.stop() == false)
			cout << r.getError() << endl;
		else
			cout << "Stoped RealTimeAudio" << endl;
	}
	else
		cout << r.getError() << endl;
	return 0;
}