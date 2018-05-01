#pragma once
#include <string>
#include <vector>
class Properties
{
public:
	Properties(std::string& n,double lL,double rL);
	bool setValue(double v);
	double getValue();
	std::string getName();
	~Properties();
private:
	double val;
	std::vector<double> limits;
	std::string name;
};

