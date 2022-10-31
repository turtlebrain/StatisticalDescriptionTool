#include "DiscreteProcess.h"
#include <assert.h>
#include <math.h>

CDiscreteRandomVariable::CDiscreteRandomVariable(vector<int> RandomSample)
{
	m_RandomSample = RandomSample;
	InitializeRandomVariable();
}

CDiscreteRandomVariable::CDiscreteRandomVariable()
{
	//Default constructor
	m_RandomSample = { 0 };
	InitializeRandomVariable();
}

CDiscreteRandomVariable::~CDiscreteRandomVariable()
{

}

void CDiscreteRandomVariable::InitializeRandomVariable()
{
	m_SampleSize = m_RandomSample.size();
	m_FrequencyVector.resize(MAX_SIZE);
	m_ProbabilityVector.resize(MAX_SIZE);
	m_CumulativeProbabilityVector.resize(MAX_SIZE);
	GenerateFrequencyVector();
	GenerateProbabilityVector();
	GenerateCumulativeProbabilityVector();
	CalculateMean();
	CalculateVariance();
	CalculateStandardDeviation();
	CalculateMode();
}

void CDiscreteRandomVariable::UpdateRandomVariable(int DiscardedValue)
{
	// Reset Probability Vectors
	m_FrequencyVector.clear();
	m_ProbabilityVector.clear();
	m_CumulativeProbabilityVector.clear();

	// Search and delete DiscardedValue
	for (int i = 0; i < m_RandomSample.size(); i++) {
		if (m_RandomSample[i] == DiscardedValue) {
			m_RandomSample.erase(m_RandomSample.begin()+i);
			i--;
		}
	}

	// Re-initialize Random Variable
	InitializeRandomVariable();
}

void CDiscreteRandomVariable::GenerateFrequencyVector()
{
	assert(m_RandomSample.size() > 0);
	for (int i = 0; i < m_RandomSample.size(); i++) {
		int zero_index = m_RandomSample[i] - 1;
		m_FrequencyVector[zero_index]++;
	}
}

void CDiscreteRandomVariable::GenerateProbabilityVector()
{
	assert(m_ProbabilityVector.size() == m_FrequencyVector.size());
	double probability_sum(0.);
	
	for (int i = 0; i < m_ProbabilityVector.size(); i++) {
		m_ProbabilityVector[i] = (double)m_FrequencyVector[i] / m_SampleSize;
		probability_sum += m_ProbabilityVector[i];
	}
	assert(probability_sum <= 1.+ TOL);
}

void CDiscreteRandomVariable::GenerateCumulativeProbabilityVector()
{
	assert(m_CumulativeProbabilityVector.size() == m_FrequencyVector.size());
	double cumul_frequency(0.);

	for (int i = 0; i < m_CumulativeProbabilityVector.size(); i++) {
		cumul_frequency += (double)m_FrequencyVector[i];
		m_CumulativeProbabilityVector[i] = cumul_frequency / m_SampleSize;
	}
	assert(m_CumulativeProbabilityVector.back() <= 1. + TOL);
}

inline double CDiscreteRandomVariable::GetProbabilityValueIsEqualTo(int value)
{
	assert(value >= 1);
	int zero_index = value - 1;
	return m_ProbabilityVector[zero_index];
}

inline double CDiscreteRandomVariable::GetProbabilityValueIsLessThan(int value)
{
	assert(value >= 1);
	int zero_index = value - 1;
	return m_CumulativeProbabilityVector[zero_index];
}

inline double CDiscreteRandomVariable::GetProbabilityValueIsGreaterThan(int value)
{
	assert(value >= 1);
	int zero_index = value - 1;
	return 1.0 - m_CumulativeProbabilityVector[zero_index];
}

void CDiscreteRandomVariable::CalculateMean()
{
	double mean(0.);
	for (int i = 0; i < m_ProbabilityVector.size(); i++) {
		mean += max_numbers[i] * m_ProbabilityVector[i];
	}

	m_Mean = mean;
	assert(m_Mean <= MAX_SIZE);
}

void CDiscreteRandomVariable::CalculateVariance()
{
	double variance(0.);
	double mean = GetMean();
	if (mean == DBL_MAX) CalculateMean();
	double meanSquare = pow(mean, 2.);
	for (int i = 0; i < MAX_SIZE; i++) {
		variance += (pow(max_numbers[i], 2.)) * m_ProbabilityVector[i];
	}
	variance -= meanSquare;

	m_Variance = variance;
}

inline void CDiscreteRandomVariable::CalculateStandardDeviation()
{
	double stdDev(0.);
	double variance = GetVariance();
	if (variance == DBL_MAX) CalculateVariance();
	stdDev = sqrt(variance);

	m_StdDeviation = stdDev;
}

void CDiscreteRandomVariable::AppendRandomSample(vector<int> RandomSampleToAppend)
{
	m_RandomSample.insert(m_RandomSample.end(), RandomSampleToAppend.begin(), RandomSampleToAppend.end());
	InitializeRandomVariable();
}

void CDiscreteRandomVariable::CalculateMode()
{
	int highest_frequency(0);
	for (int i = 0; i < MAX_SIZE; i++) {
		if (m_FrequencyVector[i] > highest_frequency) {
			highest_frequency = m_FrequencyVector[i];
			m_Mode = i+1;
		}
	}
}

CDiscreteProcess::CDiscreteProcess()
{

}

CDiscreteProcess::~CDiscreteProcess()
{

}