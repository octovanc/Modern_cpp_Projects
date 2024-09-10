#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cstdint>

// Enumeration for minimum and maximum selection
enum class minmax_t : uint8_t { MIN, MAX };

// Helper function to get the minimum or maximum value based on the template parameter
template <minmax_t M, typename T>
constexpr T get(const std::vector<T>& v1, const std::vector<T>& v2, const std::vector<T>& v3) {
    auto op = [](const T& a, const T& b) {
        if constexpr (M == minmax_t::MIN) {
            return a < b ? a : b;  // Compare for minimum
        } else {
            return a > b ? a : b;  // Compare for maximum
        }
    };

    // Get the minimum/maximum of each vector, and then compare between vectors
    T result1 = *std::min_element(v1.begin(), v1.end());
    T result2 = *std::min_element(v2.begin(), v2.end());
    T result3 = *std::min_element(v3.begin(), v3.end());

    return op(op(result1, result2), result3);
}

// Function to get both the minimum and the maximum
template <typename T>
constexpr std::pair<T, T> get(const std::vector<T>& v1, const std::vector<T>& v2, const std::vector<T>& v3) {
    T min_val = get<minmax_t::MIN>(v1, v2, v3);  // Get minimum
    T max_val = get<minmax_t::MAX>(v1, v2, v3);  // Get maximum
    return {min_val, max_val};
}

int main() {
    std::vector<int> vec1 = {10, 20, 30};
    std::vector<int> vec2 = {5, 25, 35};
    std::vector<int> vec3 = {0, 15, 40};

    // Test with integers
    std::cout << "Testing with integers:" << std::endl;
    std::cout << "Minimum value: " << get<minmax_t::MIN>(vec1, vec2, vec3) << std::endl;
    std::cout << "Maximum value: " << get<minmax_t::MAX>(vec1, vec2, vec3) << std::endl;

    auto [min_val, max_val] = get(vec1, vec2, vec3);
    std::cout << "Min value: " << min_val << ", Max value: " << max_val << std::endl;

    // Test with floating-point numbers
    std::vector<double> vec4 = {1.1, 2.2, 3.3};
    std::vector<double> vec5 = {0.5, 2.5, 3.5};
    std::vector<double> vec6 = {1.0, 2.0, 4.0};

    std::cout << "\nTesting with doubles:" << std::endl;
    std::cout << "Minimum value: " << get<minmax_t::MIN>(vec4, vec5, vec6) << std::endl;
    std::cout << "Maximum value: " << get<minmax_t::MAX>(vec4, vec5, vec6) << std::endl;

    auto [min_dbl, max_dbl] = get(vec4, vec5, vec6);
    std::cout << "Min value: " << min_dbl << ", Max value: " << max_dbl << std::endl;

    return 0;
}