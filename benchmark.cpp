#include "benchmark.h"
#include <chrono>
#include "imgui.h"

namespace
{
    using Clock = std::chrono::steady_clock;

    Clock::time_point startTime;
    double lastMs  = 0.0;
    double totalMs = 0.0;
    long   samples = 0;
}

namespace Benchmark
{
    void Begin()
    {
        startTime = Clock::now();
    }

    void End()
    {
        std::chrono::duration<double, std::milli> dt = Clock::now() - startTime;
        lastMs = dt.count();
        totalMs += lastMs;
        samples++;
    }

    double LastMs() { return lastMs; }
    double AvgMs()  { return samples > 0 ? totalMs / samples : 0.0; }

    void Reset()
    {
        lastMs = 0.0;
        totalMs = 0.0;
        samples = 0;
    }

    void DrawUI()
    {
        ImGui::Begin("Benchmark");
        ImGui::Text("Last: %.4f ms", lastMs);
        ImGui::Text("Avg:  %.4f ms", AvgMs());
        ImGui::Text("Samples: %ld", samples);
        if (ImGui::Button("Reset")) Reset();
        ImGui::End();
    }
}