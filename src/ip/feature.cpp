#include <cassert>
#include <cmath>
#include <limits>
#include <algorithm>

#include <omp.h>

#include "image_processing.h"
#include "ip/util/math_function.h"

namespace ip {
namespace feature {

template <typename DType>
DType GetValidNeighbour(const PADDING padding,
                        const int w, const int h,
                        const uint8_t* const image_data,
                        const int x, const int y) {
  DType val = 0;
  if (x >= 0 && x < w && y >= 0 && y < h) {
    val = image_data[y * w + x];
  } else {
    const int fixedx = std::min(std::max(0, x), w-1);
    const int fixedy = std::min(std::max(0, y), h-1);
    switch (padding) {
      case PADDING_SAME:
        val = image_data[fixedy * w + fixedx];
        break;
      case PADDING_ZERO:
        break;
      default:
        // default for padding same
        val = image_data[fixedy * w + fixedx];
        break;
    }
  }
  return val;
}

void GetNeighbour(const double range, const double angle,
                  const int cx, const int cy,
                  int* x, int* y) {
  const double dx = range * std::cos(angle);
  const double dy = range * std::sin(angle);
  *x = static_cast<int>(std::round(cx + dx));
  *y = static_cast<int>(std::round(cy + dy));
}

double LBP(const int kernel_size, const int start,
           const int cx, const int cy,
           const PADDING padding,
           const int w, const int h,
           const uint8_t* const image_data) {
  const double pi = ip::math::pi();
  const int range = kernel_size / 2;
  const double delta = 2.0 * pi / (range * 8);

  double val = 0;
  for (int k = 0 ; k < range * 8 ; ++k) {
    int newx = 0;
    int newy = 0;
    GetNeighbour(range, k * delta + start, cx, cy, &newx, &newy);

    const double neighbour_val =
        GetValidNeighbour<double>(padding, w, h, image_data, newx, newy);
    if (image_data[cy * w + cx] < neighbour_val) {
      val += std::pow(2, k);
    }
  }
  return val;
}

double TLBP(const int kernel_size, const int start,
            const int cx, const int cy,
            const PADDING padding,
            const int w, const int h,
            const uint8_t* const image_data) {
  const double pi = ip::math::pi();
  const int range = kernel_size / 2;
  const double delta = 2.0 * pi / (range * 8);
  double val = 0;
  double cp = 0;
  double cn = 0;
  int cp_count = 0;
  int cn_count = 0;
  for (int k = 0 ; k < range * 8 ; ++k) {
    int newx = 0;
    int newy = 0;
    GetNeighbour(range, k * delta + start, cx, cy, &newx, &newy);

    const double neighbour_val =
        GetValidNeighbour<double>(padding, w, h, image_data, newx, newy);
    if (image_data[cy * w + cx] < neighbour_val) {
      cp += neighbour_val;
      ++ cp_count;
      val += std::pow(2, k);
    } else {
      cn += neighbour_val;
      ++ cn_count;
    }
  }
  val = val * (cp / cp_count - cn / cn_count);
  return val;
}

double CLBP(const int kernel_size, const int start,
            const int cx, const int cy,
            const PADDING padding,
            const int w, const int h,
            const uint8_t* const image_data) {
  const double pi = ip::math::pi();
  const int range = kernel_size / 2;
  const double delta = 2.0 * pi / (range * 8);

  // compute average
  double ave = 0;
  for (int i = 0 ; i < kernel_size ; ++i) {
    for (int j = 0 ; j < kernel_size ; ++j) {
      ave += image_data[(cy + i) * w + cx + j];
    }
  }
  ave /= (kernel_size * kernel_size);

  double val = 0;
  for (int k = 0 ; k < range * 8 ; ++k) {
    int newx = 0;
    int newy = 0;
    GetNeighbour(range, k * delta + start, cx, cy, &newx, &newy);

    const double neighbour_val =
        GetValidNeighbour<double>(padding, w, h, image_data, newx, newy);
    if (image_data[cy * w + cx] < neighbour_val) {
      val += std::pow(2, 2 * k);
    }
    if (ave < neighbour_val) {
      val += std::pow(2, 2 * k + 1);
    }
  }
  return val;
}

double CSLBP(const int kernel_size, const int start,
             const int cx, const int cy,
             const PADDING padding,
             const int w, const int h,
             const uint8_t* const image_data) {
  const double pi = ip::math::pi();
  const int range = kernel_size / 2;
  const double delta = 2.0 * pi / (range * 8);

  double val = 0;
  for (int k = 0 ; k < range * 8 ; ++k) {
    int newx1 = 0;
    int newy1 = 0;
    int newx2 = 0;
    int newy2 = 0;
    GetNeighbour(range, k * delta + start, cx, cy, &newx1, &newy1);
    GetNeighbour(range, (k + range * 4) * delta + start, cx, cy, &newx2, &newy2);

    const double neighbour_val1 =
        GetValidNeighbour<double>(padding, w, h, image_data, newx1, newy1);
    const double neighbour_val2 =
        GetValidNeighbour<double>(padding, w, h, image_data, newx2, newy2);
    if (neighbour_val1 >= neighbour_val2) {
      val += std::pow(2, k);
    }
  }
  return val;
}

template <int A, int R, int S>
double TPLBP(const int kernel_size, const int start,
             const int cx, const int cy,
             const PADDING padding,
             const int w, const int h,
             const uint8_t* const image_data) {
  const double pi = ip::math::pi();
  const int range = R;
  const double delta = 2.0 * pi / S;

  double val = 0;
  for (int k = 0 ; k < S ; ++k) {
    int newx1 = 0;
    int newy1 = 0;
    int newx2 = 0;
    int newy2 = 0;
    GetNeighbour(range, k * delta + start, cx, cy, &newx1, &newy1);
    GetNeighbour(range, (k + A) * delta + start, cx, cy, &newx2, &newy2);

    const double patch1 = LBP(kernel_size, start, newx1, newy1,
                              padding, w, h, image_data);
    const double patch2 = LBP(kernel_size, start, newx2, newy2,
                              padding, w, h, image_data);
    if (patch1 >= patch2) {
      val += std::pow(2, k);
    }
  }
  return val;
}

void ComputeLBP(LBPFeatureType feature,
                const PADDING padding,
                const int kernel_size, const double start,
                const int w, const int h, const uint8_t* image_data,
                uint8_t* const output) {

  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      const double feature_val = feature(kernel_size, start, j, i,
                                         padding, w, h, image_data);
      output[i * w + j] = ip::math::clamp<uint8_t>(feature_val);
    }
  }
}


