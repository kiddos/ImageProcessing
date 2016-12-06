#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <functional>
#include <vector>

using std::cout;
using std::string;
using std::vector;
using cv::Mat;
using cv::Point;
using cv::Size;
using cv::Scalar;
using cv::VideoCapture;

class Demo {
 public:
  Demo(int canny_threshold, double canny_ratio, int sobel_size)
      : camera_(0), canny_(480, 640, CV_8UC1),
        histogram_(480, 768 * 2, CV_8UC3),
        canny_threshold_low_(canny_threshold),
        canny_threshold_high_(canny_threshold * canny_ratio),
        canny_threshold_ratio_(canny_ratio),
        canny_sobel_size_(sobel_size) {
    for (int i = 0 ; i < 3 ; ++i) {
      vector<long> bins;
      for (int j = 0 ; j < 256 ; ++j) {
        bins.push_back(0);
      }
      hist_bins_.push_back(bins);
    }
  }

  static void CannyThresholdChanged(int threshold, void* ptr) {
    Demo* demo = static_cast<Demo*>(ptr);
    demo->canny_threshold_high_ = demo->canny_threshold_low_ +
        threshold * demo->canny_threshold_ratio_;
  }

  static void CannyThresholdRatioChanged(int threshold_ratio, void* ptr) {
    Demo* demo = static_cast<Demo*>(ptr);
    demo->canny_threshold_ratio_ = threshold_ratio;
    demo->canny_threshold_high_ = demo->canny_threshold_low_ *
        (1 + threshold_ratio);
  }

  static void CannySobelSizeChanged(int sobel_size, void* ptr) {
    Demo* demo = static_cast<Demo*>(ptr);
    demo->canny_sobel_size_ = sobel_size * 2 + 1;
  }

  bool CheckCamera() {
    if (!camera_.isOpened()) {
      cout << "fail to open camera\n";
      return false;
    }
    return true;
  }

  void SetupWindow() {
    cv::namedWindow(ORIGINAL_WINDOW);
    cv::namedWindow(CANNY_WINDOW);
    cv::namedWindow(HISTOGRAM_WINDOW);
    cv::moveWindow(CANNY_WINDOW, 800, 0);
    cv::moveWindow(HISTOGRAM_WINDOW, 0, 600);

    cv::createTrackbar("Lower Threshold:", CANNY_WINDOW,
                       &canny_threshold_low_, 100,
                       CannyThresholdChanged, this);
    cv::createTrackbar("Threshold Ratio:", CANNY_WINDOW,
                       &canny_threshold_ratio_, 100,
                       CannyThresholdRatioChanged, this);
    int size = 3;
    cv::createTrackbar("Sobel Size:", CANNY_WINDOW,
                       &size, 4,
                       CannySobelSizeChanged, this);
  }

  void ClearHistogram() {
    for (int i = 0 ; i < 3 ; ++i) {
      for (int j = 0 ; j < 256 ; ++j) {
        hist_bins_[i][j] = 0;
      }
    }
  }

  int ComputeHistogram(Mat smoothed) {
    int max_bin = 0;
    for (int i = 0 ; i < smoothed.rows ; ++i) {
      for (int j = 0 ; j < smoothed.cols ; ++j) {
        cv::Vec3b pixel = smoothed.at<cv::Vec3b>(i, j);
        for (int k = 0 ; k < 3 ; ++k) {
          hist_bins_[k][pixel[k]] ++;
          if (hist_bins_[k][pixel[k]] > max_bin) {
            max_bin = hist_bins_[k][pixel[k]];
          }
        }
      }
    }
    return max_bin;
  }

  void DrawHistogram(Mat hist_display, int max_bin) {
    if (hist_display.cols % (256 * 3) != 0) {
      return;
    }
    int scale = hist_display.cols / (256 * 3);

    // zero out the histogram display
    hist_display = Mat::zeros(hist_display.rows, hist_display.cols,
                              hist_display.type());

    int base_line = hist_display.rows - 16;
    // draw the base line and text
    cv::putText(hist_display, "0", Point(0, base_line),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255));
    cv::putText(hist_display, "255",
                Point(hist_display.cols - 50, base_line),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255));
    base_line -= 20;
    cv::rectangle(hist_display, Point(0, base_line),
                  Point(hist_display.cols, base_line),
                  Scalar(255, 255, 255));

    for (int i = 0 ; i < 256 ; ++i) {
      for (int j = 0 ; j < 3 ; ++j) {
        int bin_value = hist_bins_[j][i];
        double display_value = static_cast<double>(bin_value) /
            max_bin * (hist_display.rows - 20);
        Point p2((i * 3 + j) * scale, base_line);
        Point p1(p2.x - 1, p2.y - display_value);
        Scalar color(0, 0, 0);
        color[j] = 255;
        cv::rectangle(hist_display, p1, p2, color);
      }
    }
  }

  void MainLoop() {
    while(true) {
      camera_.read(image_);

      // canny
      canny_sobel_size_ = std::min(std::max(canny_sobel_size_, 3), 7);
      cv::Canny(image_, canny_, canny_threshold_low_,
                canny_threshold_high_, canny_sobel_size_);

      // histogram
      cv::GaussianBlur(image_, smoothed_, Size(5, 5), 1, 1);
      ClearHistogram();
      int max_bin = ComputeHistogram(smoothed_);
      DrawHistogram(histogram_, max_bin);

      cv::imshow(ORIGINAL_WINDOW, image_);
      cv::imshow(CANNY_WINDOW, canny_);
      cv::imshow(HISTOGRAM_WINDOW, histogram_);
      char key = cv::waitKey(10);
      if (key == 'q' || key == 10 || key == 27) {
        break;
      }
    }
  }

 private:
  const string ORIGINAL_WINDOW = "Original Image";
  const string CANNY_WINDOW = "Canny";
  const string HISTOGRAM_WINDOW = "Histogram";

  VideoCapture camera_;
  Mat image_, canny_, smoothed_, histogram_;
  int canny_threshold_low_, canny_threshold_high_;
  int canny_threshold_ratio_, canny_sobel_size_;
  vector<vector<long>> hist_bins_;
};

int main(void) {
  Demo demo(1, 0.3, 3);
  if (!demo.CheckCamera()) {
    return -1;
  }

  demo.SetupWindow();
  demo.MainLoop();
  return 0;
}
