#include "RealTimeEffects.h"
#include "EffectDelay.h"
#include "EffectRobot.h"
#include "EffectVibrato.h"
#include <iostream>
using namespace std;



int main(void)
{
	EffectDelay e3;
	EffectRobot e1;
	EffectVibrato e2;
	vector<Effect*> effs = { &e1,&e2,&e3 };
	RealTimeEffects r = RealTimeEffects(effs);
	if (r.start() == true)
	{
		//system(clc);
		cout << "Starting RealTimeAudio" << endl;
		cout << "Press any key to exit" << endl;
		getchar();
		if (r.stop() == false)
			cout << r.getError() << endl;
		else
			cout << "Stoped RealTimeAudio" << endl;
	}
	else
		cout << r.getError() << endl;
	cout << "Press any key to exit" << endl;
	getchar();
	return 0;
}