double LTP(const int kernel_size, const int start, const double threshold,
           const int cx, const int cy, const PADDING padding,
           const int w, const int h,
           const uint8_t* const image_data) {
  const double pi = ip::math::pi();
  const int range = kernel_size / 2;
  const double delta = 2.0 * pi / (range * 8);

  double val = 0;
  for (int k = 0 ; k < range * 8 ; ++k) {
    int newx = 0;
    int newy = 0;
    GetNeighbour(range, k * delta + start, cx, cy, &newx, &newy);

    const double neighbour_val =
        GetValidNeighbour<double>(padding, w, h, image_data, newx, newy);
    const double current_val = image_data[cy * w + cx];
    if (neighbour_val >= current_val + threshold) {
      val += 2 * std::pow(3, k);
    } else if (neighbour_val >= current_val - threshold &&
               neighbour_val < current_val + threshold) {
      val += std::pow(3, k);
    }
  }
  return val;
}

double CSLTP(const int kernel_size, const int start, const double threshold,
             const int cx, const int cy, const PADDING padding,
             const int w, const int h,
             const uint8_t* const image_data) {
  const double pi = ip::math::pi();
  const int range = kernel_size / 2;
  const double delta = 2.0 * pi / (range * 8);

  double val = 0;
  for (int k = 0 ; k < range * 8 ; ++k) {
    int newx1 = 0;
    int newy1 = 0;
    int newx2 = 0;
    int newy2 = 0;
    GetNeighbour(range, k * delta + start, cx, cy, &newx1, &newy1);
    GetNeighbour(range, (k + range * 4) * delta + start,
                 cx, cy, &newx2, &newy2);

    const double neighbour_val1 =
        GetValidNeighbour<double>(padding, w, h, image_data, newx1, newy1);
    const double neighbour_val2 =
        GetValidNeighbour<double>(padding, w, h, image_data, newx2, newy2);
    if (neighbour_val1 >= neighbour_val2 + threshold) {
      val += 2 * std::pow(3, k);
    } else if (neighbour_val1 >= neighbour_val2 - threshold &&
               neighbour_val1 < neighbour_val2 + threshold) {
      val += std::pow(3, k);
    }
  }
  return val;
}

