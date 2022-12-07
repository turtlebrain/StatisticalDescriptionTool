#define _USE_MATH_DEFINES

#include "ProbabilisticDistributions.h"
#include <assert.h>
#include <cmath>

CNormalDistribution::CNormalDistribution(vector<double>& possibleOutcomes, double mean, double stdDev)
{
	m_possibleOutcomes = possibleOutcomes;
	m_mean = mean;
	m_stdDev = stdDev;
	GenerateProbabilityDensityFunction();
}

CNormalDistribution::CNormalDistribution(double mean, double stdDev)
{
	int n_Outcomes(100);
	vector<double> possibleOutcomes(n_Outcomes);
	double lower_bound(mean - stdDev * 5);
	double upper_bound(mean - stdDev * 5);
	double inc = (upper_bound - lower_bound) / (double(n_Outcomes) - 1);
	possibleOutcomes[0] = lower_bound;

	for (int i = 1; i < n_Outcomes; i++) {
		possibleOutcomes[i] = possibleOutcomes[i - 1] + inc;
	}

	m_possibleOutcomes = possibleOutcomes;
	m_mean = mean;
	m_stdDev = stdDev;
	GenerateProbabilityDensityFunction();
}

CNormalDistribution::~CNormalDistribution()
{

}

void CNormalDistribution::GenerateProbabilityDensityFunction()
{
	int number_of_possible_outcomes = m_possibleOutcomes.size();
	assert(number_of_possible_outcomes > 0);
	vector<pair<double, double>> pdf(number_of_possible_outcomes);
	for (int i = 0; i < number_of_possible_outcomes; i++) {
		pdf[i].first = m_possibleOutcomes[i];
		pdf[i].second = (1. / sqrt(2 * M_PI * pow(m_stdDev, 2))) * exp(-(1. / 2.) * pow((m_possibleOutcomes[i] - m_mean) / m_stdDev, 2));
	}

	m_PDF = pdf;
}

void CNormalDistribution::GenerateDistribution(double SampleSize)
{
	int n_possibleOutcomes = m_PDF.size();
	assert(n_possibleOutcomes > 0);
	for (int i = 0; i < n_possibleOutcomes; i++) {
		m_Distribution.push_back(SampleSize * m_PDF[i].second);
	}
}

CGammaDistribution::CGammaDistribution(vector<double>& possibleOutcomes, double mean, double stdDev)
{
	m_possibleOutcomes = possibleOutcomes;
	m_mean = mean;
	m_stdDev = stdDev;
	m_alpha = round(pow(mean, 2) / pow(stdDev, 2));
	m_beta = round(mean/pow(stdDev, 2));
	GenerateProbabilityDensityFunction();
}

CGammaDistribution::~CGammaDistribution()
{

}

double CGammaDistribution::Gamma(double alpha)
{
	assert(alpha >= 1);
	if (alpha > 1) {
		return (alpha - 1) * Gamma(alpha - 1);
	}
	else { return 1; }
}

void CGammaDistribution::GenerateProbabilityDensityFunction()
{
	int number_of_possible_outcomes = m_possibleOutcomes.size();
	assert(number_of_possible_outcomes > 0);
	vector<pair<double, double>> pdf(number_of_possible_outcomes);
	for (int i = 0; i < number_of_possible_outcomes; i++) {
		pdf[i].first = m_possibleOutcomes[i];
		pdf[i].second = pow(m_possibleOutcomes[i], m_alpha - 1) * exp(-1 * m_beta * m_possibleOutcomes[i]) * pow(m_beta, m_alpha) / Gamma(m_alpha);
	}

	m_PDF = pdf;
}

void CGammaDistribution::GenerateDistribution(double SampleSize)
{
	int n_possibleOutcomes = m_PDF.size();
	assert(n_possibleOutcomes > 0);
	for (int i = 0; i < n_possibleOutcomes; i++) {
		m_Distribution.push_back(SampleSize * m_PDF[i].second);
	}
}