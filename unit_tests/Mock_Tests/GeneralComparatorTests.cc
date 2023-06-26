#include "TestElementBuilder.hpp"

#include "gtest/gtest.h"

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;

template <typename ElementType> struct Comparator_TestType {
  using ElementTypePtr = shared_ptr<ElementType>;

  Comparator_TestType(const string& name_) : name(name_) {}

  ElementTypePtr makeEmpty() { return ElementTypePtr(); }

  /**
   * Sanity checks, test if the built parameter is not equal to an empty
   * pointer, if the built parameter points to the same pointer and if it's
   * value is the same
   */
  ElementTypePtr make() {
    throw logic_error("Requested Element type is not supported");
  }

  ElementTypePtr makeAnother() {
    throw logic_error("Requested Element type is not supported");
  }

  const string& name;
  TestElementBuilder builder = TestElementBuilder();
};

/**
 * Test paramater for same element type test case
 *
 */
struct DifferentRefIdsTestParam : Comparator_TestType<DeviceElement> {
  DifferentRefIdsTestParam() : Comparator_TestType("Different Ref IDs") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeReadable());
  }

  /**
   * In this test case we check if two elements containing the same element type
   * and meta information are the same when they are built for the same device.
   *
   * Expected result is that these elements are not equal, due to refID_ values
   * being different
   */
  DeviceElementPtr makeAnother() {
    // if we do not reset the builder, NamedElement refID_ values will be
    // different, thus elements will not be the same
    return builder.build<DeviceElement>(makeReadable());
  }
};

/**
 * Test paramater for DeviceElement with different element types test case
 *
 */
struct DifferentElementTypesTestParam : Comparator_TestType<DeviceElement> {
  DifferentElementTypesTestParam()
      : Comparator_TestType("Different Element Types") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeExecutable(meta_info));
  }

  /**
   * In this test case we check if two DeviceElement instances, containing the
   * same refID_, name and description are different because of their
   * ElementType
   *
   * Expected result is that these elements are not equal, due to ElementType
   * values being different
   */
  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeWritable(meta_info));
  }

private:
  const ElementMetaInfo meta_info =
      ElementMetaInfo("", "element", "Just another element mock");
};

/**
 * Test paramater for readable metric with differing value types test case
 *
 */
struct DifferentReadableValuesTestParam : Comparator_TestType<DeviceElement> {
  DifferentReadableValuesTestParam()
      : Comparator_TestType("Different Metric Value Types") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeReadable());
  }

  /**
   * In this test case we check if two metrics contain are of
   * ElementType::READABLE type, have the same refID_, name and description, but
   * different return types
   *
   * Expected result is that these elements are not equal, due to return type
   * values being different
   */
  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeReadable(DataType::DOUBLE));
  }
};

/**
 * Test paramater for write-only metric (Writable metric without read callback)
 * with differing value types test case
 *
 */
struct DifferentWriteOnlyValuesTestParam : Comparator_TestType<DeviceElement> {
  DifferentWriteOnlyValuesTestParam()
      : Comparator_TestType("Different Write-only WritableMetric Value Types") {
  }

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeWriteOnly());
  }

  /**
   * In this test case we check if two metrics contain are of
   * ElementType::WRITABLE type, have the same refID_, name and description, but
   * different return types
   *
   * Expected result is that these elements are not equal, due to return type
   * values being different
   */
  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeWriteOnly(DataType::INTEGER));
  }
};

/**
 * Test paramater for writable metric with differing value types test case
 *
 */
struct DifferentWritableValuesTestParam : Comparator_TestType<DeviceElement> {
  DifferentWritableValuesTestParam()
      : Comparator_TestType("Different WritableMetric Value Types") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeWritable());
  }

  /**
   * In this test case we check if two metrics contain are of
   * ElementType::WRITABLE type, have the same refID_, name and description, but
   * different return types
   *
   * Expected result is that these elements are not equal, due to return type
   * values being different
   */
  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeWritable(DataType::STRING));
  }
};

/**
 * Test paramater for function with different return values test case
 *
 */
struct DifferentExecuteReturnsTestParam : Comparator_TestType<DeviceElement> {
  DifferentExecuteReturnsTestParam()
      : Comparator_TestType("Different Function Return Types") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeExecutable());
  }

  /**
   * In this test case we check if two metrics contain are of
   * ElementType::FUNCTION type, have the same refID_, name, description and
   * acceptable parameters, but different return types
   *
   * Expected result is that these elements are not equal, due to return type
   * values being different
   */
  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeExecutable(DataType::OPAQUE));
  }
};

