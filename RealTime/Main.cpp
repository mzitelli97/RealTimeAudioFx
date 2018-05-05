#include "RealTimeEffects.h"
#include "EffectDelay.h"
#include "EffectRobot.h"
#include "EffectVibrato.h"
#include "EffectWhisper.h"
#include "EffectDistortionHC.h"
#include "EffectReverbLP.h"
#include <iostream>
using namespace std;



int main(void)
{
	EffectDelay e3;
	EffectRobot e1;
	EffectVibrato e2;
	EffectWhisper e4;
	EffectDistortionHC e5;
	EffectReverbLP e6;
	vector<Effect*> effs = { &e1,&e2,&e3,&e4,&e5,&e6 };
	RealTimeEffects r = RealTimeEffects(effs);
	if (r.start() == true)
	{
		system("cls");
		cout << "Starting RealTimeAudio" << endl;
		int currEffect=0;
		while (currEffect != effs.size())
		{
			system("cls");
			cout << "The available effects are: " << endl;
			for (unsigned i = 0; i<effs.size(); i++)
				cout << i  << " - " << effs[i]->getName() << endl;
			cout << effs.size()  << " - Exit" << endl;
			cin >> currEffect;
			if (currEffect >= 0 && currEffect<effs.size())
			{
				r.setEffect(currEffect);
				vector<Properties> p = effs[currEffect]->getProps();
				int prop=0;
				double val;
				while (prop < p.size())
				{
					system("cls");
					cout << "Now on effect: " << effs[currEffect]->getName() << endl;
					for (unsigned j = 0; j < p.size(); j++)
					{
						vector<double> lim = p[j].getLimits();
						cout << j << " - " << p[j].getName() << "\t\t\t[" << lim[0] << " , " << lim[1] << "]\t\t\t\t" << p[j].getValue() << endl;
					}
					cout << p.size() << " - Back" << endl;
					cout << "Enter the prop and value you want to modify. Ex: 1  0.4" << endl;
					cin >> prop;
					if (prop >= 0 && prop < p.size())
					{
						cin >> val;

						if (!effs[currEffect]->setProp(prop, val))
						{
							vector<double> lim = p[currEffect].getLimits();
							char dummy;
							cout << "Values must be in range: [" << lim[0] << " , " << lim[1] << endl;
							cout << "Press enter to continue" << endl;
							cin >> dummy;
						}
						else
							p = effs[currEffect]->getProps();
					}
				}
				
				
				
			}
			
		}
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