#pragma once
#include <vector>

namespace Benchmark
{
    void   Begin();     //start timer la inceput de frame
    void   End();       //stop timer si salveaza datele
    double LastMs();    //cat a durat ultimul frame
    double AvgMs();     //average
    void   Reset();     //reseteaza statisticile
    void   DrawUI();    //desenneaza ui-ul
}
