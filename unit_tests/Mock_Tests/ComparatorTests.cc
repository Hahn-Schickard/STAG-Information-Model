#include "TestElementBuilder.hpp"

#include "gtest/gtest.h"

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;

template <typename ElementType> struct Comparator_TestType {
  using ElementTypePtr = shared_ptr<ElementType>;

  Comparator_TestType(const string& name_) : name(name_) {}

  ElementTypePtr makeEmpty() { return ElementTypePtr(); }

  ElementTypePtr make() {
    throw logic_error("Requested Element type is not supported");
  }

  ElementTypePtr makeAnother() {
    throw logic_error("Requested Element type is not supported");
  }

  const string& name;

protected:
  TestElementBuilder builder = TestElementBuilder();
};

ElementMetaInfo makeReadableInfo(const string& parent_id = "",
    const string& name = "readable",
    const string& desc = "readable element mock") {
  return ElementMetaInfo(parent_id, name, desc);
}

TestElementInfoPtr makeReadable(DataType return_type = DataType::BOOLEAN,
    const ElementMetaInfo& meta_info = makeReadableInfo()) {
  auto functionality = DeviceMockBuilder::Functionality(
      return_type, DeviceBuilderInterface::Reader());
  return make_shared<TestElementInfo>(meta_info, functionality);
}

TestElementInfoPtr makeReadable(const ElementMetaInfo& meta_info) {
  return makeReadable(DataType::BOOLEAN, meta_info);
}

struct SameElementTypeTestParam : Comparator_TestType<DeviceElement> {
  SameElementTypeTestParam() : Comparator_TestType("SameElementType") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeReadable());
  }

  DeviceElementPtr makeAnother() {
    // if we do not reset the builder, NamedElement refID_ values will be
    // different, thus elements will not be the same
    return builder.build<DeviceElement>(makeReadable());
  }
};

struct ReadableTestParam : Comparator_TestType<DeviceElement> {
  ReadableTestParam() : Comparator_TestType("Readable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeReadable());
  }

  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeReadable(DataType::DOUBLE));
  }
};

ElementMetaInfo makeWriteOnlyInfo(const string& parent_id = "",
    const string& name = "write-only",
    const string& desc = "write-only element mock") {
  return ElementMetaInfo(parent_id, name, desc);
}

TestElementInfoPtr makeWriteOnly(DataType return_type = DataType::BOOLEAN,
    const ElementMetaInfo& meta_info = makeWriteOnlyInfo()) {
  auto functionality = DeviceMockBuilder::Functionality(
      return_type, DeviceBuilderInterface::Writer());
  return make_shared<TestElementInfo>(meta_info, functionality);
}

TestElementInfoPtr makeWriteOnly(const ElementMetaInfo& meta_info) {
  return makeWriteOnly(DataType::BOOLEAN, meta_info);
}

struct WriteOnlyTestParam : Comparator_TestType<DeviceElement> {
  WriteOnlyTestParam() : Comparator_TestType("WriteOnly") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeWriteOnly());
  }

  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeWriteOnly(DataType::INTEGER));
  }
};

ElementMetaInfo makeWritableInfo(const string& parent_id = "",
    const string& name = "read&writable",
    const string& desc = "read&write element mock") {
  return ElementMetaInfo(parent_id, name, desc);
}

TestElementInfoPtr makeWritable(DataType return_type = DataType::BOOLEAN,
    const ElementMetaInfo& meta_info = makeWritableInfo()) {
  auto functionality = DeviceMockBuilder::Functionality(return_type,
      DeviceBuilderInterface::Reader(),
      DeviceBuilderInterface::Writer());
  return make_shared<TestElementInfo>(meta_info, functionality);
}

TestElementInfoPtr makeWritable(const ElementMetaInfo& meta_info) {
  return makeWritable(DataType::BOOLEAN, meta_info);
}

struct WritableTestParam : Comparator_TestType<DeviceElement> {
  WritableTestParam() : Comparator_TestType("Writable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeWritable());
  }

  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeWritable(DataType::STRING));
  }
};

ElementMetaInfo makeExecutableInfo(const string& parent_id = "",
    const string& name = "executable",
    const string& desc = "executable element mock") {
  return ElementMetaInfo(parent_id, name, desc);
}

