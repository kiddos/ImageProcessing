#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <cmath>

#include "image_processing.h"
#include "common.h"

using std::cout;
using std::string;
using cv::Mat;

void Display(const uint8_t* const image_data,
             const int w, const int h,
             const int kernel_size, const double start,
             const double threshold,
             bool output_csv, const string output_path) {
  double* output_feature = new double[w * h];
  ip_ltp(tpltp, PADDING_SAME, kernel_size, start, threshold,
         w, h, image_data, output_feature);
  uint8_t* output_image = new uint8_t[w * h];
  ip::demo::NormalizeForDisplay(output_feature, w * h, output_image);

  if (output_csv) {
    ip::demo::OutputCSV(output_feature, w * h, output_path);
  }

  Mat output_display(h, w, CV_8UC1, output_image);
  cv::imshow(output_path, output_display);
  delete[] output_feature;
  delete[] output_image;
}

int main(int argc, char** argv) {
  string image_path;
  if (argc != 2) {
    image_path = "lena.png";
  } else {
    image_path = argv[1];
  }
  Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

  uint8_t* image_data = nullptr;
  int w = 0, h = 0, d = 0;
  ip::demo::ExtractCVMat(image, &image_data, &w, &h, &d);
  if (d != 1) {
    cout << "depth not 1\n";
    return -1;
  }

  cout << "start changing kernel size..." << std::endl;
  Display(image_data, w, h, 3, 0, 10, true, "tpcsltp3x3.csv");
  Display(image_data, w, h, 5, 0, 10, false, "tpcsltp5x5");
  Display(image_data, w, h, 7, 0, 10, false, "tpcsltp7x7");
  cv::imshow("input", image);
  cv::waitKey(0);

  cout << "start changing threshold..." << std::endl;
  Display(image_data, w, h, 3, 0, 10, true, "tpcsltp3x3-10.csv");
  Display(image_data, w, h, 3, 0, 50, false, "tpcsltp3x3-50.csv");
  Display(image_data, w, h, 3, 0, 100, false, "tpcsltp3x3-100.csv");
  cv::waitKey(0);

  cout << "start changing start point..." << std::endl;
  Display(image_data, w, h, 3, 0, 10, true, "tpcsltp3x3-start0.csv");
  Display(image_data, w, h, 3, M_PI / 2, 10, false,
          "tpcsltp3x3-start-pi2.csv");
  Display(image_data, w, h, 3, M_PI, 10, false,
          "tpcsltp3x3-10-start-pi.csv");
  cv::waitKey(0);

  delete[] image_data;
  return 0;
}
