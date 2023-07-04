#include "TestElementBuilder.hpp"

#include "gtest/gtest.h"

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;

class DeviceElementGroupComparisonTests : public ::testing::Test {
protected:
  void SetUp() override {
    first_group_info =
        makeSingleLevelGroupInfo("", "first-group", "first group mock");
    second_group_info =
        makeSingleLevelGroupInfo("", "second-group", "second group mock");
  }

  // NOLINTNEXTLINE(readability-identifier-naming)
  TestElementBuilder builder = TestElementBuilder();
  ElementMetaInfo first_group_info; // NOLINT(readability-identifier-naming)
  ElementMetaInfo second_group_info; // NOLINT(readability-identifier-naming)
};

// NOLINTNEXTLINE
TEST_F(DeviceElementGroupComparisonTests, emptyGroupsAreEqual) {
  auto first_group =
      builder.build<DeviceElementGroup>(makeEmptyGroup(first_group_info));
  builder.resetBuilder();
  auto second_group =
      builder.build<DeviceElementGroup>(makeEmptyGroup(second_group_info));

  EXPECT_NE(first_group, second_group)
      << "First group points to the same pointer as second group";
  EXPECT_EQ(*first_group, *second_group)
      << "First group is not equal to the second one";
}

// NOLINTNEXTLINE
TEST_F(DeviceElementGroupComparisonTests, emptyGroupIsNotEqualToNotEmpty) {
  auto first_group =
      builder.build<DeviceElementGroup>(makeEmptyGroup(first_group_info));
  builder.resetBuilder();
  auto second_group = builder.build<DeviceElementGroup>(
      makeSingleLevelGroup(second_group_info));

  EXPECT_NE(first_group, second_group)
      << "First group points to the same pointer as second group";
  EXPECT_NE(*first_group, *second_group)
      << "First group is equal to the second one";
}

// NOLINTNEXTLINE
TEST_F(DeviceElementGroupComparisonTests,
    sameSizedDifferentElementGroupsAreNotEqual) {
  { // 2 element group, each element type is different
    auto first_group_elements = vector<TestElementType>{
        TestElementType::READABLE, TestElementType::WRITABLE};
    auto first_group = builder.build<DeviceElementGroup>(
        makeSingleLevelGroup(first_group_info, first_group_elements));
    builder.resetBuilder();
    auto second_group_elements = vector<TestElementType>{
        TestElementType::EXECUTABLE, TestElementType::WRITE_ONLY};
    auto second_group = builder.build<DeviceElementGroup>(
        makeSingleLevelGroup(second_group_info, second_group_elements));
    builder.resetBuilder();

    EXPECT_EQ(first_group->getSubelements().size(),
        second_group->getSubelements().size())
        << "Groups are of different sizes";
    EXPECT_NE(first_group, second_group)
        << "First group points to the same pointer as second group";
    EXPECT_NE(*first_group, *second_group)
        << "First group is equal to the second one";
  }
  { // 2 element group, first element is different
    auto first_group_elements = vector<TestElementType>{
        TestElementType::READABLE, TestElementType::WRITABLE};
    auto first_group = builder.build<DeviceElementGroup>(
        makeSingleLevelGroup(first_group_info, first_group_elements));
    builder.resetBuilder();
    auto second_group_elements = vector<TestElementType>{
        TestElementType::EXECUTABLE, TestElementType::WRITABLE};
    auto second_group = builder.build<DeviceElementGroup>(
        makeSingleLevelGroup(second_group_info, second_group_elements));
    builder.resetBuilder();

    EXPECT_EQ(first_group->getSubelements().size(),
        second_group->getSubelements().size())
        << "Groups are of different sizes";
    EXPECT_NE(first_group, second_group)
        << "First group points to the same pointer as second group";
    EXPECT_NE(*first_group, *second_group)
        << "First group is equal to the second one";
  }
  { // 2 element group, last element is different
    auto first_group_elements = vector<TestElementType>{
        TestElementType::READABLE, TestElementType::WRITABLE};
    auto first_group = builder.build<DeviceElementGroup>(
        makeSingleLevelGroup(first_group_info, first_group_elements));
    builder.resetBuilder();
    auto second_group_elements = vector<TestElementType>{
        TestElementType::READABLE, TestElementType::EXECUTABLE};
    auto second_group = builder.build<DeviceElementGroup>(
        makeSingleLevelGroup(second_group_info, second_group_elements));
    builder.resetBuilder();

    EXPECT_EQ(first_group->getSubelements().size(),
        second_group->getSubelements().size())
        << "Groups are of different sizes";
    EXPECT_NE(first_group, second_group)
        << "First group points to the same pointer as second group";
    EXPECT_NE(*first_group, *second_group)
        << "First group is equal to the second one";
  }
  { // 3 element group, middle element is different
    auto first_group_elements =
        vector<TestElementType>{TestElementType::READABLE,
            TestElementType::EXECUTABLE,
            TestElementType::WRITABLE};
    auto first_group = builder.build<DeviceElementGroup>(
        makeSingleLevelGroup(first_group_info, first_group_elements));
    builder.resetBuilder();
    auto second_group_elements =
        vector<TestElementType>{TestElementType::READABLE,
            TestElementType::WRITE_ONLY,
            TestElementType::WRITABLE};
    auto second_group = builder.build<DeviceElementGroup>(
        makeSingleLevelGroup(second_group_info, second_group_elements));
    builder.resetBuilder();

    EXPECT_EQ(first_group->getSubelements().size(),
        second_group->getSubelements().size())
        << "Groups are of different sizes";
    EXPECT_NE(first_group, second_group)
        << "First group points to the same pointer as second group";
    EXPECT_NE(*first_group, *second_group)
        << "First group is equal to the second one";
  }
}