TestElementInfoPtr makeExecutable(DataType return_type = DataType::BOOLEAN,
    Function::ParameterTypes supported_params = {},
    const ElementMetaInfo& meta_info = makeExecutableInfo()) {
  auto functionality = DeviceMockBuilder::Functionality(return_type,
      DeviceBuilderInterface::Executor(),
      DeviceBuilderInterface::Canceler(),
      supported_params);
  return make_shared<TestElementInfo>(meta_info, functionality);
}

TestElementInfoPtr makeExecutable(const ElementMetaInfo& meta_info) {
  return makeExecutable(DataType::BOOLEAN, {}, meta_info);
}

struct ExecutableTestParam : Comparator_TestType<DeviceElement> {
  ExecutableTestParam() : Comparator_TestType("Executable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeExecutable());
  }

  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeExecutable(DataType::OPAQUE));
  }
};

struct ExecutableNotWritableTestParam : Comparator_TestType<DeviceElement> {
  ExecutableNotWritableTestParam()
      : Comparator_TestType("ExecutableNotWritable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeExecutable(meta_info));
  }

  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeWritable(meta_info));
  }

private:
  const ElementMetaInfo meta_info = ElementMetaInfo(
      "", "executableNotWritable", "Just another executable element mock");
};

TestElementInfoPtr makeEmptyGroup(const string& parent_ref_id = "",
    const string& name = "empty-group",
    const string& desc = "Group element mock without any elements") {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);

  return make_shared<TestElementInfo>(meta_info);
}

enum class TestElementType {
  READABLE,
  WRITE_ONLY,
  WRITABLE,
  OBSERVABLE,
  EXECUTABLE
};

TestElementInfoPtr makeSubElement(
    TestElementType type, const string& parent_ref_id) {
  switch (type) {
  case TestElementType::READABLE: {
    return makeReadable(makeReadableInfo(parent_ref_id));
  }
  case TestElementType::WRITE_ONLY: {
    return makeWriteOnly(makeWriteOnlyInfo(parent_ref_id));
  }
  case TestElementType::WRITABLE: {
    return makeWritable(makeWritableInfo(parent_ref_id));
  }
  case TestElementType::EXECUTABLE: {
    return makeExecutable(makeExecutableInfo(parent_ref_id));
  }
  default: { throw logic_error(""); }
  }
}

vector<TestElementInfoPtr> makeSubElements(
    vector<TestElementType> subelement_types, const string& parent_ref_id) {
  vector<TestElementInfoPtr> subelements;
  for (auto subelement_type : subelement_types) {
    subelements.push_back(makeSubElement(subelement_type, parent_ref_id));
  }
  return subelements;
}

TestElementInfoPtr makeSingleLevelGroup(const string& parent_ref_id = "",
    const string& name = "1-level-group",
    const string& desc =
        "Group element mock containing only Metric/Function elements",
    vector<TestElementType> subelement_types =
        {// clang-format off
          TestElementType::READABLE,
          TestElementType::WRITE_ONLY,
          TestElementType::WRITABLE,
          TestElementType::EXECUTABLE
        } // clang-format on
) {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality();

  return make_shared<TestElementInfo>(meta_info,
      functionality,
      makeSubElements(subelement_types, parent_ref_id));
}

struct EmptyDeviceElementGroupTestParam
    : Comparator_TestType<DeviceElementGroup> {
  EmptyDeviceElementGroupTestParam() : Comparator_TestType("EmptyGroup") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeEmptyGroup());
  }

  DeviceElementGroupPtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup("",
        "another-group",
        "Just another simple element group mock",
        subelement_types));
  }
};

struct SimpleDeviceElementGroupTestParam
    : Comparator_TestType<DeviceElementGroup> {
  SimpleDeviceElementGroupTestParam() : Comparator_TestType("SimpleGroup") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup());
  }

  DeviceElementGroupPtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup("",
        "another-group",
        "Just another simple element group mock",
        subelement_types));
  }
};

TestElementInfoPtr makeNestedGroup(const string& parent_ref_id = "",
    const string& name = "nested-group",
    const string& desc = "Nested Group element mock",
    vector<TestElementType> subsubelement_types = {TestElementType::READABLE,
        TestElementType::WRITE_ONLY,
        TestElementType::WRITABLE,
        TestElementType::EXECUTABLE},
    vector<TestElementType> subelement_types = {TestElementType::READABLE,
        TestElementType::WRITE_ONLY,
        TestElementType::WRITABLE,
        TestElementType::EXECUTABLE}) {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality();
  vector<TestElementInfoPtr> elements = {makeSingleLevelGroup(
      "0", "subelement-group", "Subelement Group mock", subsubelement_types)};
  auto subelements = makeSubElements(subelement_types, parent_ref_id);
  elements.insert(elements.end(), subelements.begin(), subelements.end());
  return make_shared<TestElementInfo>(meta_info, functionality, elements);
}

