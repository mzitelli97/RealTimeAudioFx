#include "RealTimeEffects.h"
#include "EffectDelay.h"
#include "EffectRobot.h"
#include "EffectVibrato.h"
#include "EffectWhisper.h"
#include "EffectDistortionHC.h"
#include "EffectReverbLP.h"
#include "EffectPhaser.h"
#include "Effect3D.h"
#include "EffectPlain.h"
#include <iostream>
using namespace std;



int main(void)
{
	EffectPlain e1;
	EffectDelay e2;
	EffectReverbLP e3;
	EffectVibrato e4;
	EffectPhaser e5;
	EffectDistortionHC e6;
	Effect3D e7;
	EffectRobot e8;
	EffectWhisper e9;
	vector<Effect*> effs = { &e1,&e2,&e3,&e4,&e5,&e6,&e7,&e8,&e9 };
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