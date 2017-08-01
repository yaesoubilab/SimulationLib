#include "../include/SimulationLib/Likelihood.h"

using namespace std;
using namespace SimulationLib;

Likelihood::Likelihood(TimeSeries * _TS, Distribuition * _distName) {
    TS = _TS;
    distName = _distName;
}

function<double(double,int)> Likelihood::CreateFunction(void) {
    function<double(double,int)> func = [](double time, double observedValue) {
        int modelValue;
        Distribuition dist;

        modelValue = TS(time);
        dist = new distName(modelValue);


    };
}