template <int A, int R, int S>
double TPCSLTP(const int kernel_size, const int start, const double threshold,
               const int cx, const int cy, const PADDING padding,
               const int w, const int h,
               const uint8_t* const image_data) {
  const double pi = ip::math::pi();
  const int range = R;
  const double delta = 2.0 * pi / S;

  double val = 0;
  for (int k = 0 ; k < S ; ++k) {
    int newx1 = 0;
    int newy1 = 0;
    int newx2 = 0;
    int newy2 = 0;
    GetNeighbour(range, k * delta + start, cx, cy, &newx1, &newy1);
    GetNeighbour(range, (k + A) * delta + start, cx, cy, &newx2, &newy2);

    const double patch1 = CSLTP(kernel_size, start, threshold, newx1, newy1,
                                padding, w, h, image_data);
    const double patch2 = CSLTP(kernel_size, start, threshold, newx2, newy2,
                                padding, w, h, image_data);
    if (patch1 >= patch2 + threshold) {
      val += 2 * std::pow(3, k);
    } else if (patch1 >= patch2 - threshold &&
               patch1 < patch2 + threshold){
      val += std::pow(3, k);
    }
  }
  return val;
}

void ComputeLTP(LTPFeatureType feature,
                const PADDING padding,
                const int kernel_size, const double start,
                const double threshold,
                const int w, const int h, const uint8_t* image_data,
                double* const output) {
#pragma omp parallel for
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      const double feature_val = feature(kernel_size, start, threshold,
                                         j, i, padding, w, h, image_data);
      output[i * w + j] = ip::math::clamp<double>(feature_val);
    }
  }
}

double LZP(const int* const order, const int order_size,
           const int cx, const int cy, const PADDING padding,
           const int w, const int h,
           const uint8_t* const image_data) {
  double val = 0;
  for (int k = 0 ; k < order_size ; ++k) {
    const int newx = cx + order[k * 2];
    const int newy = cy + order[k * 2 + 1];

    const double neighbour_val =
        GetValidNeighbour<double>(padding, w, h, image_data, newx, newy);
    if (neighbour_val > image_data[cy * w + cx]) {
      val += std::pow(2, k);
    }
  }
  return val;
}

template <int A, int R, int S>
double TPLZP(const int* const order, const int order_size,
             const int cx, const int cy, const PADDING padding,
             const int w, const int h,
             const uint8_t* const image_data) {
  const int shift = order_size / 2;
  double val = 0;
  for (int k = 0 ; k < order_size ; ++k) {
    const int newx1 = cx + order[k * 2];
    const int newy1 = cy + order[k * 2 + 1];
    const int newx2 = cx + order[(k * 2 + shift) % order_size];
    const int newy2 = cy + order[(k * 2 + 1 + shift) % order_size];

    const double neighbour_val1 =
        GetValidNeighbour<double>(padding, w, h, image_data, newx1, newy1);
    const double neighbour_val2 =
        GetValidNeighbour<double>(padding, w, h, image_data, newx2, newy2);
    if (neighbour_val1 > neighbour_val2) {
      val += std::pow(2, k);
    }
  }
  return val;
}

void ComputeLZP(LZPFeatureType feature,
                const int* const order, const int order_size,
                const enum PADDING padding,
                const int w, const int h, const uint8_t* const image_data,
                uint8_t* const output) {
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      const double feature_val = feature(order, order_size, j, i, padding,
                                         w, h, image_data);
      output[i * w + j] = ip::math::clamp<uint8_t>(feature_val);
    }
  }
}

LDPDirection LDPEast() {
  LDPDirection direction;
  direction.dir = new double[9] {
    -3, -3,  5,
    -3,  0,  5,
    -3, -3,  5
  };
  direction.w = 3;
  direction.h = 3;
  return direction;
}

