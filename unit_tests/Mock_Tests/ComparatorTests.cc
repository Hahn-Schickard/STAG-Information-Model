#include "Device.hpp"
#include "mocks/DeviceMockBuilder.hpp"

#include "gtest/gtest.h"

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;

struct ElementMetaInfo {
  ElementMetaInfo(const string& ref_id, const string& name, const string& desc)
      : ref_ID_(ref_id), name_(name), desc_(desc) {}

  string ref_ID_;
  string name_;
  string desc_;
};

struct Comparator_TestParam {
  Comparator_TestParam(ElementMetaInfo meta_info_,
      DeviceMockBuilder::Functionality functionality_,
      vector<shared_ptr<Comparator_TestParam>> subelements_ = {})
      : meta_info(meta_info_), functionality(functionality_),
        subelements(subelements_) {}

  // NOLINTNEXTLINE(readability-identifier-naming)
  ElementMetaInfo meta_info;
  // NOLINTNEXTLINE(readability-identifier-naming)
  const DeviceMockBuilder::Functionality functionality;
  // NOLINTNEXTLINE(readability-identifier-naming)
  const vector<shared_ptr<Comparator_TestParam>> subelements;
};

using Comparator_TestParamPtr = shared_ptr<Comparator_TestParam>;

struct TestElementBuilder : DeviceMockBuilder {
  TestElementBuilder()
      : TestElementBuilder(ElementMetaInfo("12345", "Mocky", "It's mocked")) {}

  TestElementBuilder(ElementMetaInfo device_info) {
    buildDeviceBase(device_info.ref_ID_, device_info.name_, device_info.desc_);
  }

  template <typename Element>
  shared_ptr<Element> build(Comparator_TestParamPtr /* param */) {
    throw logic_error("Element builder can not build the given element type");
  }
};

template <>
DeviceElementPtr TestElementBuilder::build<DeviceElement>(
    Comparator_TestParamPtr param) {
  DeviceElementPtr result;
  auto group = getGroupImplementation(param->meta_info.ref_ID_);
  auto ref_id = group->generateReferenceID();
  if (param->functionality.type() != ElementType::GROUP) {
    auto interface = buildSpecificInterface(param->functionality);
    result = makeDeviceElement( //
        ref_id,
        param->meta_info.name_,
        param->meta_info.desc_,
        move(interface));
  } else {
    auto sub_group =
        make_shared<::testing::NiceMock<MockDeviceElementGroup>>(ref_id);
    result = makeDeviceElement(ref_id,
        param->meta_info.name_,
        param->meta_info.desc_,
        NonemptyDeviceElementGroupPtr(sub_group));
  }
  group->addDeviceElement(NonemptyDeviceElementPtr(result));
  return result;
}

template <>
DeviceElementGroupPtr TestElementBuilder::build<DeviceElementGroup>(
    Comparator_TestParamPtr param) {
  auto base_group = getGroupImplementation(param->meta_info.ref_ID_);
  auto ref_id = base_group->generateReferenceID();
  auto group = make_shared<::testing::NiceMock<MockDeviceElementGroup>>(ref_id);
  auto element = makeDeviceElement(ref_id,
      param->meta_info.name_,
      param->meta_info.desc_,
      NonemptyDeviceElementGroupPtr(group));
  base_group->addDeviceElement(NonemptyDeviceElementPtr(element));
  for (auto subelement_info : param->subelements) {
    // assign new group id as parent
    subelement_info->meta_info.ref_ID_ = ref_id;
    auto subelement = build<DeviceElement>(subelement_info);
    // subelements are automaticaly added to this group in
    // build<DeviceElement>() call, thus there is no need to call
    // group->addDeviceElement(NonemptyDeviceElementPtr(subelement))
  }
  return group;
}

template <>
DevicePtr TestElementBuilder::build<Device>(Comparator_TestParamPtr param) {
  for (auto element : param->subelements) {
    build<DeviceElement>(element);
  }

  return std::move(device_);
}

template <typename ElementType> struct Comparator_TestType {
  using ElementTypePtr = shared_ptr<ElementType>;

  Comparator_TestType(const string& name_) : name(name_) {}

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

Comparator_TestParamPtr makeReadable(const string& parent_ref_id = "",
    const string& name = "readable",
    const string& desc = "readable element mock") {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality(
      DataType::BOOLEAN, DeviceBuilderInterface::Reader());
  return make_shared<Comparator_TestParam>(meta_info, functionality);
}

struct ReadableTestParam : Comparator_TestType<DeviceElement> {
  ReadableTestParam() : Comparator_TestType("Readable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeReadable());
  }

  DeviceElementPtr makeAnother() {
    return builder.build<DeviceElement>(makeReadable(
        "", "another-readable", "Just another readable element mock"));
  }
};

Comparator_TestParamPtr makeWriteOnly(const string& parent_ref_id = "",
    const string& name = "write-only",
    const string& desc = "write-only element mock") {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality(
      DataType::BOOLEAN, DeviceBuilderInterface::Writer());
  return make_shared<Comparator_TestParam>(meta_info, functionality);
}

struct WriteOnlyTestParam : Comparator_TestType<DeviceElement> {
  WriteOnlyTestParam() : Comparator_TestType("WriteOnly") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeWriteOnly());
  }

  DeviceElementPtr makeAnother() {
    return builder.build<DeviceElement>(makeWriteOnly(
        "", "another-write-only", "Just another write-only element mock"));
  }
};

