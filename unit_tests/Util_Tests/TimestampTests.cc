#include "DataVariant.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

struct TimestampTests : public testing::Test {
  TimestampTests() = default;

  Timestamp time1{2025, 9, 11, 18, 9, 23, 521};
  Timestamp time2{2025, 9, 11, 18, 9, 0, 521};
  Timestamp time3{2025, 9, 11, 18, 0, 23, 521};
  Timestamp time4{2025, 9, 11, 0, 9, 23, 521};
  Timestamp time5{2025, 9, 1, 18, 9, 23, 521};
  Timestamp time6{2025, 1, 11, 18, 9, 23, 521};
  Timestamp bad_time1{0, 9, 11, 18, 9, 23, 521};
  Timestamp bad_time2{2025, 13, 11, 18, 9, 23, 521};
  Timestamp bad_time3{2025, 9, 32, 18, 9, 23, 521};
  Timestamp bad_time4{2025, 9, 11, 25, 9, 23, 521};
  Timestamp bad_time5{2025, 9, 11, 18, 61, 23, 521};
  Timestamp bad_time6{2025, 9, 11, 18, 9, 61, 521};
  Timestamp bad_time7{2025, 0, 11, 18, 9, 61, 521};
  Timestamp bad_time8{2025, 9, 0, 18, 9, 61, 521};
};

TEST_F(TimestampTests, timestampsEqual) {
  EXPECT_EQ(time1, time1);
  EXPECT_EQ(time2, time2);
  EXPECT_EQ(time3, time3);
  EXPECT_EQ(time4, time4);
  EXPECT_EQ(time5, time5);
  EXPECT_EQ(time6, time6);
}

TEST_F(TimestampTests, timestampsNotEqual) {
  EXPECT_NE(time1, time2);
  EXPECT_NE(time2, time3);
  EXPECT_NE(time3, time4);
  EXPECT_NE(time4, time5);
  EXPECT_NE(time5, time6);
}

TEST_F(TimestampTests, timestampsMore) {
  EXPECT_TRUE(time1 > time2);
  EXPECT_TRUE(time2 > time3);
  EXPECT_TRUE(time3 > time4);
  EXPECT_TRUE(time4 > time5);
  EXPECT_TRUE(time5 > time6);
}

TEST_F(TimestampTests, verifyTimestampThrows) {
  EXPECT_THAT([this]() { verifyTimestamp(bad_time1); },
      ThrowsMessage<invalid_argument>(
          "Ancient time recording is not supported"));

  EXPECT_THAT([this]() { verifyTimestamp(bad_time2); },
      ThrowsMessage<invalid_argument>(
          "A year can not have more than 12 months"));

  EXPECT_THAT([this]() { verifyTimestamp(bad_time3); },
      ThrowsMessage<invalid_argument>(
          "A Month can not have more than 31 days"));

  EXPECT_THAT([this]() { verifyTimestamp(bad_time4); },
      ThrowsMessage<invalid_argument>("A day can not have more than 24 hours"));

  EXPECT_THAT([this]() { verifyTimestamp(bad_time5); },
      ThrowsMessage<invalid_argument>(
          "An hour can not have more than 59 minutes"));

  EXPECT_THAT([this]() { verifyTimestamp(bad_time6); },
      ThrowsMessage<invalid_argument>(
          "A minute can not have more than 59 seconds"));

  EXPECT_THAT([this]() { verifyTimestamp(bad_time7); },
      ThrowsMessage<invalid_argument>("A year can not have 0 months"));

  EXPECT_THAT([this]() { verifyTimestamp(bad_time8); },
      ThrowsMessage<invalid_argument>("A Month can not have 0 days"));
}

} // namespace Information_Model::testing