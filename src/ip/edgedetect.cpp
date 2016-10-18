#include <cassert>

#include "image_processing.h"
#include "ip/util/math_function.h"

namespace ip {
namespace edgedetect {

void PrepareSobelKernel(double* gx, double* gy, const int kernel_size) {
  for (int i = 0 ; i < kernel_size ; ++i) {
    if (i == kernel_size / 2) {
      for (int j = 0 ; j < kernel_size ; ++j) {
        gy[i * kernel_size + j] = 0;
        gx[j * kernel_size + i] = 0;
      }
    } else {
      double row_val = i - kernel_size / 2;
      double dr = -1;
      for (int j = 0 ; j < kernel_size ; ++j) {
        gy[i * kernel_size + j] = row_val;
        gx[j * kernel_size + i] = row_val;
        if (j == kernel_size / 2) dr = -dr;
        row_val += dr;
      }
    }
  }
}

template <typename T, typename U>
void Sobel(const int kernel_size, const int w, const int h,
           const T* const image_data, U* const output) {
  assert(kernel_size % 2 == 1);
  // prepare filter
  double* gx = new double[kernel_size * kernel_size];
  double* gy = new double[kernel_size * kernel_size];
  PrepareSobelKernel(gx, gy, kernel_size);

  double* dx = new double[w * h];
  double* dy = new double[w * h];

  ip::math::convolution2d(kernel_size / 2, 1, w, h, image_data,
                          kernel_size, kernel_size, gx,
                          w, h, dx);
  ip::math::convolution2d(kernel_size / 2, 1, w, h, image_data,
                          kernel_size, kernel_size, gy,
                          w, h, dy);
  for (int i = 0 ; i < w * h ; ++i) {
    const double val = std::sqrt(dx[i] * dx[i] + dy[i] * dy[i]);
    output[i] = ip::math::clamp<U>(val);
  }
}

void PrepareGaussianBlurKernel(const int kernel_size,
                               double sigmax, double sigmay,
                               double* kernel) {
  constexpr double pi = ip::math::pi();
  for (int i = 0 ; i < kernel_size ; ++i) {
    for (int j = 0 ; j < kernel_size ; ++j) {
      const double dx = j - kernel_size / 2;
      const double dy = i - kernel_size / 2;
      kernel[i * kernel_size + j] = (1.0 / 2 / pi)
          * std::exp(-(dx * dx / 2 / sigmax) - (dy * dy / 2 / sigmay));
    }
  }
}

template <typename DType>
void GaussianBlur(const PADDING padding, const int kernel_size,
                  double sigmax, double sigmay,
                  const int w, const int h, const uint8_t* const image_data,
                  DType* const output) {
  assert(kernel_size % 2 == 1);
  // prepare kernel
  double* kernel = new double[kernel_size * kernel_size];
  PrepareGaussianBlurKernel(kernel_size, sigmax, sigmay, kernel);

  const int padding_size = kernel_size / 2;
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      double val = 0;
      for (int k = 0 ; k < kernel_size ; ++k) {
        for (int l = 0 ; l < kernel_size ; ++l) {
          const int r = i - padding_size + k;
          const int c = j - padding_size + l;
          if (r >= 0 && r < h && c >= 0 && c < w) {
            val += image_data[r * w + c] * kernel[k * kernel_size + l];
          } else {
            switch (padding) {
              case PADDING_SAME:
                val += image_data[i * w + j] * kernel[k * kernel_size + l];
                break;
              case PADDING_ZERO:
                break;
              default:
                // default to padding same
                val += image_data[i * w + j] * kernel[k * kernel_size + l];
                break;
            }
          }
        }
      }
      output[i * w + j] = ip::math::clamp<DType>(val);
    }
  }
}

