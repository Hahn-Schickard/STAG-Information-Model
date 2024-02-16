#include "ObservableMetric_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;

TEST(ObservableMetricTests, canBuild) {
  EXPECT_NO_THROW(auto observable = make_shared<MockObservableMetric>());
}