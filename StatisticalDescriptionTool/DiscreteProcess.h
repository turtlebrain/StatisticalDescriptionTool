#pragma once
#include <iostream>
#include <vector>

using namespace std;

const double TOL = 1e-4;
const int MAX_SIZE = 50;
const int NUMBER_OF_DRAWS = 8;
const vector<int> max_numbers{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
								11, 12, 13, 14, 15, 16, 17,	18, 19, 20,
								21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
								31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
								41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };



class CDiscreteRandomVariable
{
public:
	CDiscreteRandomVariable();	
	CDiscreteRandomVariable(vector<int>& RandomSample);
	~CDiscreteRandomVariable();

public:
	void InitializeRandomVariable();
	void UpdateRandomVariable(int DiscardedValue);
	void GenerateFrequencyVector();
	void GenerateProbabilityVector();
	void GenerateCumulativeProbabilityVector();
	void CalculateMean();
	void CalculateVariance();
	void CalculateStandardDeviation();
	void CalculateMode();
	vector<int> GetFrequencyVector() { return m_FrequencyVector; }
	vector<double> GetProbabilityVector() { return m_ProbabilityVector; }
	vector<double> GetCumulativeProbabilityVector() { return m_CumulativeProbabilityVector; }
	double GetProbabilityValueIsEqualTo(int value);
	double GetProbabilityValueIsLessThan(int value);
	double GetProbabilityValueIsGreaterThan(int value);
	int GetMode() { return m_Mode; }
	double GetMean() { return m_Mean; }
	double GetVariance() { return m_Variance; }
	double GetStdVariation() { return m_StdDeviation; }
	int GetSampleSize() { return m_SampleSize; }

	// Utility functions
	void AppendRandomSample(vector<int> &RandomSampleToAppend);

protected:
	vector<int> m_RandomSample;
	vector<int> m_FrequencyVector;
	vector<double> m_ProbabilityVector;
	vector<double> m_CumulativeProbabilityVector;
	int m_SampleSize;
	int m_Mode;
	double m_Mean;
	double m_Variance;
	double m_StdDeviation;

};

class CDiscreteProcess : CDiscreteRandomVariable
{
public:
	CDiscreteProcess();
	~CDiscreteProcess();
};


