#pragma once
#include <vector>

using namespace std;

enum class eDistributionType { NORMAL, GAMMA };

class CContinuousDistribution
{
public:
	CContinuousDistribution() {};
	~CContinuousDistribution() {};

public:
	virtual void GenerateProbabilityDensityFunction() = 0;
	virtual void GenerateDistribution(double SampleSize) = 0;
	virtual vector<pair<double, double>> GetPDF() { return m_PDF; }
	virtual vector<double> GetDistribution() { return m_Distribution; }
	
protected:
	vector<pair<double, double>> m_PDF;
	vector<double> m_Distribution;
};

class CNormalDistribution : public CContinuousDistribution
{
public:
	CNormalDistribution(vector<double>& possibleOutcomes, double mean, double stdDev);
	CNormalDistribution(double mean, double stdDev);
	~CNormalDistribution();

public:
	void GenerateProbabilityDensityFunction() override;
	void GenerateDistribution(double SampleSize) override;
	vector<pair<double, double>> GetPDF() { return m_PDF; }
	vector<double> GetDistribution() { return m_Distribution; }
	

protected:
	double m_mean;
	double m_stdDev;
	vector<double> m_possibleOutcomes;
	vector<pair<double, double>> m_PDF;
	vector<double> m_Distribution;

};

class CGammaDistribution : public CContinuousDistribution
{
public:
	CGammaDistribution(vector<double>& possibleOutcomes, double mean, double stdDev);
	~CGammaDistribution();

public:
	void GenerateProbabilityDensityFunction() override;
	void GenerateDistribution(double SampleSize) override;
	double Gamma(double alpha);
	vector<pair<double, double>> GetPDF() { return m_PDF; }
	vector<double> GetDistribution() { return m_Distribution; }

protected:
	double m_alpha;							//Shape parameter
	double m_beta;							//Rate parameter
	double m_mean;
	double m_stdDev;
	vector<double> m_possibleOutcomes;
	vector<pair<double, double>> m_PDF;
	vector<double> m_Distribution;
};

