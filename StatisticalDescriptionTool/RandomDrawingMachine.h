#pragma once
#include "DiscreteProcess.h"
#include "ProbabilisticDistributions.h"

class RandomDrawingMachine
{
public:
	RandomDrawingMachine() {};
	~RandomDrawingMachine() {};

public:
	int DrawOneNumber(CDiscreteRandomVariable* random_var);
	vector<int> DrawEightNumbers(vector<CDiscreteRandomVariable*>& random_vars);
};

