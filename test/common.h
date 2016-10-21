#ifndef COMMON_H
#define COMMON_H

#include <opencv2/core.hpp>
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <cmath>
#include <sys/time.h>

namespace ip {
namespace test {

class Timer {
 public:
  Timer() = default;

  void start() {
    gettimeofday(&start_, nullptr);
  }
  void stop() {
    gettimeofday(&end_, nullptr);
  }
  void output_time_passed() {
    std::cout << "time elapsed: "
        << end_.tv_sec - start_.tv_sec + 1e-6 * (end_.tv_usec - start_.tv_usec)
        << " sec\n";
  }

 private:
  struct timeval start_, end_;
};

int get_random_size(int base_size) {
  std::mt19937 generator(
      std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> distribution(base_size / 2, base_size);
  return base_size + distribution(generator);
}

template <typename DType>
void set_data_value(DType* data, const int size, DType val) {
  for (int i = 0 ; i < size ; ++i) {
    data[i] = val;
  }
}

template <typename DType>
void print_2d_data(DType* data, const int w, const int h) {
  for (int i = 0 ; i < h ; ++i) {
    for (int j = 0 ; j < w ; ++j) {
      std::cout << std::setw(4) << data[j * h + i] << " ";
    }
    std::cout << '\n';
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
              image.at<cv::Vec3b>(i, j)[k];
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
bool IsSame(const DType* data1, const DType* data2,
            const int data_size, const double threshold) {
  for (int i = 0 ; i < data_size ; ++i) {
    if (std::fabs(data1[i] - data2[i]) > threshold) {
      std::cout << i + 1
          << ". data1: " << data1[i] << " | data2: " << data2[i] << '\n';
      return false;
    }
  }
  return true;
}

bool IsSame(const cv::Mat& image1, const cv::Mat& image2,
            const double threshold) {
  if (image1.rows != image2.rows) return false;
  if (image1.cols != image2.cols) return false;

  for (int i = 0 ; i < image1.rows ; ++i) {
    for (int j = 0 ; j < image1.cols ; ++j) {
      for (int k = 0 ; k < image1.channels() ; ++k) {
        if (std::fabs(image1.ptr<uint8_t>(i, j)[k] -
                      image2.ptr<uint8_t>(i, j)[k]) > threshold) {
          std::cout << "(" << i << ", " << j << "): ";
          std::cout << static_cast<int>(image1.ptr<uint8_t>(i, j)[k])
              << ", " << static_cast<int>(image2.ptr<uint8_t>(i, j)[k])
              << '\n';
          return false;
        }
      }
    }
  }
  return true;
}

} /* end of test namespace */
} /* end of ip namespace */

#endif /* end of include guard: COMMON_H */
