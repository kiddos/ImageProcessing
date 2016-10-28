#ifndef COMMON_H
#define COMMON_H

#include <opencv2/core.hpp>
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "ip/util/math_function.h"

namespace ip {
namespace demo {

template <typename DType>
void ChooseRandomPoint(const int w, const int h, const DType* const image_data,
                       const int ow, const int oh, DType* const output,
                       int* const oi, int* const oj) {
  auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> ri(0, w - ow);
  std::uniform_int_distribution<int> rj(0, h - oh);

  *oi = ri(generator);
  *oj = rj(generator);

  for (int i = 0 ; i < oh ; ++i) {
    for (int j = 0 ; j < ow ; ++j) {
      output[i * ow + j] = image_data[(*oi + i) * w + *oj + j];
    }
  }
}

template <typename DType>
void ExtractCVMat(const cv::Mat& image, DType** data,
                  int* w, int* h, int* d) {
  if (!image.empty()) {
    // allocate the memory for output
    *w = image.cols;
    *h = image.rows;
    *d = image.channels();
    *data = new DType[image.rows * image.cols * image.channels()];

    const int step = image.rows * image.channels();
    for (int i = 0 ; i < image.rows ; ++i) {
      for (int j = 0 ; j < image.cols ; ++j) {
        for (int k = 0 ; k < image.channels() ; ++k) {
          (*data)[i * step + j * image.channels() + k] =
              image.ptr<uchar>(i, j)[k];
        }
      }
    }
  }
}

template <typename DType>
void ConvertToCVMat(const DType* const data, int w, int h, int d,
                    cv::Mat* image) {
  if (w > 0 && h > 0) {
    *image = cv::Mat_<DType>(h, w);

    for (int i = 0 ; i < h ; ++i) {
      for (int j = 0 ; j < w ; ++j) {
        for (int k = 0 ; k < d ; ++k) {
          image->ptr<DType>(i, j)[k] = data[i * w * d + j * d + k];
        }
      }
    }
  }
}

template <typename DType>
void NormalizeForDisplay(const DType* const data, const int data_size,
                         uint8_t* const output_data) {
  double maxval = 0, minval = std::numeric_limits<double>::max();
  for (int i = 0 ; i < data_size ; ++i) {
    if (data[i] > maxval) maxval = data[i];
    if (data[i] < minval) minval = data[i];
  }
  std::cout << "max value: " << maxval << '\n';
  std::cout << "min value: " << minval << '\n';
  const double diff = maxval - minval;
  for (int i = 0 ; i < data_size ; ++i) {
    const double output_val = static_cast<double>(data[i]) / diff * 255.0;
    output_data[i] = ip::math::clamp<uint8_t>(output_val);
  }
}

template <typename DType>
void OutputCSV(const DType* const data, const int data_size,
               const std::string output_path) {
  std::ofstream output_file(output_path, std::ios::out);
  for (int i = 0 ; i < data_size ; ++i) {
    output_file << data[i] << '\n';
  }
}

} /* end of demo namespace */
} /* end of ip namespace */

#endif /* end of include guard: COMMON_H */
