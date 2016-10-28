#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

#include "image_processing.h"
#include "common.h"

using std::cout;
using std::string;
using std::vector;
using cv::Mat;

void DisplayCSLTPResult(const int w, const int h, const uint8_t* const parts,
                        const double threshold, vector<double>& values) {
  for (int k = 0 ; k < 8 ; ++k) {
    int cx = 2 + static_cast<int>(std::round(std::cos(M_PI / 4 * k)));
    int cy = 2 + static_cast<int>(std::round(std::sin(M_PI / 4 * k)));

    cout << "cx: " << cx << " | cy: " << cy << '\n';
    for (int i = cy - 1 ; i < h - 2 + cy - 1 ; ++i) {
      for (int j = cx - 1 ; j < w - 2 + cx - 1; ++j) {
        cout << std::setw(4) <<
            static_cast<int>(parts[i * w + j]);
      }
      cout << '\n';
    }
    for (int i = 0 ; i < 8 ; ++i) {
      const double dx1 = std::cos(M_PI / 4 * i);
      const double dy1 = std::sin(M_PI / 4 * i);
      const int newx1 = static_cast<int>(std::round(cx + dx1));
      const int newy1 = static_cast<int>(std::round(cy + dy1));
      const int val1 = static_cast<int>(parts[newy1 * w + newx1]);

      const double dx2 = std::cos(M_PI / 4 * (i + 4));
      const double dy2 = std::sin(M_PI / 4 * (i + 4));
      const int newx2 = static_cast<int>(std::round(cx + dx2));
      const int newy2 = static_cast<int>(std::round(cy + dy2));
      const int val2 = static_cast<int>(parts[newy2 * w + newx2]);

      cout << "(" << val1 << " vs " << val2 << ")";
      if (val1 >= val2 + threshold) {
        cout << "2*3^" << i;
      } else if (val1 >= val2 - threshold &&
                  val1 < val2 + threshold) {
        cout << "3^" << i;
      } else {
        cout << "0";
      }
      if (i != 7) cout << " + ";
    }
    double ltp_result = csltp(3, 0, threshold, cx, cy,
                              PADDING_ZERO, w, h, parts);
    values.push_back(ltp_result);
    cout << " = " << ltp_result << '\n';
  }
}

void DisplayTPCSLTP(const uint8_t* const parts, const int kernel_size,
                    vector<double>& values, const int w, const int threshold) {
  const int s = values.size();
  for (int i = 0 ; i < s ; ++i) {
    const double val1 = values[i % s];
    const double val2 = values[(i + w) % s];
    cout << "(" << val1 << "," << val2 << ")";
    if (val1 >= val2 + threshold) {
      cout << "2*3^" << i;
    } else if (val1 >= val2 - threshold &&
               val1 < val2 + threshold) {
      cout << "3^" << i;
    } else {
      cout << "0";
    }
    if (i != s -1) cout << " + ";
  }
  double final_result = tpcsltp(kernel_size, 0, threshold,
                                2, 2, PADDING_ZERO, 5, 5, parts);
  cout << " = " << final_result << '\n';
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

  const int rect_size = 5;
  uint8_t* output = new uint8_t[rect_size * rect_size];
  int oi = 0, oj = 0;
  ip::demo::ChooseRandomPoint(w, h, image_data,
                              rect_size, rect_size, output, &oi, &oj);
  cout << "choosen row: " << oi << '\n';
  cout << "choosen col: " << oj << '\n';
  cout << "parts:\n";
  for (int i = 0 ; i < rect_size ; ++i) {
    for (int j = 0 ; j < rect_size ; ++j) {
      cout << std::setw(4)
          << static_cast<int>(output[i * rect_size + j]);
    }
    cout << '\n';
  }
  vector<double> values;
  DisplayCSLTPResult(rect_size, rect_size, output, 10, values);
  DisplayTPCSLTP(output, 3, values, 3, 10);

  delete[] image_data;
  delete[] output;
  return 0;
}