struct ComplexDeviceElementGroupTestParam
    : Comparator_TestType<DeviceElementGroup> {
  ComplexDeviceElementGroupTestParam() : Comparator_TestType("ComplexGroup") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeNestedGroup());
  }

  DeviceElementGroupPtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subsubelement_types = {
        TestElementType::WRITE_ONLY, TestElementType::EXECUTABLE};
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(makeNestedGroup("",
        "another-nested-group",
        "Just another nested group element mock",
        subsubelement_types,
        subelement_types));
  }
};

struct SimpleDeviceTestParam : Comparator_TestType<Device> {
  SimpleDeviceTestParam() : Comparator_TestType("SimpleDevice") {}

  DevicePtr make() { return builder.build<Device>(makeSingleLevelGroup()); }

  DevicePtr makeAnother() {
    builder = TestElementBuilder(
        ElementMetaInfo("23456", "AnotherMocky", "It's just another mocked"));
    return builder.build<Device>(makeSingleLevelGroup());
  }
};

struct ComplexDeviceTestParam : Comparator_TestType<Device> {
  ComplexDeviceTestParam() : Comparator_TestType("ComplexDevice") {}

  DevicePtr make() { return builder.build<Device>(makeNestedGroup()); }

  DevicePtr makeAnother() {
    builder = TestElementBuilder(
        ElementMetaInfo("23456", "AnotherMocky", "It's just another mocked"));
    return builder.build<Device>(makeNestedGroup());
  }
};

template <typename T> struct Comparator_TestSuite : public ::testing::Test {
  Comparator_TestSuite() : param(T()) {}

  T param; // NOLINT(readability-identifier-naming)
};

using ComparatorTestTypes = ::testing::Types< //
    SameElementTypeTestParam,
    ReadableTestParam,
    WriteOnlyTestParam,
    ExecutableTestParam,
    ExecutableNotWritableTestParam,
    EmptyDeviceElementGroupTestParam,
    SimpleDeviceElementGroupTestParam,
    ComplexDeviceElementGroupTestParam,
    SimpleDeviceTestParam,
    ComplexDeviceTestParam>;

TYPED_TEST_SUITE(Comparator_TestSuite, ComparatorTestTypes);

TYPED_TEST(Comparator_TestSuite, isEqual) {
  auto empty = this->param.makeEmpty();

  EXPECT_TRUE(empty ? false : true) << " comparator empty ptr is not empty";
  EXPECT_EQ(empty, empty) << " comparator empty ptr is not equal to itself";

  auto tested = this->param.make();

  EXPECT_EQ(tested, tested)
      << this->param.name
      << " comparator tested ptr does not point to the same ptr";

  EXPECT_EQ(*tested, *tested)
      << this->param.name
      << " comparator tested value does not contain the same value";
}

TYPED_TEST(Comparator_TestSuite, isNotEqual) {
  auto tested = this->param.make();
  auto empty = this->param.makeEmpty();
  EXPECT_NE(tested, empty)
      << this->param.name
      << " comparator tested ptr is the same as an empty ptr";

  auto another = this->param.makeAnother();
  EXPECT_NE(tested, another)
      << this->param.name
      << " comparator tested ptr is the same as another ptr";
  EXPECT_NE(*tested, *another)
      << this->param.name
      << " comparator tested value is the same as another value";
}

TEST(DeviceElementGroupComparisonTests, emptyGroupsAreEqual) {
  auto builder = TestElementBuilder();
  auto empty_group_info = make_shared<TestElementInfo>(
      ElementMetaInfo("", "empty-group", "Just an empty group mock"));
  auto first_group = builder.build<DeviceElementGroup>(empty_group_info);
  builder.resetBuilder();
  auto second_group = builder.build<DeviceElementGroup>(empty_group_info);

  EXPECT_NE(first_group, second_group)
      << "First group points to the same pointer as second group";
  EXPECT_EQ(*first_group, *second_group)
      << "First group is not equal to the second one";
}

