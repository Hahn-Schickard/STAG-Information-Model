#include "DataVariant.hpp"

#include "gtest/gtest.h"
#include <chrono>
#include <memory>
#include <string>
#include <thread>

using namespace Information_Model;
using namespace std;

// NOLINTNEXTLINE
TEST(DateTimeTests, isEqual) {
  DateTime dt1 = DateTime();
  DateTime dt2 = dt1;
  EXPECT_TRUE((dt1 == dt2))
      << "expected: " << dt1.toString() << "==" << dt2.toString() << endl;
  EXPECT_FALSE((dt1 != dt2))
      << "expected: " << dt1.toString() << "!=" << dt2.toString() << endl;
}

// NOLINTNEXTLINE
TEST(DateTimeTests, isNotEqual) {
  DateTime dt1 = DateTime();
  this_thread::sleep_for(1s);
  DateTime dt2 = DateTime();
  EXPECT_FALSE((dt1 == dt2))
      << "expected: " << dt1.toString() << "!=" << dt2.toString() << endl;
  EXPECT_TRUE((dt1 != dt2))
      << "expected: " << dt1.toString() << "!=" << dt2.toString() << endl;
}

// NOLINTNEXTLINE
TEST(DateTimeTests, isLess) {
  DateTime dt1 = DateTime();
  this_thread::sleep_for(1s);
  DateTime dt2 = DateTime();
  EXPECT_FALSE((dt1 == dt2))
      << "expected: " << dt1.toString() << "!=" << dt2.toString() << endl;
  EXPECT_TRUE((dt1 < dt2)) << "expected: " << dt1.toString() << "<"
                           << dt2.toString() << endl;
}

// NOLINTNEXTLINE
TEST(DateTimeTests, isMore) {
  DateTime dt1 = DateTime();
  this_thread::sleep_for(1s);
  DateTime dt2 = DateTime();
  EXPECT_FALSE((dt1 == dt2))
      << "expected: " << dt1.toString() << "!=" << dt2.toString() << endl;
  EXPECT_TRUE((dt2 > dt1)) << "expected: " << dt2.toString() << ">"
                           << dt1.toString() << endl;
}
