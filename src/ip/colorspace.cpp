#include <cassert>
#include <cmath>
#include <limits>
#include <algorithm>

#include <iostream>

#include "image_processing.h"
#include "ip/util/math_function.h"

namespace ip {
namespace colorspace {

void RGB2Gray(const int w, const int h,
              const uint8_t* const image_data,
              const double* weights,
              uint8_t* const output) {
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      double val = 0;
      for (int k = 0 ; k < 3 ; ++k) {
        val += weights[k]
            * static_cast<double>(image_data[i * w * 3 + j * 3 + k]);
      }
      output[i * w + j] = ip::math::clamp<uint8_t>(val);
    }
  }
}

void Histogram(const int w, const int h, const int d,
               const uint8_t* const image_data,
               const int bin_size, int* const histogram_bin) {
  const int maxval = 256;
  assert(bin_size <= maxval);

  // zero the bin
  for (int i = 0 ; i < bin_size ; ++i) histogram_bin[i] = 0;
  // count the bins
  const int range = maxval / bin_size;
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      for (int k = 0 ; k < d ; ++k) {
        ++histogram_bin[image_data[i * w * d + j * d + k] / range];
      }
    }
  }
}

void GammaCorrection(gamma_correction_method method,
                     const int w, const int h, const int d,
                     const uint8_t* const image_data,
                     uint8_t* const output) {
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      for (int k = 0 ; k < d ; ++k) {
        const int index = i * w * d + j * d + k;
        output[index] =
            ip::math::clamp<uint8_t>(method(image_data[index]));
      }
    }
  }
}

void ConvertTo(colorspace_convert convert,
               const int w, const int h,
               const uint8_t* const image_data,
               uint8_t* const output) {
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w; ++j) {
      uint8_t i1 = image_data[i * w * 3 + j * 3];
      uint8_t i2 = image_data[i * w * 3 + j * 3 + 1];
      uint8_t i3 = image_data[i * w * 3 + j * 3 + 2];
      double c1 = 0;
      double c2 = 0;
      double c3 = 0;
      convert(i1, i2, i3, &c1, &c2, &c3);
      output[i * w * 3 + j * 3] = ip::math::clamp<uint8_t>(c1);
      output[i * w * 3 + j * 3 + 1] = ip::math::clamp<uint8_t>(c2);
      output[i * w * 3 + j * 3 + 2] = ip::math::clamp<uint8_t>(c3);
    }
  }
}

} /* end of colorspace namespace */
} /* end of ip namespace */

void ip_rgb_to_gray(const int w, const int h,
                    const uint8_t* const image_data,
                    const double* weights,
                    uint8_t* const output) {
  ip::colorspace::RGB2Gray(w, h, image_data, weights, output);
}

void ip_histogram(const int w, const int h, const int d,
                  const uint8_t* const image_data,
                  const int bin_size, int* const histogram_bin) {
  ip::colorspace::Histogram(w, h, d, image_data, bin_size, histogram_bin);
}

void ip_rgb_gamma_correction(gamma_correction_method method,
                             const int w, const int h, const int d,
                             const uint8_t* const image_data,
                             uint8_t* const output) {
  ip::colorspace::GammaCorrection(method, w, h, d, image_data, output);
}

void ip_convert_to(colorspace_convert convert,
                   const int w, const int h,
                   const uint8_t* const image_data,
                   uint8_t* const output) {
  ip::colorspace::ConvertTo(convert, w, h, image_data, output);
}