TEST(DeviceElementGroupComparisonTests, emptyGroupIsNotEqualToNotEmpty) {
  auto builder = TestElementBuilder();
  auto empty_group_info = make_shared<TestElementInfo>(
      ElementMetaInfo("", "empty-group", "Just an empty group mock"));
  auto first_group = builder.build<DeviceElementGroup>(empty_group_info);
  builder.resetBuilder();
  auto second_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup());

  EXPECT_NE(first_group, second_group)
      << "First group points to the same pointer as second group";
  EXPECT_NE(*first_group, *second_group)
      << "First group is equal to the second one";
}

TEST(DeviceElementGroupComparisonTests,
    sameSizedDifferentElementGroupsAreNotEqual) {
  { // 2 element group, each element type is different
    auto builder = TestElementBuilder();
    auto first_group_elements = vector<TestElementType>{
        TestElementType::READABLE, TestElementType::WRITABLE};
    auto first_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        "", "first-group", "first group mock", first_group_elements));
    builder.resetBuilder();
    auto second_group_elements = vector<TestElementType>{
        TestElementType::EXECUTABLE, TestElementType::WRITE_ONLY};
    auto second_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        "", "second-group", "second group mock", second_group_elements));
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
    auto builder = TestElementBuilder();
    auto first_group_elements = vector<TestElementType>{
        TestElementType::READABLE, TestElementType::WRITABLE};
    auto first_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        "", "first-group", "first group mock", first_group_elements));
    builder.resetBuilder();
    auto second_group_elements = vector<TestElementType>{
        TestElementType::EXECUTABLE, TestElementType::WRITABLE};
    auto second_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        "", "second-group", "second group mock", second_group_elements));
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
    auto builder = TestElementBuilder();
    auto first_group_elements = vector<TestElementType>{
        TestElementType::READABLE, TestElementType::WRITABLE};
    auto first_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        "", "first-group", "first group mock", first_group_elements));
    builder.resetBuilder();
    auto second_group_elements = vector<TestElementType>{
        TestElementType::READABLE, TestElementType::EXECUTABLE};
    auto second_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        "", "second-group", "second group mock", second_group_elements));
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
    auto builder = TestElementBuilder();
    auto first_group_elements =
        vector<TestElementType>{TestElementType::READABLE,
            TestElementType::EXECUTABLE,
            TestElementType::WRITABLE};
    auto first_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        "", "first-group", "first group mock", first_group_elements));
    builder.resetBuilder();
    auto second_group_elements =
        vector<TestElementType>{TestElementType::READABLE,
            TestElementType::WRITE_ONLY,
            TestElementType::WRITABLE};
    auto second_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        "", "second-group", "second group mock", second_group_elements));
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

TEST(DeviceElementGroupComparisonTests,
    sameSizedDifferentElementOrderGroupsAreNotEqual) {
  auto builder = TestElementBuilder();
  auto first_group_elements = vector<TestElementType>{
      TestElementType::READABLE, TestElementType::WRITABLE};
  auto first_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
      "", "first-group", "first group mock", first_group_elements));
  builder.resetBuilder();
  auto second_group_elements = vector<TestElementType>{
      TestElementType::WRITABLE, TestElementType::READABLE};
  auto second_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
      "", "second-group", "second group mock", second_group_elements));

  EXPECT_EQ(first_group->getSubelements().size(),
      second_group->getSubelements().size())
      << "Groups are of different sizes";
  EXPECT_NE(first_group, second_group)
      << "First group points to the same pointer as second group";
  EXPECT_NE(*first_group, *second_group)
      << "First group is equal to the second one";
}

TEST(DeviceElementGroupComparisonTests, differentSizeGroupsAreNotEqual) {
  auto builder = TestElementBuilder();
  auto first_group_elements = vector<TestElementType>{
      TestElementType::READABLE, TestElementType::WRITABLE};
  auto first_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
      "", "first-group", "first group mock", first_group_elements));
  builder.resetBuilder();
  auto second_group_elements =
      vector<TestElementType>{TestElementType::WRITABLE,
          TestElementType::READABLE,
          TestElementType::WRITE_ONLY};
  auto second_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup(
      "", "second-group", "second group mock", second_group_elements));

  EXPECT_NE(first_group->getSubelements().size(),
      second_group->getSubelements().size())
      << "Groups are of different sizes";
  EXPECT_NE(first_group, second_group)
      << "First group points to the same pointer as second group";
  EXPECT_NE(*first_group, *second_group)
      << "First group is equal to the second one";
}