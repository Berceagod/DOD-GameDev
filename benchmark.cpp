#include "benchmark.h"
#include <chrono>
#include <algorithm>
#include "imgui.h"

namespace
{
    using Clock = std::chrono::steady_clock;

    Clock::time_point startTime;
    double lastMs  = 0.0;
    double totalMs = 0.0;
    long   samples = 0;
    //cel mai slab frame
    double maxMs = 0.0;

    //buffer pentru graficul din ImGui
    const int HISTORY_SIZE = 120; //max 120
    float frameHistory[HISTORY_SIZE] = { 0 };
    int historyIdx = 0;
}

namespace Benchmark
{
    void Begin()
    {
        startTime = Clock::now();
    }

    void End()
    {
        
        std::chrono::duration<double, std::milli> time = Clock::now() - startTime;
        lastMs = time.count();

        totalMs += lastMs;
        samples++;

        //updatam max ms
        if (lastMs > maxMs) maxMs = lastMs;

        //adaugam in bufferul pt grafic
        frameHistory[historyIdx] = static_cast<float>(lastMs);
        historyIdx = (historyIdx + 1) % HISTORY_SIZE;
    }

    double LastMs() { return lastMs; }

    double AvgMs()  { return samples > 0 ? totalMs / samples : 0.0; }

    //resetam statisicile
    void Reset()
    {
        lastMs = 0.0;
        totalMs = 0.0;
        samples = 0;
        maxMs = 0.0;
        
        for (int i = 0; i < HISTORY_SIZE; i++) {
            frameHistory[i] = 0.0f;
        }
        historyIdx = 0;
    }

    void DrawUI()
    {
        ImGui::Begin("Benchmark");
        
        //calculam frameurile pe secunda
        double fps = lastMs > 0.0 ? (1000.0 / lastMs) : 0.0;
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "FPS: %.1f", fps);
        ImGui::Separator();

        ImGui::Text("Last: %.4f ms", lastMs);
        ImGui::Text("Avg:  %.4f ms", AvgMs());
        ImGui::Text("Max:  %.4f ms", maxMs);
        ImGui::Text("Samples: %ld", samples);

        ImGui::Spacing();
        
        //desenam graficul
        float plotMax = static_cast<float>(maxMs > 16.0 ? maxMs : 16.0); 
        ImGui::PlotLines("Frame Time", frameHistory, HISTORY_SIZE, historyIdx, 
                         "ms per frame", 0.0f, plotMax, ImVec2(0, 80));

        ImGui::Spacing();
        if (ImGui::Button("Reset Benchmark", ImVec2(-1, 0))) {
            Reset();
        }
        ImGui::End();
    }
}