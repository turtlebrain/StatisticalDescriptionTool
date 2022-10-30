#include "GoodnessOfFitTest.h"
#include "FileReader.h"
#include <assert.h>

CGoodnessOfFitTest::CGoodnessOfFitTest(CDiscreteRandomVariable* ObservedDist, CContinuousDistribution* AssumedDist)
{
	int ObservedDistSize = ObservedDist->GetFrequencyVector().size();
	int AssumedDistSize = AssumedDist->GetDistribution().size();
	assert(ObservedDistSize == AssumedDistSize);
	m_dof = ObservedDistSize - 1;
	m_alpha = 0.05;
	CalculateChiSquaredStatistic(ObservedDist, AssumedDist);
	InitializeChiSquaredCriticalTable();
	m_chi2_crit = GetChiSquaredCritical(m_alpha, m_dof);
	if (m_chi2_crit < m_chi2_stat) {
		m_H0 = false;
		m_H1 = true;
	}
	else {
		m_H0 = true;
		m_H1 = false;
	}
}

CGoodnessOfFitTest::~CGoodnessOfFitTest()
{

}

void CGoodnessOfFitTest::InitializeChiSquaredCriticalTable()
{
	CFileReader Chi2CriticalTableReader;
	Chi2CriticalTableReader.ReadTableOfDoubles("Chi2Critical.csv");
	m_Chi2CriticalTable = Chi2CriticalTableReader.GetDoublesDataSet();
}

int CGoodnessOfFitTest::GetIndexFromAlpha(double alpha)
{
	int index = 0;
	if (alpha == 0.995) index = 1;
	else if (alpha == 0.99) index = 2;
	else if (alpha == 0.975) index = 3;
	else if (alpha == 0.95) index = 4;
	else if (alpha == 0.9) index = 5;
	else if (alpha == 0.1) index = 6;
	else if (alpha == 0.05) index = 7;
	else if (alpha == 0.025) index = 8;
	else if (alpha == 0.01) index = 9;
	else if (alpha == 0.005) index = 10;
	else throw invalid_argument("alpha index not found");
	return index;
}

double CGoodnessOfFitTest::GetChiSquaredCritical(double alpha, int dof)
{
	int Chi2CriticalTableSize = m_Chi2CriticalTable.size();
	double target(0.);
	double y_l(0.), y_u(0.), x_l(0.), x_u(0.);
	int alpha_index(-1);

	assert(Chi2CriticalTableSize > 0);
	try
	{
		alpha_index = GetIndexFromAlpha(alpha);
	}
	catch (invalid_argument& e)
	{
		cerr << e.what() << endl;
		return DBL_MAX;
	}
	for (int i = 0; i < Chi2CriticalTableSize; i++) {
		if (m_Chi2CriticalTable[i][0] < dof) {
			continue;
		}
		if (m_Chi2CriticalTable[i][0] == dof) {
			target = m_Chi2CriticalTable[i][alpha_index];
			break;
		}
		else {
			assert(i > 1);
			int i_minus1 = i - 1;
			y_l = m_Chi2CriticalTable[i_minus1][alpha_index];
			y_u = m_Chi2CriticalTable[i][alpha_index];
			x_l = m_Chi2CriticalTable[i_minus1][0];
			x_u = m_Chi2CriticalTable[i][0];
			target = y_u - (x_u - dof) * ((y_u - y_l) / (x_u - x_l));
			break;
		}
	}

	return target;
}

void CGoodnessOfFitTest::CalculateChiSquaredStatistic(CDiscreteRandomVariable* ObservedDist, CContinuousDistribution* AssumedDist)
{
	double ChiSquaredStat(0.);
	for (int i = 0; i < ObservedDist->GetFrequencyVector().size(); i++) {
		ChiSquaredStat += (pow(ObservedDist->GetFrequencyVector()[i] - AssumedDist->GetDistribution()[i], 2)) / 
			AssumedDist->GetDistribution()[i];
	}
	m_chi2_stat = ChiSquaredStat;
}