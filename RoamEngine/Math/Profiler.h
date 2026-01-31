// Profiler.h - The performance analyzer
// This measures how fast our code runs and finds bottlenecks

#ifndef PROFILER_H
#define PROFILER_H

#include <string>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>

// Profile sample - a single measurement
struct ProfileSample {
    std::string name;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    std::chrono::microseconds duration;
    uint32_t threadId;
    uint32_t depth;
};

// Profile result - aggregated data
struct ProfileResult {
    std::string name;
    uint64_t callCount;
    std::chrono::microseconds totalTime;
    std::chrono::microseconds averageTime;
    std::chrono::microseconds minTime;
    std::chrono::microseconds maxTime;
    uint32_t depth;
};

// The Profiler class - our performance detective
class Profiler {
public:
    static Profiler& GetInstance();

    // Start profiling a section - begin timing
    void BeginSample(const std::string& name);

    // End profiling a section - stop timing
    void EndSample();

    // Get profiling results - what did we learn?
    std::vector<ProfileResult> GetResults();

    // Clear all profiling data - start fresh
    void Clear();

    // Enable/disable profiling - turn it on/off
    void SetEnabled(bool enabled) { isEnabled = enabled; }
    bool IsEnabled() const { return isEnabled; }

    // Set maximum depth - how deep do we go?
    void SetMaxDepth(uint32_t depth) { maxDepth = depth; }

    // Save results to file - preserve our findings
    bool SaveToFile(const std::string& filename);

    // Load results from file - review past performance
    bool LoadFromFile(const std::string& filename);

private:
    Profiler();
    ~Profiler();

    // Prevent copying
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    // Current samples - what's running now?
    std::vector<ProfileSample> currentSamples;

    // Completed samples - what have we measured?
    std::vector<ProfileSample> completedSamples;

    // Mutex for thread safety - don't let threads mess with our data
    std::mutex profilerMutex;

    // Configuration
    bool isEnabled;
    uint32_t maxDepth;
    uint32_t currentDepth;
};

// Scoped profiler - automatic timing
class ScopedProfiler {
public:
    ScopedProfiler(const std::string& name) : sampleName(name) {
        Profiler::GetInstance().BeginSample(sampleName);
    }

    ~ScopedProfiler() {
        Profiler::GetInstance().EndSample();
    }

private:
    std::string sampleName;
};

// Macros for easy profiling - just add these to your code
#define PROFILE_SCOPE(name) ScopedProfiler profiler##__LINE__(name)
#define PROFILE_BEGIN(name) Profiler::GetInstance().BeginSample(name)
#define PROFILE_END() Profiler::GetInstance().EndSample()

#endif // PROFILER_H
