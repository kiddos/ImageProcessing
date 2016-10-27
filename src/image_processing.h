#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// color space conversion
// compute rgb to gray
void ip_rgb_to_gray(const int w, const int h,
                    const uint8_t* const image_data,
                    const double* weights,
                    uint8_t* const output);
// compute image histogram
void ip_histogram(const int w, const int h, const int d,
                  const uint8_t* const image_data,
                  const int bin_size, int* const histogram_bin);
// rgb gamma correction
typedef double (*gamma_correction_method)(double);
void ip_rgb_gamma_correction(gamma_correction_method method,
                             const int w, const int h, const int d,
                             const uint8_t* const image_data,
                             uint8_t* const output);
// common types of gamma correction
extern gamma_correction_method gc_method1;
extern gamma_correction_method gc_method2;
// convert to different color space and convert back
// NOTE: assumes that image are all 3 channels deep
typedef void (*colorspace_convert)(uint8_t, uint8_t, uint8_t,
                                   double*, double*, double*);
void ip_convert_to(colorspace_convert convert,
                   const int w, const int h,
                   const uint8_t* const image_data,
                   uint8_t* const output);
// common types of convertions
// YUV
extern colorspace_convert rgb_to_yuv;
extern colorspace_convert yuv_to_rgb;
// YIQ
extern colorspace_convert rgb_to_yiq;
extern colorspace_convert yiq_to_rgb;
// YCbCr
extern colorspace_convert rgb_to_ycbcr601;
extern colorspace_convert rgb_to_ycbcr709;
// image feature
enum PADDING {
  PADDING_SAME, PADDING_ZERO
};
typedef double (*LBPFeatureType) (const int, const int, const int, const int,
                                  const enum PADDING, const int, const int,
                                  const uint8_t* const);
extern LBPFeatureType lbp;
extern LBPFeatureType tlbp;
extern LBPFeatureType clbp;
extern LBPFeatureType cslbp;
extern LBPFeatureType tplbp;
// lbp (local binary pattern)
void ip_lbp(LBPFeatureType feature,
            const enum PADDING padding,
            const int kernel_size, const double start,
            const int w, const int h, const uint8_t* image_data,
            uint8_t* const output);
typedef double (*LTPFeatureType) (const int, const int, const double,
                                  const int, const int, const PADDING,
                                  const int, const int,
                                  const uint8_t* const);
extern LTPFeatureType ltp;
extern LTPFeatureType csltp;
extern LTPFeatureType tpltp;
// ltp (local ternary pattern)
void ip_ltp(LTPFeatureType feature,
            const enum PADDING padding,
            const int kernel_size, const double start, const double threshold,
            const int w, const int h, const uint8_t* const image_data,
            double* const output);
// lzp (local zigzag pattern)
typedef double (*LZPFeatureType) (const int* const, const int,
                                  const int, const int, const PADDING,
                                  const int, const int,
                                  const uint8_t* const);
void ip_lzp(LZPFeatureType feature,
            const int* const order, const int order_size,
            const enum PADDING padding,
            const int w, const int h, const uint8_t* const image_data,
            uint8_t* const output);
extern LZPFeatureType lzp;
extern LZPFeatureType tplzp;
// ldp (local direction pattern)
typedef struct {
  double* dir;
  int w;
  int h;
} LDPDirection;
void ip_ldp(LDPDirection direction,
            const enum PADDING padding,
            const int w, const int h, const uint8_t* const image_data,
            uint8_t* const output);
extern LDPDirection ldp_east;
extern LDPDirection ldp_north_east;
extern LDPDirection ldp_north;
extern LDPDirection ldp_north_west;
extern LDPDirection ldp_west;
extern LDPDirection ldp_south_west;
extern LDPDirection ldp_south;
extern LDPDirection ldp_south_east;
// edge detection
// sobel derivative
void ip_sobel(const int kernel_size,
              const int w, const int h, const uint8_t* const image_data,
              uint8_t* const output);
// canny edge detection
void ip_canny(const int kernel_size,
              const int w, const int h, const uint8_t* const image_data,
              const int lower_threshold, const int higher_threshold,
              uint8_t* const output);
// laplacian
typedef struct {
  double* filter;
  int size;
} LaplacianFilter;
void ip_laplacian(LaplacianFilter filter,
                  const enum PADDING padding,
                  const int w, const int h, const uint8_t* const image_data,
                  uint8_t* const output);
extern LaplacianFilter laplacian_filter1;
extern LaplacianFilter laplacian_filter2;
extern LaplacianFilter laplacian_filter3;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: IMAGE_PROCESSING_H */
