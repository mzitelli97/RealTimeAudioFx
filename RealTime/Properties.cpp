#include "Properties.h"


Properties::Properties(std::string & n, double lL, double rL)
{
	name = n;
	limits = std::vector<double>(2);
	limits[0] = lL;
	limits[1] = rL;
}

bool Properties::setValue(double v)
{
	if (v >= limits[0] && v <= limits[1])
	{
		val = v;
		return true;
	}
	return false;
}

double Properties::getValue()
{
	return val;
}

std::string Properties::getName()
{
	return name;
}

Properties::~Properties()
{
}
