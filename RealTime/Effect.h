#pragma once
#include <vector>
#include <string>
#include "Properties.h"
class Effect
{
public:
	Effect(std::string s);
	virtual bool next(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer);
	virtual std::string getName();
    virtual std::vector<Properties> getProps();
	virtual bool setProp(unsigned i, double v);
	~Effect();
protected:
	std::vector<Properties> props;
	std::string name;
};

