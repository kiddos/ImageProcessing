#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <thread>

#include "image_processing.h"
#include "common.h"

using std::cout;
using std::string;
using std::thread;
using std::chrono::high_resolution_clock;
using cv::Mat;

class Display {
 public:
  Display(const string& path)
      : kernel_size_(3), threshold_(10), alpha_(3), range_(1), s_(8),
        start_(0), image_data_(nullptr), output_feature_(nullptr) {
    image_ = cv::imread(path, cv::IMREAD_GRAYSCALE);
    if (!image_.empty()) {
      ip::demo::ExtractCVMat(image_, &image_data_, &w_, &h_, &d_);
      if (d_ == 1) {
        output_feature_ = new double[w_ * h_];
        output_image_ = new uint8_t[w_ * h_];
      }
    }
  }

  ~Display() {
    updating_ = false;
    if (update_task_.joinable()) update_task_.join();
    if (image_data_) delete[] image_data_;
    if (output_feature_) delete[] output_feature_;
  }

  void Show(const string name) {
    if (image_data_) {
      window_name_ = name;
      cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
      cv::createTrackbar("Kernel Size:", name, &kernel_size_, 17, nullptr);
      cv::createTrackbar("Threshold:", name, &threshold_, 255, nullptr);
      cv::createTrackbar("Alpha:", name, &alpha_, 7, nullptr);
      cv::createTrackbar("Range:", name, &range_, 10, nullptr);
      cv::createTrackbar("Surround:", name, &s_, 32, nullptr);
      cv::imshow("Input", image_);
      updating_ = true;
      update_task_ = thread([this] { Loop(); });
    }
  }

  void OutputToFile(const string& path) {
    if (output_feature_)
      ip::demo::OutputCSV(output_feature_, w_ * h_, path);
  }

 private:
  void Loop() {
    while (updating_) {
      auto start = high_resolution_clock::now();
      ip_three_patch_ltp(csltp, PADDING_SAME,
                         kernel_size_, start_, threshold_,
                         alpha_, range_, s_, w_, h_,
                         image_data_, output_feature_);
      auto end = high_resolution_clock::now();
      ip::demo::NormalizeForDisplay(output_feature_, w_ * h_, output_image_);
      cout << "\rComputation took "
          << std::chrono::duration_cast<std::chrono::milliseconds>(
          end - start).count() << " ms" << std::endl;

      output_display_ = Mat(h_, w_, CV_8UC1, output_image_);
      cv::imshow(window_name_, output_display_);
      char key = cv::waitKey(10);
      if (key == 'q' || key == 10 || key == 27) updating_ = false;
    }
  }

  int w_, h_, d_, kernel_size_, threshold_, alpha_, range_, s_;
  double start_;

  uint8_t* image_data_;
  uint8_t* output_image_;
  double* output_feature_;
  string window_name_;
  bool updating_;
  thread update_task_;

  Mat image_, output_display_;
};

int main(int argc, char** argv) {
  string image_path;
  if (argc != 2) {
    image_path = "lena.png";
  } else {
    image_path = argv[1];
  }

  Display display(image_path);
  display.Show("Output");
  cv::waitKey(0);
  display.OutputToFile("output.csv");
  return 0;
}
