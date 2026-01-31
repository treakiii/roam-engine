// Random.h - The random number generator
// This creates unpredictable numbers for games and simulations

#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <vector>
#include <chrono>

// The Random class - our chaos generator
class Random {
public:
    Random();
    Random(uint32_t seed);
    ~Random();

    // Set seed - control the chaos
    void SetSeed(uint32_t seed);

    // Integer generation - whole numbers
    int NextInt();
    int NextInt(int max); // 0 to max-1
    int NextInt(int min, int max); // min to max-1

    // Float generation - decimal numbers
    float NextFloat(); // 0.0 to 1.0
    float NextFloat(float max); // 0.0 to max
    float NextFloat(float min, float max); // min to max

    // Double generation - high precision decimals
    double NextDouble(); // 0.0 to 1.0
    double NextDouble(double max); // 0.0 to max
    double NextDouble(double min, double max); // min to max

    // Boolean generation - true or false
    bool NextBool();
    bool NextBool(float trueProbability); // probability of true

    // Vector generation - random vectors
    class Vector3 NextVector3(); // unit sphere
    class Vector3 NextVector3(float length); // sphere of given radius
    class Vector3 NextVector3(const class Vector3& min, const class Vector3& max); // box

    // Color generation - random colors
    uint32_t NextColor(); // RGBA
    uint32_t NextColor(float alpha); // with specific alpha

    // Choice from collection - pick one
    template<typename T>
    const T& Choose(const std::vector<T>& collection) {
        if (collection.empty()) {
            throw std::out_of_range("Collection is empty");
        }
        return collection[NextInt(collection.size())];
    }

    // Shuffle collection - randomize order
    template<typename T>
    void Shuffle(std::vector<T>& collection) {
        for (size_t i = collection.size() - 1; i > 0; --i) {
            size_t j = NextInt(i + 1);
            std::swap(collection[i], collection[j]);
        }
    }

    // Weighted choice - pick based on weights
    template<typename T>
    const T& ChooseWeighted(const std::vector<T>& items, const std::vector<float>& weights) {
        if (items.size() != weights.size() || items.empty()) {
            throw std::invalid_argument("Items and weights must have same size and not be empty");
        }

        float totalWeight = 0.0f;
        for (float weight : weights) {
            totalWeight += weight;
        }

        float randomValue = NextFloat(totalWeight);
        float cumulativeWeight = 0.0f;

        for (size_t i = 0; i < items.size(); ++i) {
            cumulativeWeight += weights[i];
            if (randomValue <= cumulativeWeight) {
                return items[i];
            }
        }

        // Fallback (should not happen)
        return items.back();
    }

    // Noise generation - Perlin-like noise
    float PerlinNoise(float x, float y = 0.0f, float z = 0.0f);
    float SimplexNoise(float x, float y = 0.0f, float z = 0.0f);

    // Distribution functions - specialized random
    float NextGaussian(float mean = 0.0f, float stddev = 1.0f); // normal distribution
    float NextExponential(float lambda = 1.0f); // exponential distribution
    int NextPoisson(float lambda); // Poisson distribution

    // Seed generation - create random seeds
    static uint32_t GenerateSeed();
    static uint32_t GenerateSeedFromTime();

    // Global instance - shared random generator
    static Random& Global();

private:
    // Mersenne Twister engine - our random core
    std::mt19937 engine;

    // For Perlin noise - permutation table
    std::vector<int> permutation;

    // Initialize permutation table
    void InitializePermutation();
    int Permute(int x) const;
    float Fade(float t) const;
    float Lerp(float a, float b, float t) const;
    float Grad(int hash, float x, float y, float z) const;
};

// Convenience functions - quick random generation
namespace RandomUtils {
    // Quick random numbers without creating instance
    int Int(int min = 0, int max = INT_MAX);
    float Float(float min = 0.0f, float max = 1.0f);
    double Double(double min = 0.0, double max = 1.0);
    bool Bool(float trueProbability = 0.5f);

    // Quick choice
    template<typename T>
    const T& Choose(const std::vector<T>& collection) {
        return Random::Global().Choose(collection);
    }

    // Quick shuffle
    template<typename T>
    void Shuffle(std::vector<T>& collection) {
        Random::Global().Shuffle(collection);
    }
}

#endif // RANDOM_H
