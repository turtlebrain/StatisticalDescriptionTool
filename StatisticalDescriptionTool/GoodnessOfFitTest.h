#pragma once
#include "DiscreteProcess.h"
#include "ProbabilisticDistributions.h"

class CGoodnessOfFitTest
{
public:
	CGoodnessOfFitTest(CDiscreteRandomVariable* ObservedDist, CContinuousDistribution* AssumedDist);
	~CGoodnessOfFitTest();

public:
	void CalculateChiSquaredStatistic(CDiscreteRandomVariable* ObservedDist, CContinuousDistribution* AssumedDist);
	void InitializeChiSquaredCriticalTable();
	double GetChiSquaredCritical(double alpha, int dof);
	int GetIndexFromAlpha(double alpha);
	bool GetH0() { return m_H0; }
	bool GetH1() { return m_H1; }

protected:
	bool m_H0;									//Null Hypothesis - Assumed Distribution is correct
	bool m_H1;									//Alternative Hypothesis - Assumed Distribution is not correct
	double m_alpha;								//significance level
	double m_chi2_stat;							//Chi-squared statistic
	double m_chi2_crit;							//Chi-squared critical
	int m_dof;									//Degrees of Freedom
	vector<vector<double>> m_Chi2CriticalTable;	//Chi-squared Critical Table

};

