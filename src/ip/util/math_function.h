#ifndef MATH_FUNCTION_H
#define MATH_FUNCTION_H

#include <limits>
#include <algorithm>
#include <cmath>

#ifdef USE_OPENMP
#include <omp.h>
#endif

namespace ip {
namespace math {

inline constexpr double pi() { return std::atan(1) * 4; }

template <typename T, typename U>
inline constexpr T min(T t, U u) { return t < u ? t : u; }

template <typename T, typename U>
inline constexpr T max(T t, U u) { return t > u ? t : u; }

template <typename T, typename U>
inline T clamp(U u) {
  T maxval = static_cast<T>(std::numeric_limits<U>::max());
  T minval = static_cast<T>(std::numeric_limits<U>::min());
  return min(max(u, minval), maxval);
}

template <typename T, typename U, typename V>
inline void convolution2d(const int padding, const int stride,
                          const int wa, const int ha,
                          const T* const a,
                          const int wb, const int hb,
                          const U* const b,
                          const int wc, const int hc,
                          V* const c) {
  const V maxval = std::numeric_limits<T>::max();
  const V minval = std::numeric_limits<T>::min();
#ifdef USE_OPENMP
#pragma omp parallel for default(none) if (wa * ha > 1024)
#endif
  for (int i = 0 ; i < wc ; ++i) {
    for (int j = 0 ; j < hc ; ++j) {
      V val = 0;
      for (int u = 0 ; u < wb ; ++u) {
        for (int v = 0 ; v < hb ; ++v) {
          const int a_c = i * stride - padding + u;
          const int a_r = j * stride - padding + v;
          if (a_c >= 0 && a_r >= 0 && a_c < wa && a_r < ha)
            val += static_cast<V>(a[a_c * ha + a_r]) * b[u * hb + v];
        }
      }
      c[i * hc + j] = std::max(std::min(val, maxval), minval);
    }
  }
}


} /* end of math namespace */
} /* end of ip namespace */

#endif /* end of include guard: MATH_FUNCTION_H */
