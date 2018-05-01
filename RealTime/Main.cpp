#include "RealTimeEffects.h"
#include "EffectDelay.h"
#include "EffectRobot.h"
#include "EffectVibrato.h"
#include <iostream>
using namespace std;



int main(void)
{
	EffectDelay e3;
	EffectRobot e2;
	EffectVibrato e1;
	vector<Effect*> effs = { &e1,&e2,&e3 };
	RealTimeEffects r = RealTimeEffects(effs);
	if (r.start() == true)
	{
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