LDPDirection LDPNorthEast() {
  LDPDirection direction;
  direction.dir = new double[9] {
    -3,  5,  5,
    -3,  0,  5,
    -3, -3, -3
  };
  direction.w = 3;
  direction.h = 3;
  return direction;
}

LDPDirection LDPNorth() {
  LDPDirection direction;
  direction.dir = new double[9] {
     5,  5,  5,
    -3,  0, -3,
    -3, -3, -3
  };
  direction.w = 3;
  direction.h = 3;
  return direction;
}

LDPDirection LDPNorthWest() {
  LDPDirection direction;
  direction.dir = new double[9] {
     5,  5, -3,
     5,  0, -3,
    -3, -3, -3
  };
  direction.w = 3;
  direction.h = 3;
  return direction;
}

LDPDirection LDPWest() {
  LDPDirection direction;
  direction.dir = new double[9] {
     5, -3, -3,
     5,  0, -3,
     5, -3, -3
  };
  direction.w = 3;
  direction.h = 3;
  return direction;
}

LDPDirection LDPSouthWest() {
  LDPDirection direction;
  direction.dir = new double[9] {
    -3, -3, -3,
     5,  0, -3,
     5,  5, -3
  };
  direction.w = 3;
  direction.h = 3;
  return direction;
}

LDPDirection LDPSouth() {
  LDPDirection direction;
  direction.dir = new double[9] {
    -3, -3, -3,
    -3,  0, -3,
     5,  5,  5
  };
  direction.w = 3;
  direction.h = 3;
  return direction;
}

LDPDirection LDPSouthEast() {
  LDPDirection direction;
  direction.dir = new double[9] {
    -3, -3, -3,
    -3,  0,  5,
    -3,  5,  5
  };
  direction.w = 3;
  direction.h = 3;
  return direction;
}

void ComputeLDP(LDPDirection direction,
                const enum PADDING padding,
                const int w, const int h, const uint8_t* const image_data,
                uint8_t* const output) {
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      double val = 0;
      for (int k = 0 ; k < direction.h ; ++k) {
        for (int l = 0 ; l < direction.w ; ++l) {
          const int newx = j + l;
          const int newy = i + k;

          const double neighbour_val =
              GetValidNeighbour<double>(padding, w, h, image_data, newx, newy);
          val += direction.dir[k * direction.w + l] * neighbour_val;
        }
      }
      output[i * w + j] = ip::math::clamp<uint8_t>(val);
    }
  }
}

LDPDirection ldp_east = LDPEast();
LDPDirection ldp_north_east = LDPNorthEast();
LDPDirection ldp_north = LDPNorth();
LDPDirection ldp_north_west = LDPNorthWest();
LDPDirection ldp_west = LDPWest();
LDPDirection ldp_south_west = LDPSouthWest();
LDPDirection ldp_south = LDPSouth();
LDPDirection ldp_south_east = LDPSouthEast();

} /* end of feature namespace */
} /* end of ip namespace */

LTPFeatureType ltp = ip::feature::LTP;
LTPFeatureType csltp = ip::feature::CSLTP;
LTPFeatureType tpltp = ip::feature::TPCSLTP<1, 3, 8>;

void ip_lbp(LBPFeatureType feature,
            const enum PADDING padding,
            const int kernel_size, const double start,
            const int w, const int h, const uint8_t* image_data,
            uint8_t* const output) {
  ip::feature::ComputeLBP(feature, padding, kernel_size, start,
                          w, h, image_data, output);
}

void ip_ltp(LTPFeatureType feature,
            const enum PADDING padding,
            const int kernel_size, const double start, const double threshold,
            const int w, const int h, const uint8_t* const image_data,
            double* const output) {
  ip::feature::ComputeLTP(feature, padding, kernel_size, start, threshold,
                          w, h, image_data, output);
}

void ip_lzp(LZPFeatureType feature,
            const int* const order, const int order_size,
            const enum PADDING padding,
            const int w, const int h, const uint8_t* const image_data,
            uint8_t* const output) {
  ip::feature::ComputeLZP(feature, order, order_size,
                          padding, w, h, image_data, output);
}

void ip_ldp(LDPDirection direction,
            const enum PADDING padding,
            const int w, const int h, const uint8_t* const image_data,
            uint8_t* const output) {
  ip::feature::ComputeLDP(direction, padding, w, h, image_data, output);
}