/**
 * Test paramater for function with different acceptable parameters test case
 *
 */
struct DifferentExecuteParamsTestParam : Comparator_TestType<DeviceElement> {
  DifferentExecuteParamsTestParam()
      : Comparator_TestType("Different Function parameters") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeExecutable());
  }

  /**
   * In this test case we check if two metrics contain are of
   * ElementType::FUNCTION type, have the same refID_, name, description and
   * return types, but different acceptable parameters
   *
   * Expected result is that these elements are not equal, due to acceptable
   * parameter values being different
   */
  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeExecutable(DataType::OPAQUE));
  }
};

/**
 * Test paramater for empty DeviceElementGroup comparison win non-empty
 * DeviceElementGroup test case
 *
 */
struct EmptyGroupNotEqualToNotEmptyTestParam
    : Comparator_TestType<DeviceElementGroup> {
  EmptyGroupNotEqualToNotEmptyTestParam()
      : Comparator_TestType("Empty Group not equal to not-empty Group") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeEmptyGroup());
  }

  /**
   * In this test case we check if an empty DeviceElementGroup is equal to a
   * non-empty DeviceElementGroup
   *
   * Expected result is that these elements are not equal
   */
  DeviceElementGroupPtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        makeSingleLevelGroupInfo(
            "", "another-group", "Just another simple element group mock"),
        subelement_types));
  }
};

/**
 * Test paramater for different single level DeviceElementGroups test case
 *
 */
struct DifferentSingleElementGroupsTestParam
    : Comparator_TestType<DeviceElementGroup> {
  DifferentSingleElementGroupsTestParam()
      : Comparator_TestType("Different single element groups") {}

  DeviceElementGroupPtr make() {
    vector<TestElementType> subelement_types = {TestElementType::READABLE};
    return builder.build<DeviceElementGroup>(
        makeSingleLevelGroup(meta_info, subelement_types));
  }

  /**
   * In this test case we check if one DeviceElementGroup with a single element
   * is equal to a another DeviceElementGroup with another single element
   *
   * Expected result is that these elements are not equal, because the contained
   * elements are different
   */
  DeviceElementGroupPtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subelement_types = {TestElementType::WRITABLE};
    return builder.build<DeviceElementGroup>(
        makeSingleLevelGroup(meta_info, subelement_types));
  }

private:
  const ElementMetaInfo meta_info = ElementMetaInfo(
      "", "single-element-group", "Just a group mock with a single element");
};

/**
 * Test paramater for different single level DeviceElementGroups test case
 *
 */
struct DifferentSingleLevelGroupsTestParam
    : Comparator_TestType<DeviceElementGroup> {
  DifferentSingleLevelGroupsTestParam()
      : Comparator_TestType("Different single level groups") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup());
  }

  /**
   * In this test case we check if an one single level DeviceElementGroup is
   * equal to a another single level DeviceElementGroup
   *
   * Expected result is that these elements are not equal, because the contained
   * elements are different
   */
  DeviceElementGroupPtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        makeSingleLevelGroupInfo(
            "", "another-group", "Just another simple element group mock"),
        subelement_types));
  }
};

/**
 * Test paramater for different two level nested DeviceElementGroups test case
 *
 */
struct DifferentTwoLevelNestedGroupsTestParam
    : Comparator_TestType<DeviceElementGroup> {
  DifferentTwoLevelNestedGroupsTestParam()
      : Comparator_TestType("Different two level nested groups") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeNestedGroup());
  }

  /**
   * In this test case we check if one DeviceElementGroup with a single nested
   * group is equal to a another DeviceElementGroup with another nested group
   *
   * Expected result is that these elements are not equal, because the contained
   * elements are different
   */
  DeviceElementGroupPtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subsubelement_types = {
        TestElementType::WRITE_ONLY, TestElementType::EXECUTABLE};
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(
        makeNestedGroup(makeNestedGroupInfo("",
                            "another-nested-group",
                            "Just another nested group element mock"),
            subsubelement_types,
            subelement_types));
  }
};

/**
 * Test paramater for different single level Devices test case
 *
 */
