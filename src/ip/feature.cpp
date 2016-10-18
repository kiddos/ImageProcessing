#include <cassert>
#include <cmath>
#include <limits>
#include <algorithm>

#include "image_processing.h"
#include "ip/util/math_function.h"

namespace ip {
namespace feature {

void lbp(const PADDING padding,
         const int kernel_size, const double start,
         const int w, const int h, const uint8_t* image_data,
         uint8_t* const output) {
  assert(kernel_size % 2 == 1);
  const double pi = ip::math::pi();
  const int range = kernel_size / 2;
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      double val = 0;
      for (int k = 0 ; k < range * 8 ; ++k) {
        const double dx = range * std::cos(k * pi / 4 + start);
        const double dy = range * std::sin(k * pi / 4 + start);
        const int newx = static_cast<int>(std::round(j + dx));
        const int newy = static_cast<int>(std::round(i + dy));

        double neighbour_val = 0;
        if (newx >= 0 && newx < w && newy >= 0 && newy < h) {
          neighbour_val = image_data[newy * w + newx];
        } else {
          switch (padding) {
            case PADDING_SAME:
              neighbour_val = image_data[i * w + j];
              break;
            case PADDING_ZERO:
              neighbour_val = 0;
              break;
            default:
              // default for padding same
              neighbour_val = image_data[i * w + j];
              break;
          }
        }
        if (image_data[i * w + j] >= neighbour_val) {
          val += std::pow(2, k);
        }
      }
      output[i * w + j] = ip::math::clamp<uint8_t>(val);
    }
  }
}

void tlbp(const PADDING padding,
          const int kernel_size, const double start,
          const int w, const int h, const uint8_t* image_data,
          uint8_t* const output) {
  assert(kernel_size % 2 == 1);
  const int range = kernel_size / 2;
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      double val = 0;
      double cp = 0;
      double cn = 0;
      int cp_count = 0;
      int cn_count = 0;
      for (int k = 0 ; k < range * 8 ; ++k) {
        const double dx = range * std::cos(start);
        const double dy = range * std::sin(start);
        const int newx = static_cast<int>(std::round(j + dx));
        const int newy = static_cast<int>(std::round(i + dy));

        double neighbour_val = 0;
        if (newx >= 0 && newx < w && newy >= 0 && newy < h) {
          neighbour_val = image_data[newy * w + newx];
        } else {
          switch (padding) {
            case PADDING_SAME:
              neighbour_val = image_data[i * w + j];
              break;
            case PADDING_ZERO:
              neighbour_val = 0;
              break;
            default:
              // default for padding same
              neighbour_val = image_data[i * w + j];
              break;
          }
        }
        if (image_data[i * w + j] >= neighbour_val) {
          cp += neighbour_val;
          ++ cp_count;
          val += std::pow(2, k);
        } else {
          cn += neighbour_val;
          ++ cn_count;
        }
      }
      val = val * (cp / cp_count - cn / cn_count);
      output[i * w + j] = ip::math::clamp<uint8_t>(val);
    }
  }
}

} /* end of feature namespace */
} /* end of ip namespace */

void ip_lbp(const PADDING padding,
            const int kernel_size, const double start,
            const int w, const int h, const uint8_t* image_data,
            uint8_t* const output) {
  ip::feature::lbp(padding, kernel_size, start, w, h, image_data, output);
}

void ip_tlbp(const PADDING padding,
             const int kernel_size, const double start,
             const int w, const int h, const uint8_t* image_data,
             uint8_t* const output) {
  ip::feature::tlbp(padding, kernel_size, start, w, h, image_data, output);
}