template <typename T, typename U>
void NonMaximumSuppression(const int w, const int h,
                           const T* const data,
                           U* const output) {
  const int delta[4][4] = {
    {-1, 1, 0, 0}, {-1, 1, 1, -1}, {0, 0, -1, 1}, {-1, -1, 1, 1}};
  for (int i = 1 ; i < h - 1 ; ++i) {
    for (int j = 1 ; j < w - 1 ; ++j) {
      double max_grad =
          std::abs(data[(i + delta[0][0]) * w + j + delta[0][2]] -
                   data[(i + delta[0][1]) * w + j + delta[0][3]]);
      int direction = 0;
      for (int d = 1 ; d < 4 ; ++d) {
        double new_grad =
            std::abs(data[(i + delta[d][0]) * w + j + delta[d][2]] -
                     data[(i + delta[d][1]) * w + j + delta[d][3]]);
        if (new_grad > max_grad) {
          max_grad = new_grad;
          direction = d;
        }
      }

      const int d2 = (direction + 2) % 4;
      const T current_val = data[i * w + j];
      const T other_val1 = data[(i + delta[d2][0]) + j + delta[d2][2]];
      const T other_val2 = data[(i + delta[d2][1]) + j + delta[d2][3]];
      if (current_val < other_val1 || current_val < other_val2) {
        output[i * w + j] = 0;
      } else {
        output[i * w + j] = ip::math::clamp<U>(current_val);
      }
    }
  }
}

template <typename DType>
void Hysteresis(const double lthreshold, const double hthreshold,
                const int w, const int h, const double* const data,
                DType* const output) {
  const DType maxval = std::numeric_limits<DType>::max();
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      const double val = data[i * w + j];
      if (val >= hthreshold) {
        output[i * w + j] = maxval;
      } else if (val < lthreshold) {
        output[i * w + j] = 0;
      } else {
        output[i * w + j] = 0;
        const int delta[8][2] = {
          {1, 0}, {-1, 0}, {0, -1}, {0, 1},
          {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
        };
        for (int d = 0 ; d < 8 ; ++d) {
          const int x = j + delta[d][0];
          const int y = i + delta[d][1];
          if (x >= 0 && x < w && y >= 0 && y < h &&
              data[y * w + x] >= hthreshold) {
            output[i * w + j] = maxval;
          }
        }
      }
    }
  }
}

void Canny(const int kernel_size,
           const int w, const int h, const uint8_t* const image_data,
           const int lower_threshold, const int higher_threshold,
           uint8_t* const output) {
  // 1. apply gaussian blur
  double* blurred = new double[w * h];
  GaussianBlur(PADDING_SAME, 5, 1.0, 1.0, w, h, image_data, blurred);
  // 2. apply sobel derivative
  double* sobel = new double[w * h];
  Sobel(kernel_size, w, h, blurred, sobel);
  // 3. apply non-maximum suppression
  double* suppress = new double[w * h];
  NonMaximumSuppression(w, h, sobel, suppress);
  // 4. apply hysteresis
  Hysteresis(lower_threshold, higher_threshold, w, h, suppress,
             output);

  delete[] blurred;
  delete[] sobel;
  delete[] suppress;
}

void Laplacian(LaplacianFilter filter,
               const PADDING padding,
               const int w, const int h, const uint8_t* const image_data,
               uint8_t* const output) {
  const int padding_size = filter.size / 2;
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      double val = 0;
      for (int k = 0 ; k < filter.size ; ++k) {
        for (int l = 0 ; l < filter.size ; ++l) {
          const int y = i - padding_size + k;
          const int x = j - padding_size + l;
          if (x >= 0 && x < w && y >= 0 && y < h) {
            val += image_data[y * w + x]
                * filter.filter[k * filter.size + l];
          } else {
            switch (padding) {
              case PADDING_SAME:
                val += image_data[i * w + j]
                    * filter.filter[k * filter.size + l];
                break;
              case PADDING_ZERO:
                break;
              default:
                val += image_data[i * w + j]
                    * filter.filter[k * filter.size + l];
                break;
            }
          }
        }
      }
      output[i * w + j] = ip::math::clamp<uint8_t>(val);
    }
  }
}

} /* end of edgedetect namespace */
} /* end of ip namespace */

void ip_sobel(const int kernel_size,
              const int w, const int h, const uint8_t* const image_data,
              uint8_t* const output) {
  ip::edgedetect::Sobel(kernel_size, w, h, image_data, output);
}

void ip_canny(const int kernel_size,
              const int w, const int h, const uint8_t* const image_data,
              const int lower_threshold, const int higher_threshold,
              uint8_t* const output) {
  ip::edgedetect::Canny(kernel_size, w, h, image_data,
                        lower_threshold, higher_threshold, output);
}

void ip_laplacian(LaplacianFilter filter,
                  const enum PADDING padding,
                  const int w, const int h, const uint8_t* const image_data,
                  uint8_t* const output) {
  ip::edgedetect::Laplacian(filter, padding, w, h, image_data, output);
}