// NOLINTNEXTLINE
TEST_F(DeviceElementGroupComparisonTests,
    sameSizedDifferentElementOrderGroupsAreNotEqual) {
  auto first_group_elements = vector<TestElementType>{
      TestElementType::READABLE, TestElementType::WRITABLE};
  auto first_group = builder.build<DeviceElementGroup>(
      makeSingleLevelGroup(first_group_info, first_group_elements));
  builder.resetBuilder();
  auto second_group_elements = vector<TestElementType>{
      TestElementType::WRITABLE, TestElementType::READABLE};
  auto second_group = builder.build<DeviceElementGroup>(
      makeSingleLevelGroup(second_group_info, second_group_elements));

  EXPECT_EQ(first_group->getSubelements().size(),
      second_group->getSubelements().size())
      << "Groups are of different sizes";
  EXPECT_NE(first_group, second_group)
      << "First group points to the same pointer as second group";
  EXPECT_NE(*first_group, *second_group)
      << "First group is equal to the second one";
}

// NOLINTNEXTLINE
TEST_F(DeviceElementGroupComparisonTests, differentSizeGroupsAreNotEqual) {
  auto first_group_elements = vector<TestElementType>{
      TestElementType::READABLE, TestElementType::WRITABLE};
  auto first_group = builder.build<DeviceElementGroup>(
      makeSingleLevelGroup(first_group_info, first_group_elements));
  builder.resetBuilder();
  auto second_group_elements =
      vector<TestElementType>{TestElementType::WRITABLE,
          TestElementType::READABLE,
          TestElementType::WRITE_ONLY};
  auto second_group = builder.build<DeviceElementGroup>(
      makeSingleLevelGroup(second_group_info, second_group_elements));

  EXPECT_NE(first_group->getSubelements().size(),
      second_group->getSubelements().size())
      << "Groups are of different sizes";
  EXPECT_NE(first_group, second_group)
      << "First group points to the same pointer as second group";
  EXPECT_NE(*first_group, *second_group)
      << "First group is equal to the second one";
}