struct DifferentSingleLevelDevicesTestParam : Comparator_TestType<Device> {
  DifferentSingleLevelDevicesTestParam()
      : Comparator_TestType("SimpleDevice") {}

  DevicePtr make() { return builder.build<Device>(makeSingleLevelGroup()); }

  /**
   * In this test case we check if one Device with a single level group is
   * equal to a another Device with same single level group
   *
   * Expected result is that these elements are not equal, because the Device
   * ref IDs are not the same
   */
  DevicePtr makeAnother() {
    builder = TestElementBuilder(
        ElementMetaInfo("23456", "AnotherMocky", "It's just another mocked"));
    return builder.build<Device>(makeSingleLevelGroup());
  }
};

/**
 * Test paramater for different two level nested Devices test case
 *
 */
struct DifferentTwoLevelDevicesTestParam : Comparator_TestType<Device> {
  DifferentTwoLevelDevicesTestParam() : Comparator_TestType("ComplexDevice") {}

  DevicePtr make() { return builder.build<Device>(makeNestedGroup()); }

  /**
   * In this test case we check if one Device with a single nested level group
   * is equal to a another Device with another single nested group
   *
   * Expected result is that these elements are not equal, because the contained
   * elements are different
   */
  DevicePtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subsubelement_types = {
        TestElementType::WRITE_ONLY, TestElementType::EXECUTABLE};
    vector<TestElementType> subelement_types = {TestElementType::READABLE,
        TestElementType::WRITE_ONLY,
        TestElementType::WRITABLE,
        TestElementType::EXECUTABLE};
    return builder.build<Device>(
        makeNestedGroup(makeNestedGroupInfo("",
                            "another-nested-group",
                            "Just another nested group element mock"),
            subsubelement_types,
            subelement_types));
  }
};

template <typename T> struct Comparator_TestSuite : public ::testing::Test {
  Comparator_TestSuite() : param(T()) {}

  T param; // NOLINT(readability-identifier-naming)
};

using ComparatorTestTypes = ::testing::Types< //
    DifferentRefIdsTestParam,
    DifferentElementTypesTestParam,
    DifferentReadableValuesTestParam,
    DifferentWriteOnlyValuesTestParam,
    DifferentWritableValuesTestParam,
    DifferentExecuteReturnsTestParam,
    DifferentExecuteParamsTestParam,
    EmptyGroupNotEqualToNotEmptyTestParam,
    DifferentSingleElementGroupsTestParam,
    DifferentSingleLevelGroupsTestParam,
    DifferentTwoLevelNestedGroupsTestParam,
    DifferentSingleLevelDevicesTestParam,
    DifferentTwoLevelDevicesTestParam>;

TYPED_TEST_SUITE(Comparator_TestSuite, ComparatorTestTypes);

TYPED_TEST(Comparator_TestSuite, isEqual) {
  auto empty = this->param.makeEmpty();

  EXPECT_TRUE(empty ? false : true)
      << this->param.name << " comparator empty ptr is not empty";
  EXPECT_EQ(empty, empty) << this->param.name
                          << " comparator empty ptr is not equal to itself";

  auto tested = this->param.make();

  EXPECT_EQ(tested, tested)
      << this->param.name
      << " comparator tested ptr does not point to the same ptr";

  EXPECT_EQ(*tested, *tested)
      << this->param.name
      << " comparator tested value does not contain the same value";

  this->param.builder.resetBuilder();
  auto tested_copy = this->param.make();
  EXPECT_EQ(*tested, *tested_copy) << this->param.name
                                   << " comparator tested value does not "
                                      "contain the same value as copied value";
}

TYPED_TEST(Comparator_TestSuite, isNotEqual) {
  auto tested = this->param.make();
  auto empty = this->param.makeEmpty();
  EXPECT_NE(tested, empty)
      << this->param.name
      << " comparator tested ptr is the same as an empty ptr";

  this->param.builder.resetBuilder();
  auto tested_copy = this->param.make();
  EXPECT_NE(tested, tested_copy)
      << this->param.name
      << " comparator tested ptr points to the same ptr as tested copy ptr";

  auto another = this->param.makeAnother();
  EXPECT_NE(tested, another)
      << this->param.name
      << " comparator tested ptr points to the same ptr as another ptr";
  EXPECT_NE(*tested, *another)
      << this->param.name
      << " comparator tested value is the same as another value";
}