Comparator_TestParamPtr makeWritable(const string& parent_ref_id = "",
    const string& name = "read&write",
    const string& desc = "read&write element mock") {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality(DataType::BOOLEAN,
      DeviceBuilderInterface::Reader(),
      DeviceBuilderInterface::Writer());
  return make_shared<Comparator_TestParam>(meta_info, functionality);
}

struct WritableTestParam : Comparator_TestType<DeviceElement> {
  WritableTestParam() : Comparator_TestType("Writable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeWritable());
  }

  DeviceElementPtr makeAnother() {
    return builder.build<DeviceElement>(makeWritable(
        "", "another-writable", "Just another writable element mock"));
  }
};

Comparator_TestParamPtr makeExecutable(const string& parent_ref_id = "",
    const string& name = "executable",
    const string& desc = "executable element mock") {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality(DataType::BOOLEAN,
      DeviceBuilderInterface::Executor(),
      DeviceBuilderInterface::Canceler());
  return make_shared<Comparator_TestParam>(meta_info, functionality);
}

struct ExecutableTestParam : Comparator_TestType<DeviceElement> {
  ExecutableTestParam() : Comparator_TestType("Executable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeExecutable());
  }

  DeviceElementPtr makeAnother() {
    return builder.build<DeviceElement>(makeExecutable(
        "", "another-executable", "Just another executable element mock"));
  }
};

enum class TestElementType {
  READABLE,
  WRITE_ONLY,
  WRITABLE,
  OBSERVABLE,
  EXECUTABLE
};

Comparator_TestParamPtr makeSubElement(
    TestElementType type, const string& parent_ref_id) {
  switch (type) {
  case TestElementType::READABLE: {
    return makeReadable(parent_ref_id);
  }
  case TestElementType::WRITE_ONLY: {
    return makeWriteOnly(parent_ref_id);
  }
  case TestElementType::WRITABLE: {
    return makeWritable(parent_ref_id);
  }
  case TestElementType::EXECUTABLE: {
    return makeExecutable(parent_ref_id);
  }
  default: { throw logic_error(""); }
  }
}

vector<Comparator_TestParamPtr> makeSubElements(
    vector<TestElementType> subelement_types, const string& parent_ref_id) {
  vector<Comparator_TestParamPtr> subelements;
  for (auto subelement_type : subelement_types) {
    subelements.push_back(makeSubElement(subelement_type, parent_ref_id));
  }
  return subelements;
}

Comparator_TestParamPtr makeSingleLevelGroup(const string& parent_ref_id = "",
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

  return make_shared<Comparator_TestParam>(meta_info,
      functionality,
      makeSubElements(subelement_types, parent_ref_id));
}

struct SimpleDeviceElementGroupTestParam
    : Comparator_TestType<DeviceElementGroup> {
  SimpleDeviceElementGroupTestParam() : Comparator_TestType("SimpleGroup") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup());
  }

  DeviceElementGroupPtr makeAnother() {
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

Comparator_TestParamPtr makeNestedGroup(const string& parent_ref_id = "",
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
  vector<Comparator_TestParamPtr> elements = {makeSingleLevelGroup(
      "0", "subelement-group", "Subelement Group mock", subsubelement_types)};
  auto subelements = makeSubElements(subelement_types, parent_ref_id);
  elements.insert(elements.end(), subelements.begin(), subelements.end());
  return make_shared<Comparator_TestParam>(meta_info, functionality, elements);
}

struct ComplexDeviceElementGroupTestParam
    : Comparator_TestType<DeviceElementGroup> {
  ComplexDeviceElementGroupTestParam() : Comparator_TestType("ComplexGroup") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeNestedGroup());
  }

  DeviceElementGroupPtr makeAnother() {
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
    ReadableTestParam,
    WriteOnlyTestParam,
    ExecutableTestParam,
    SimpleDeviceElementGroupTestParam,
    ComplexDeviceElementGroupTestParam,
    SimpleDeviceTestParam,
    ComplexDeviceTestParam>;

TYPED_TEST_SUITE(Comparator_TestSuite, ComparatorTestTypes);

TYPED_TEST(Comparator_TestSuite, isEqual) {
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

  auto another = this->param.makeAnother();
  EXPECT_NE(tested, another)
      << this->param.name
      << " comparator tested ptr is the same as another ptr";
  EXPECT_NE(*tested, *another)
      << this->param.name
      << " comparator tested value is the same as another value";
}
