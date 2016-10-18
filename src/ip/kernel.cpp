#include <cmath>

#include "image_processing.h"

namespace ip {
namespace kernel {

inline double GammaCorrectionMethod1(double val) {
  const double normed_val = val / 255.0;
  if (normed_val < 0.0812) {
    return normed_val / 4.5;
  } else {
    return std::pow((normed_val + 0.099) / 1.099, 1 / 0.45);
  }
}

inline double GammaCorrectionMethod2(double val) {
  const double normed_val = val / 255.0;
  if (normed_val < 0.018) {
    return 4.5 * normed_val;
  } else {
    return 1.099 * std::pow(normed_val, 0.45) - 0.099;
  }
}

inline void RGB2YUV(uint8_t r, uint8_t g, uint8_t b,
                    double* y, double* u, double* v) {
  *y = r * 0.299 + g * 0.587 + b * 0.114;
  *u = 0.492 * (b - *y);
  *v = 0.877 * (r - *y);
}

inline void YUV2RGB(uint8_t y, uint8_t u, uint8_t v,
                    double* r, double* g, double* b) {
  *r = y + 1.140 * v;
  *g = y - 0.395 * u - 0.581 * v;
  *b = y + 2.032 * u;
}

inline void RGB2YIQ(uint8_t r, uint8_t g, uint8_t b,
                    double* y, double* i, double* q) {
  *y = r * 0.299 + g * 0.587 + b * 0.114;
  *i = r * 0.596 - g * 0.275 - b * 0.321;
  *q = r * 0.212 - g * 0.523 + b * 0.311;
}

inline void YIQ2RGB(uint8_t y, uint8_t i, uint8_t q,
                    double* r, double* g, double* b) {
  *r = y + 0.956 * i + 0.621 * q;
  *g = y - 0.272 * i - 0.647 * q;
  *b = y - 1.107 * i + 1.704 * q;
}

inline void RGB2YCbCr601(uint8_t r, uint8_t g, uint8_t b,
                         double* y, double* cb, double* cr) {
  *y  = r *  0.257 + g * 0.504 + b * 0.098 + 16;
  *cb = r * -0.148 - g * 0.291 + b * 0.439 + 128;
  *cr = r *  0.439 - g * 0.368 - b * 0.071 + 128;
}

inline void RGB2YCbCr709(uint8_t r, uint8_t g, uint8_t b,
                         double* y, double* cb, double* cr) {
  *y  = r *  0.183 + g * 0.614 + b * 0.062 + 16;
  *cb = r * -0.101 - g * 0.338 + b * 0.439 + 128;
  *cr = r *  0.439 - g * 0.399 - b * 0.040 + 128;
}

LaplacianFilter GetLaplacianFilter1() {
  LaplacianFilter filter;
  filter.filter = new double[3 * 3] {
    0,  1, 0,
    1, -4, 1,
    0,  1, 0
  };
  filter.size = 0;
  return filter;
}

LaplacianFilter GetLaplacianFilter2() {
  LaplacianFilter filter;
  filter.filter = new double[3 * 3] {
    1,   4, 1,
    4, -20, 4,
    1,   4, 1
  };
  filter.size = 0;
  return filter;
}

LaplacianFilter GetLaplacianFilter3() {
  LaplacianFilter filter;
  filter.filter = new double[3 * 3] {
    -1, -1, -1,
    -1,  8, -1,
    -1, -1, -1
  };
  filter.size = 0;
  return filter;
}

} /* end of kernel namespace */
} /* end of ip namespace */

// gamma correction kernels
gamma_correction_method gc_method1 = ip::kernel::GammaCorrectionMethod1;
gamma_correction_method gc_method2 = ip::kernel::GammaCorrectionMethod2;
// convertion kernels
// YUV
colorspace_convert rgb_to_yuv = ip::kernel::RGB2YUV;
colorspace_convert yuv_to_rgb = ip::kernel::YUV2RGB;
// YIQ
colorspace_convert rgb_to_yiq = ip::kernel::RGB2YIQ;
colorspace_convert yiq_to_rgb = ip::kernel::YIQ2RGB;
// YCbCr
colorspace_convert rgb_to_ycbcr601 = ip::kernel::RGB2YCbCr601;
colorspace_convert rgb_to_ycbcr709 = ip::kernel::RGB2YCbCr709;
// laplacian filter
LaplacianFilter laplacian_filter1 = ip::kernel::GetLaplacianFilter1();
LaplacianFilter laplacian_filter2 = ip::kernel::GetLaplacianFilter2();
LaplacianFilter laplacian_filter3 = ip::kernel::GetLaplacianFilter3();
