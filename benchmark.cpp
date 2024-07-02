//Qayam Damji 2024
#include "eytzinger.hpp"
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <functional>

// Prevent over-optimization
volatile std::size_t sink;

// Utility function for timing
template<typename F>
double time_function(F func, int iterations = 1000) {
    // Warm-up run
    for (int i = 0; i < iterations / 10; ++i) {
        func();
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        sink = func(); // Use volatile to prevent over-optimization
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return diff.count() / iterations;
}

// Benchmark function
void run_benchmark(const std::vector<std::size_t>& sizes, std::size_t searches_per_size, const std::string& csv_filename) {
    std::ofstream csv_file(csv_filename);
    csv_file << "Size,Distribution,StdUpperBound,EytzingerUpperBound\n";

    std::random_device rd;
    std::mt19937 gen(rd());

    // Different distributions to test
    std::vector<std::pair<std::string, std::function<int(std::mt19937&, int)>>> distributions = {
        {"Uniform", [](std::mt19937& g, int max) { return std::uniform_int_distribution<>(0, max)(g); }},
        {"Normal", [](std::mt19937& g, int max) { return std::min(max, std::max(0, static_cast<int>(std::normal_distribution<>(max/2, max/6)(g)))); }},
        {"Exponential", [](std::mt19937& g, int max) { return std::min(max, static_cast<int>(std::exponential_distribution<>(3)(g))); }},
    };

    for (std::size_t size : sizes) {
        std::vector<int> data(size);
        for (std::size_t i = 0; i < size; ++i) {
            data[i] = static_cast<int>(i);
        }

        std::vector<int> eytzinger_data = data;
        eytzinger::eytzinger(eytzinger_data.data(), eytzinger_data.size());

        for (const auto& [dist_name, dist_func] : distributions) {
            std::vector<int> search_values(searches_per_size);
            for (int& val : search_values) {
                val = dist_func(gen, static_cast<int>(size - 1));
            }

            // Benchmark std::upper_bound
            double std_upper_bound_time = time_function([&]() {
                return std::upper_bound(data.begin(), data.end(), search_values[gen() % search_values.size()]) - data.begin();
            }, 10000);

            // Benchmark eytzinger
            double eytzinger_upper_bound_time = time_function([&]() {
                return eytzinger::eytzinger_upper_bound(eytzinger_data.data(), eytzinger_data.size(), search_values[gen() % search_values.size()]);
            }, 10000);

            csv_file << size << ","
                     << dist_name << ","
                     << std_upper_bound_time << ","
                     << eytzinger_upper_bound_time << "\n";

            std::cout << "Completed benchmark for size: " << size << ", distribution: " << dist_name << std::endl;
        }
    }

    csv_file.close();
}

int main() {
    std::vector<std::size_t> sizes;
    for (std::size_t i = 10; i <= 28; ++i) {
        sizes.push_back(1ULL << i);  // 2^10 to 2^28
    }
    std::size_t searches_per_size = 10000;

    run_benchmark(sizes, searches_per_size, "search_benchmark.csv");

    return 0;
}