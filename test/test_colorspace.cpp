#define BOOST_TEST_MODULE Color Space Unit Test
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <string>

#include "common.h"
#include "image_processing.h"

using std::cout;
using std::string;
using cv::Mat;

const string IMAGE_PATH = "lena.png";
const Mat image = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);

BOOST_AUTO_TEST_CASE(TestRGBToGray) {
  BOOST_CHECK(!image.empty());
  BOOST_CHECK(image.channels() == 3);

  Mat rgb_image;
  cv::cvtColor(image, rgb_image, CV_BGR2RGB);

  uint8_t* data = nullptr;
  int w = 0;
  int h = 0;
  int d = 0;
  ip::test::ExtractCVMat(rgb_image, &data, &w, &h, &d);

  // testing function result
  uint8_t* output_gray = new uint8_t[w * h];
  double weights[3] = {0.299, 0.587, 0.114};
  ip_rgb_to_gray(w, h, data, weights, output_gray);
  Mat unit_test_gray;
  ip::test::ConvertToCVMat(output_gray, w, h, 1, &unit_test_gray);

  // OpenCV result
  Mat gray;
  cv::cvtColor(image, gray, CV_BGR2GRAY);

  BOOST_CHECK(ip::test::IsSame(unit_test_gray, gray, 1));

  delete[] data;
  delete[] output_gray;
}

BOOST_AUTO_TEST_CASE(TestHistogram) {
  BOOST_CHECK(!image.empty());
  BOOST_CHECK(image.channels() == 3);

  Mat gray_image;
  cv::cvtColor(image, gray_image, CV_BGR2GRAY);

  uint8_t* data = nullptr;
  int w = 0;
  int h = 0;
  int d = 0;
  ip::test::ExtractCVMat(gray_image, &data, &w, &h, &d);

  // testing function result
  const int num_bins = 256;
  int* output_histogram = new int[num_bins];
  ip_histogram(w, h, d, data, num_bins, output_histogram);

  // OpenCV result
  Mat hist;
  float range[] = {0, 256};
  const float* hist_range = {range};
  cv::calcHist(&gray_image, 1, nullptr, Mat(), hist, 1, &num_bins, &hist_range);
  BOOST_CHECK(hist.type() == CV_32SC1 || hist.type() == CV_32FC1);
  const int hist_size = hist.rows * hist.cols * hist.channels();
  BOOST_CHECK(hist_size == num_bins);
  int* hist_result = new int[hist_size];
  for (int i = 0 ; i < hist.rows * hist.cols * hist.channels() ; ++i) {
    hist_result[i] = static_cast<int>(hist.ptr<float>()[i]);
  }

  BOOST_CHECK(ip::test::IsSame(output_histogram, hist_result, num_bins, 10));
}

BOOST_AUTO_TEST_CASE(TestGammaCorrection) {
}

BOOST_AUTO_TEST_CASE(TestRGBToYUV) {
}
