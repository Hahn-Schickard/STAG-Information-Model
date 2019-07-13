#include "BlueprintTests.hpp"
#include "BlueprintTests.hpp"
#include "ConsumerAdapterTests.hpp"
#include "ConsumerAdapterTests.hpp"
#include "DeviceUnitTests.hpp"

#include "gtest/gtest.h"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
