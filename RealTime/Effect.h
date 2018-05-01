#pragma once
#include <vector>
#include <string>
#include "Properties.h"
class Effect
{
public:
	Effect(std::string s);
	virtual bool next(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer);
	std::string getName();
    std::vector<Properties> getProps();
	bool setProp(unsigned i, double v);
	~Effect();
protected:
	std::vector<Properties> props;
	std::string name;
};

