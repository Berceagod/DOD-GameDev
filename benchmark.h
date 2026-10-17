#pragma once

namespace Benchmark
{
    void   Begin();     // start timer
    void   End();       // stop timer, record the sample
    double LastMs();    // last measured time in ms
    double AvgMs();     // average of all samples since Reset
    void   Reset();
    void   DrawUI();    // call between rlImGuiBegin() and rlImGuiEnd()
}
