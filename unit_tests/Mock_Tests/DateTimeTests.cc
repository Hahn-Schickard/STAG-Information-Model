#include "DataVariant.hpp"

#include "gtest/gtest.h"
#include <chrono>
#include <memory>
#include <string>
#include <thread>

using namespace Information_Model;
using namespace std;

TEST(DateTimeTests, isEqual) {
  DateTime dt1 = DateTime();
  DateTime dt2 = dt1;
  EXPECT_TRUE((dt1 == dt2 ? true : false))
      << "expected: " << dt1.toString() << "==" << dt2.toString() << endl;
  EXPECT_FALSE((dt1 != dt2 ? true : false))
      << "expected: " << dt1.toString() << "!=" << dt2.toString() << endl;
}

TEST(DateTimeTests, isNotEqual) {
  DateTime dt1 = DateTime();
  this_thread::sleep_for(1s);
  DateTime dt2 = DateTime();
  EXPECT_FALSE((dt1 == dt2 ? true : false))
      << "expected: " << dt1.toString() << "!=" << dt2.toString() << endl;
  EXPECT_TRUE((dt1 != dt2 ? true : false))
      << "expected: " << dt1.toString() << "!=" << dt2.toString() << endl;
}

TEST(DateTimeTests, isLess) {
  DateTime dt1 = DateTime();
  this_thread::sleep_for(1s);
  DateTime dt2 = DateTime();
  EXPECT_FALSE((dt1 == dt2 ? true : false))
      << "expected: " << dt1.toString() << "!=" << dt2.toString() << endl;
  EXPECT_TRUE((dt1 < dt2 ? true : false))
      << "expected: " << dt1.toString() << "<" << dt2.toString() << endl;
}

TEST(DateTimeTests, isMore) {
  DateTime dt1 = DateTime();
  this_thread::sleep_for(1s);
  DateTime dt2 = DateTime();
  EXPECT_FALSE((dt1 == dt2 ? true : false))
      << "expected: " << dt1.toString() << "!=" << dt2.toString() << endl;
  EXPECT_TRUE((dt2 > dt1 ? true : false))
      << "expected: " << dt2.toString() << ">" << dt1.toString() << endl;
}
