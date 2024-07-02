#ifndef EYTZINGER_HPP
#define EYTZINGER_HPP

#include <cstddef>
#include <algorithm>
#include <vector>

namespace eytzinger {

template<typename T>
void in_shuffle(T* a, std::size_t start, std::size_t end) {
    std::size_t size = end - start;
    if (size <= 2) return;
    std::size_t mid = start + size / 2;
    std::vector<bool> used(size, false);
    for (std::size_t k = 1; k < size; k += 2) {
        if (!used[k]) {
            std::size_t curr = k;
            T temp = a[start + curr];
            used[curr] = true;
            while (!used[curr / 2]) {
                a[start + curr] = a[start + curr / 2];
                curr /= 2;
                used[curr] = true;
            }
            a[start + curr] = temp;
        }
    }
    in_shuffle(a, start, mid);
    in_shuffle(a, mid, end);
}

template<typename T>
void out_shuffle(T* a, std::size_t start, std::size_t end) {
    std::size_t size = end - start;
    if (size <= 2) return;
    std::size_t mid = start + size / 2;
    out_shuffle(a, start, mid);
    out_shuffle(a, mid, end);
    std::size_t i = 1;
    std::size_t j = mid - start;
    while (i < j) {
        for (std::size_t k = i; k < j; k += 2) {
            std::swap(a[start + k], a[start + k + 1]);
        }
        i *= 2;
        j = mid - start + i / 2;
    }
}

template<typename T>
void eytzinger(T* a, std::size_t n) {
    in_shuffle(a, 0, n);
}

template<typename T>
void uneytzinger(T* a, std::size_t n) {
    out_shuffle(a, 0, n);
}

template<typename T>
std::size_t eytzinger_upper_bound(const T* __restrict__ a, std::size_t n, const T& x) {
    std::size_t i = 0;
    std::size_t prefetch_distance = 4; //Could be fine tuned for system specific examples
    while (i < n) {
        __builtin_prefetch(a + ((i * prefetch_distance + prefetch_distance - 1) & (n - 1)));
        i = 2 * i + 1 + (x > a[i]);
    }
    return (i + 1) >> __builtin_ffs(~static_cast<unsigned int>(i + 1));
}

template<typename T>
std::size_t eytzinger_next(const T* a, std::size_t n, std::size_t i) {
    if (i >= n - 1) return n;
    
    std::size_t k = i + 1;
    while (k & (k - 1)) k = k & (k - 1);
    std::size_t j = i + k;
    return (j < n) ? j : i + 1;
}

template<typename T>
std::size_t eytzinger_prev(const T* a, std::size_t n, std::size_t i) {
    if (i == 0) return n;
    
    std::size_t k = i;
    while (!(k & 1)) k = k | (k - 1);
    std::size_t j = i - ((k + 1) >> 1);
    return j;
}

} // namespace eytzinger

#endif // EYTZINGER_HPP