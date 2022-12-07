#include "RandomDrawingMachine.h"
#include <stdlib.h> 
#include <time.h>

int RandomDrawingMachine::DrawOneNumber(CDiscreteRandomVariable* random_variable) 
{
    /* initialize random seed: */
    srand(time(NULL) + rand());

    /* generate a random number between 0 and 1 */
    double random_num = (double) rand() / RAND_MAX;

    // Binary Search to find value drawn from CDF
    vector<double> cdf = random_variable->GetCumulativeProbabilityVector();
    int value_drawn(0);
    int lower(0), upper(cdf.size());
    int middle = (lower + upper) / 2;
    int iteration(0), max_iteration(100);
    while (upper > lower && iteration < max_iteration) {

        if ((upper - lower) == 1 && cdf[upper] >= random_num && cdf[lower] <= random_num) {
            double slope = (cdf[upper] - cdf[lower]);
            value_drawn = (1/slope)*(random_num - cdf[lower]) + lower; 
            break;
        }

        if (cdf[middle] >= random_num) {
            upper = middle;
            middle = (lower + upper) / 2;
        }
        else {
            lower = middle;
            middle = (lower + upper) / 2;
        }
        iteration++;
    }

    if (value_drawn == 0) value_drawn++;

    return value_drawn;
}

vector<int> RandomDrawingMachine::DrawEightNumbers(const vector<CDiscreteRandomVariable*> &random_vars)
{
    vector<int> values_drawn(random_vars.size());

    for (int i = 0; i < values_drawn.size(); i++) {
        if (i == 0) {
            values_drawn[i] = DrawOneNumber(random_vars[i]);
        }
        else {
            int j = i;
            while (j > 0) {
                j--;
                random_vars[i]->UpdateRandomVariable(values_drawn[j]);
            }
            values_drawn[i] = DrawOneNumber(random_vars[i]);
            
        }

    }

    return values_drawn;
}