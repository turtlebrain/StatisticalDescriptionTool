// StatisticalDescriptionTool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "DiscreteProcess.h"
#include "FileReader.h"
#include "pbPlots.h"
#include "supportLib.h"
#include "ProbabilisticDistributions.h"
#include "GoodnessOfFitTest.h"
#include "RandomDrawingMachine.h"

void PrintVector(const vector<int>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

void PrintVector(const vector<double>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

void PrintHypothesisTestingResults(const vector<CGoodnessOfFitTest*> &NormalDistributionHypothesisTests, const vector<CGoodnessOfFitTest*> &GammaDistributionHypothesisTests) {
    if (NormalDistributionHypothesisTests.size() == GammaDistributionHypothesisTests.size()) {
        for (int j = 0; j < NormalDistributionHypothesisTests.size(); j++) {
            cout << "Distribution of Draw #" << j << " is: ";
            if (NormalDistributionHypothesisTests[j]->GetH0()) cout << "NORMAL";
            else if (GammaDistributionHypothesisTests[j]->GetH0()) cout << "GAMMA";
            else cout << "NEITHER NORMAL NOR GAMMA";
            if (NormalDistributionHypothesisTests[j]->GetH0() && GammaDistributionHypothesisTests[j]->GetH0())
                cout << " AND GAMMA";
            cout << endl;
        }
    }
}


vector<int> GetVectorOfSuccessiveCounts(const vector<vector<int>>& sample) {
    vector<int> successiveCount(MAX_SIZE-1);
    for (int i = 0; i < sample[0].size(); i++) {
        for (int j = 1; j < sample.size(); j++) {
            int j_minus1 = j - 1;
            if (sample[j_minus1][i] + 1 == sample[j][i]) {
                int smpl_val_minus1 = sample[j_minus1][i] - 1;
                successiveCount[smpl_val_minus1]++;
            }
        }
    }

    return successiveCount;
}


int main()
{
    vector<double> d_max_numbers{ 1., 2., 3., 4., 5., 6., 7., 8., 9., 10.,
                            11., 12., 13., 14., 15., 16., 17.,	18., 19., 20.,
                            21., 22., 23., 24., 25., 26., 27., 28., 29., 30.,
                            31., 32., 33., 34., 35., 36., 37., 38., 39., 40.,
                            41., 42., 43., 44., 45., 46., 47., 48., 49., 50. };

    CFileReader LottoFileReader;

    // 2021
    string lottoFileName("2021Results-PP.csv");
    LottoFileReader.ReadTableOfIntegers(lottoFileName);
    vector<vector<int>> cumulYearsResults = LottoFileReader.GetIntegersDataSet();
    // 2022
    string l2022FileName("2022Results-PP.csv");
    LottoFileReader.ReadTableOfIntegers(l2022FileName);
    vector<vector<int>> y2022Results = LottoFileReader.GetIntegersDataSet();
    // 2021 - 2022
    cumulYearsResults.insert(cumulYearsResults.end(), y2022Results.begin(), y2022Results.end());

    vector<vector<int>> SampleWinningNumbers(NUMBER_OF_DRAWS);
    vector<CDiscreteRandomVariable*> RandomVariableWinningNumbers(NUMBER_OF_DRAWS);
    for (int i = 0; i < cumulYearsResults.size(); i++) {
        for (int j = 0; j < NUMBER_OF_DRAWS; j++) {
            SampleWinningNumbers[j].push_back(cumulYearsResults[i][j]);
        }
    }
    for (int i = 0; i < NUMBER_OF_DRAWS; i++) {
        RandomVariableWinningNumbers[i] = new CDiscreteRandomVariable(SampleWinningNumbers[i]);
    }

    // Plots
    bool pdf_success(false), cdf_success(false);
    StringReference* errorMessage = new StringReference();
    vector<RGBABitmapImageReference*> PDFplots(NUMBER_OF_DRAWS);
    vector<RGBABitmapImageReference*> CDFplots(NUMBER_OF_DRAWS);
#pragma omp parallel for
    for (int j = 0; j < NUMBER_OF_DRAWS; j++) {
        PDFplots[j] = CreateRGBABitmapImageReference();
        CDFplots[j] = CreateRGBABitmapImageReference();
        pdf_success = DrawScatterPlot(PDFplots[j], 400, 600, &d_max_numbers, &(RandomVariableWinningNumbers[j]->GetProbabilityVector()), errorMessage);
        cdf_success = DrawScatterPlot(CDFplots[j], 400, 600, &d_max_numbers, &(RandomVariableWinningNumbers[j]->GetCumulativeProbabilityVector()), errorMessage);
        if (pdf_success) {
            vector<double>* PDFpngdata = ConvertToPNG(PDFplots[j]->image);
            string pngName = "PDF_" + to_string(j+1) + ".png";
            WriteToFile(PDFpngdata, pngName);
            DeleteImage(PDFplots[j]->image);
        }
        else {
            cerr << "Error: ";
            for (wchar_t c : *errorMessage->string) {
                wcerr << c;
            }
            cerr << endl;
        }

        if (cdf_success) {
            vector<double>* CDFpngdata = ConvertToPNG(CDFplots[j]->image);
            string pngName = "CDF_" + to_string(j + 1) + ".png";
            WriteToFile(CDFpngdata, pngName);
            DeleteImage(CDFplots[j]->image);
        }
        else {
            cerr << "Error: ";
            for (wchar_t c : *errorMessage->string) {
                wcerr << c;
            }
            cerr << endl;
        }
    }

    // Output Statistical Moments
    vector<double> MeanWinningNumbers, VarWinningNumbers, StdDevWinningNumbers, ModeWinningNumbers;
    vector<int> intMeanWinningNumbers;
    for (int i = 0; i < RandomVariableWinningNumbers.size(); i++) {
        MeanWinningNumbers.push_back(RandomVariableWinningNumbers[i]->GetMean());
        VarWinningNumbers.push_back(RandomVariableWinningNumbers[i]->GetVariance());
        StdDevWinningNumbers.push_back(RandomVariableWinningNumbers[i]->GetStdVariation());
        ModeWinningNumbers.push_back(RandomVariableWinningNumbers[i]->GetMode());
        intMeanWinningNumbers.push_back((int)RandomVariableWinningNumbers[i]->GetMean());
    }

    cout << "Mean of Winning Numbers: ";
    PrintVector(MeanWinningNumbers);
    cout << "Variance of Winning Numbers: ";
    PrintVector(VarWinningNumbers);
    cout << "Standard Deviation of Winning Numbers: ";
    PrintVector(StdDevWinningNumbers);
    cout << "Mode of Winning Numbers: ";
    PrintVector(ModeWinningNumbers);
    cout << "int Mean of Winning Number: ";
    PrintVector(intMeanWinningNumbers);


    // Hypothesis Testing
    // H0 : Random Variable is Assumed Distribution
    // H1 : Random Variable is NOT Assumed Distributed
    vector<CNormalDistribution*> NormallyDistributedWinningNumbers(NUMBER_OF_DRAWS);
    vector<CGammaDistribution*> GammaDistributedWinningNumbers(NUMBER_OF_DRAWS);
    vector<CGoodnessOfFitTest*> NormalDistributionHypothesisTests(NUMBER_OF_DRAWS);
    vector<CGoodnessOfFitTest*> GammaDistributionHypothesisTests(NUMBER_OF_DRAWS);
    double WinningNumbersSampleSize(0.);

#pragma omp parallel for
    for (int j = 0; j < NUMBER_OF_DRAWS; j++) {
        NormallyDistributedWinningNumbers[j] = new CNormalDistribution(d_max_numbers, RandomVariableWinningNumbers[j]->GetMean(), RandomVariableWinningNumbers[j]->GetStdVariation());
        GammaDistributedWinningNumbers[j] = new CGammaDistribution(d_max_numbers, RandomVariableWinningNumbers[j]->GetMean(), RandomVariableWinningNumbers[j]->GetStdVariation());
        WinningNumbersSampleSize = (double)RandomVariableWinningNumbers[j]->GetSampleSize();
        NormallyDistributedWinningNumbers[j]->GenerateDistribution(WinningNumbersSampleSize);
        GammaDistributedWinningNumbers[j]->GenerateDistribution(WinningNumbersSampleSize);
        NormalDistributionHypothesisTests[j] = new CGoodnessOfFitTest(RandomVariableWinningNumbers[j], NormallyDistributedWinningNumbers[j]);
        GammaDistributionHypothesisTests[j] = new CGoodnessOfFitTest(RandomVariableWinningNumbers[j], GammaDistributedWinningNumbers[j]);
    }
    PrintHypothesisTestingResults(NormalDistributionHypothesisTests, GammaDistributionHypothesisTests);

    vector<int> successiveTest;
    successiveTest = GetVectorOfSuccessiveCounts(SampleWinningNumbers);
    cout << "Successive Counts" << endl;
    PrintVector(successiveTest);

    // Draw Simulation
    int drw_sample_size(30);
    int num_trials(50);

    for (int trials = 1; trials <= num_trials; trials++) {
        vector<vector<int>> RandomDraws(NUMBER_OF_DRAWS);


        cout << endl;
        cout << "Trial# " << trials << endl;
   
        for (int i = 0; i < drw_sample_size; i++) {
            vector<int> RandomDraw = RandomDrawingMachine::DrawEightNumbers(RandomVariableWinningNumbers);
            for (int j = 0; j < NUMBER_OF_DRAWS; j++) {
                RandomDraws[j].push_back(RandomDraw[j]);
            }
            PrintVector(RandomDraw);
        }

        // Average Draws
        vector<int> meanDraw(NUMBER_OF_DRAWS);
        vector<int> modeDraw(NUMBER_OF_DRAWS);
        vector<CDiscreteRandomVariable*> SampleDraws(NUMBER_OF_DRAWS);
#pragma omp parallel for
        for (int i = 0; i < NUMBER_OF_DRAWS; i++) {
            SampleDraws[i] = new CDiscreteRandomVariable(RandomDraws[i]);
            meanDraw[i] = (int)(ceil(SampleDraws[i]->GetMean()));
            modeDraw[i] = SampleDraws[i]->GetMode();
            delete SampleDraws[i];
        }

        cout << "Mean Draw:" << endl;
        PrintVector(meanDraw);
        cout << "Mode Draw:" << endl;
        PrintVector(modeDraw);

    }


    // Garbage Collection
#pragma omp parallel for
    for (int i = 0; i < NUMBER_OF_DRAWS; i++) {
        delete RandomVariableWinningNumbers[i];
        delete NormallyDistributedWinningNumbers[i];
        delete GammaDistributedWinningNumbers[i];
        delete NormalDistributionHypothesisTests[i];
        delete GammaDistributionHypothesisTests[i];
    }

    return (pdf_success && cdf_success) ? 0 : 1;
}


