#define BOOST_TEST_MODULE Util Unit Test
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <sys/time.h>

#include "ip/util/math_function.h"

using std::cout;
using boost::unit_test::test_suite;

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
      cout << std::setw(4) << data[j * h + i] << " ";
    }
    cout << '\n';
  }
}

void TestConvolution2d(const int scale, const int padding, const int stride) {
  const int a_width = get_random_size(scale);
  const int a_height = get_random_size(scale);
  const int b_width = get_random_size(ip::math::min(scale / 2, 4));
  const int b_height = get_random_size(ip::math::min(scale / 2, 4));
  cout << "A | width: " << a_width << " | height: " << a_height << '\n';
  cout << "B | width: " << b_width << " | height: " << b_height << '\n';
  cout << "[Test Convolution...]: (" <<  a_width << "," << a_height;
  cout << ")(" << b_width << "," << b_height << ')';
  cout << " | initializing data...";
  // generate all ones
  float* a = new float[a_width * a_height];
  float* b = new float[b_width * b_height];
  const int output_width = (a_width - b_width + 2 * padding) / stride + 1;
  const int output_height = (a_height - b_height + 2 * padding) / stride + 1;
  float* c = new float[output_width * output_height];
  set_data_value(a, a_width * a_height, 1.0f);
  set_data_value(b, b_width * b_height, 1.0f);
  cout << "done.\n";

  Timer t;
  t.start();
  ip::math::convolution2d(padding, stride,
                          a_width, a_height, a,
                          b_width, b_height, b,
                          output_width, output_height, c);
  t.stop();
  t.output_time_passed();

  for (int i = 0 ; i < output_width ; ++i) {
    for (int j = 0 ; j < output_height ; ++j) {
      float val = 0;
      for (int k = 0 ; k < b_width ; ++k) {
        for (int l = 0 ; l < b_height ; ++l) {
          if (i * stride + k < padding || i * stride + k >= a_width + padding)
            continue;
          if (j * stride + l < padding || j * stride + l >= a_height + padding)
            continue;
          ++val;
        }
      }
      const float expected = val;
      bool check = (expected == c[i * output_height + j]);
      if (!check) {
        cout << "i: " << i << " | j: " << j
            << " | expected: " << expected << " | c: "
            << c[i * output_height + j] << '\n';
        print_2d_data(c, output_width, output_height);
        delete[] a;
        delete[] b;
        delete[] c;
        exit(0);
      }
      BOOST_CHECK(check);
    }
  }

  delete[] a;
  delete[] b;
  delete[] c;
}

} /* end of test namespace */
} /* end of ip namespace */

BOOST_AUTO_TEST_CASE(TestMinMaxFunction) {
  double a = 100;
  uint8_t b = 30;

  BOOST_CHECK(ip::math::max(a, b) == a);
  BOOST_CHECK(ip::math::min(b, a) == b);
}

BOOST_AUTO_TEST_CASE(TestClampFunction) {
  double a = 256;
  BOOST_CHECK(ip::math::clamp<uint8_t>(a) == 255);

  double b = -256;
  BOOST_CHECK(ip::math::clamp<int8_t>(b) == -128);

  double c = 256;
  BOOST_CHECK(ip::math::clamp<int>(c) == 256);
}

BOOST_AUTO_TEST_CASE(TestConvolution2d) {
  // small scale
  ip::test::TestConvolution2d(4, 1, 1);
  ip::test::TestConvolution2d(4, 1, 2);
  ip::test::TestConvolution2d(4, 1, 3);
  // medium scale
  ip::test::TestConvolution2d(16, 1, 1);
  ip::test::TestConvolution2d(16, 1, 2);
  ip::test::TestConvolution2d(16, 2, 3);
  ip::test::TestConvolution2d(64, 1, 1);
  ip::test::TestConvolution2d(64, 1, 2);
  ip::test::TestConvolution2d(64, 2, 3);
  ip::test::TestConvolution2d(256, 1, 1);
  ip::test::TestConvolution2d(256, 1, 2);
  ip::test::TestConvolution2d(256, 2, 3);
  // large scale
  ip::test::TestConvolution2d(1024, 1, 1);
  ip::test::TestConvolution2d(1024, 1, 2);
  ip::test::TestConvolution2d(1024, 2, 3);
}
