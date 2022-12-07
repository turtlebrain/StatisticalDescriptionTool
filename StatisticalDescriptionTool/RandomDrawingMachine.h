#pragma once
#include "DiscreteProcess.h"
#include "ProbabilisticDistributions.h"

class RandomDrawingMachine
{
public:
	RandomDrawingMachine() {};
	~RandomDrawingMachine() {};

public:
	static int DrawOneNumber(CDiscreteRandomVariable* random_var);
	static vector<int> DrawEightNumbers(const vector<CDiscreteRandomVariable*>& random_vars);
};

