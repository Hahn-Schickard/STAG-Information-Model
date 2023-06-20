#ifndef __INFORMATION_MODEL_TEST_ELEMENT_BUILDER_HPP
#define __INFORMATION_MODEL_TEST_ELEMENT_BUILDER_HPP

#include "Device.hpp"
#include "mocks/DeviceMockBuilder.hpp"

#include <vector>

namespace Information_Model {
namespace testing {
struct ElementMetaInfo {
  ElementMetaInfo() = default;
  ElementMetaInfo(const std::string& ref_id,
      const std::string& name,
      const std::string& desc)
      : ref_ID_(ref_id), name_(name), desc_(desc) {}

  std::string ref_ID_;
  std::string name_;
  std::string desc_;
};

struct TestElementInfo {
  TestElementInfo(ElementMetaInfo meta_info_,
      DeviceMockBuilder::Functionality functionality_ =
          DeviceMockBuilder::Functionality(),
      std::vector<std::shared_ptr<TestElementInfo>> subelements_ = {})
      : meta_info(meta_info_), functionality(functionality_),
        subelements(subelements_) {}

  // NOLINTNEXTLINE(readability-identifier-naming)
  ElementMetaInfo meta_info;
  // NOLINTNEXTLINE(readability-identifier-naming)
  const DeviceMockBuilder::Functionality functionality;
  // NOLINTNEXTLINE(readability-identifier-naming)
  const std::vector<std::shared_ptr<TestElementInfo>> subelements;
};

using TestElementInfoPtr = std::shared_ptr<TestElementInfo>;

struct TestElementBuilder : DeviceMockBuilder {
  TestElementBuilder()
      : TestElementBuilder(ElementMetaInfo("12345", "Mocky", "It's mocked")) {}

  TestElementBuilder(ElementMetaInfo device_info) {
    buildDeviceBase(device_info.ref_ID_, device_info.name_, device_info.desc_);
  }

  void resetBuilder(ElementMetaInfo device_info = ElementMetaInfo(
                        "12345", "Mocky", "It's mocked")) {
    device_.reset();
    buildDeviceBase(device_info.ref_ID_, device_info.name_, device_info.desc_);
  }

  template <typename Element>
  std::shared_ptr<Element> build(TestElementInfoPtr /* param */) {
    throw std::logic_error(
        "Element builder can not build the given element type");
  }
};

template <>
inline DeviceElementPtr TestElementBuilder::build<DeviceElement>(
    TestElementInfoPtr param) {
  auto group = getGroupImplementation(param->meta_info.ref_ID_);
  auto ref_id = group->generateReferenceID();
  // allocate ptr, so we can declare variant with Nonempty values
  std::unique_ptr<DeviceElement::SpecificInterface> interface;
  if (param->functionality.type() != ElementType::GROUP) {
    interface = std::make_unique<DeviceElement::SpecificInterface>(
        buildSpecificInterface(param->functionality));
  } else {
    auto group =
        std::make_shared<::testing::NiceMock<MockDeviceElementGroup>>(ref_id);
    auto nonempty_group = NonemptyDeviceElementGroupPtr(std::move(group));
    interface = std::make_unique<DeviceElement::SpecificInterface>(
        std::move(nonempty_group));
  }
  auto result = makeDeviceElement( //
      ref_id,
      param->meta_info.name_,
      param->meta_info.desc_,
      std::move(*interface));
  group->addDeviceElement(NonemptyDeviceElementPtr(result));
  return result;
}

template <>
inline DeviceElementGroupPtr TestElementBuilder::build<DeviceElementGroup>(
    TestElementInfoPtr param) {
  auto element = build<DeviceElement>(param);
  for (auto subelement_info : param->subelements) {
    // assign new group id as parent
    subelement_info->meta_info.ref_ID_ = element->getElementId();
    if (subelement_info->functionality.type() != ElementType::GROUP) {
      auto subelement = build<DeviceElement>(subelement_info);
    } else {
      auto subgroup = build<DeviceElementGroup>(subelement_info);
    }
    // subelements are automaticaly added to this group in
    // build<DeviceElement>() call, thus there is no need to call
    // group->addDeviceElement(NonemptyDeviceElementPtr(subelement))
  }
  auto group = std::get<NonemptyDeviceElementGroupPtr>(element->functionality);
  return group.base();
}

template <>
inline DevicePtr TestElementBuilder::build<Device>(TestElementInfoPtr param) {
  auto root_group = build<DeviceElementGroup>(param);
  for (auto element : param->subelements) {
    if (element->functionality.type() != ElementType::GROUP) {
      auto subelement = build<DeviceElement>(element);
    } else {
      auto subgroup = build<DeviceElementGroup>(element);
    }
  }
  return std::move(device_);
}

inline ElementMetaInfo makeReadableInfo(const std::string& parent_id = "",
    const std::string& name = "readable",
    const std::string& desc = "readable element mock") {
  return ElementMetaInfo(parent_id, name, desc);
}

inline TestElementInfoPtr makeReadable(DataType return_type = DataType::BOOLEAN,
    const ElementMetaInfo& meta_info = makeReadableInfo()) {
  auto functionality = DeviceMockBuilder::Functionality(
      return_type, DeviceBuilderInterface::Reader());
  return std::make_shared<TestElementInfo>(meta_info, functionality);
}

inline TestElementInfoPtr makeReadable(const ElementMetaInfo& meta_info) {
  return makeReadable(DataType::BOOLEAN, meta_info);
}

inline ElementMetaInfo makeWriteOnlyInfo(const std::string& parent_id = "",
    const std::string& name = "write-only",
    const std::string& desc = "write-only element mock") {
  return ElementMetaInfo(parent_id, name, desc);
}

inline TestElementInfoPtr makeWriteOnly(
    DataType return_type = DataType::BOOLEAN,
    const ElementMetaInfo& meta_info = makeWriteOnlyInfo()) {
  auto functionality = DeviceMockBuilder::Functionality(
      return_type, DeviceBuilderInterface::Writer());
  return std::make_shared<TestElementInfo>(meta_info, functionality);
}

inline TestElementInfoPtr makeWriteOnly(const ElementMetaInfo& meta_info) {
  return makeWriteOnly(DataType::BOOLEAN, meta_info);
}

inline ElementMetaInfo makeWritableInfo(const std::string& parent_id = "",
    const std::string& name = "read&writable",
    const std::string& desc = "read&write element mock") {
  return ElementMetaInfo(parent_id, name, desc);
}

inline TestElementInfoPtr makeWritable(DataType return_type = DataType::BOOLEAN,
    const ElementMetaInfo& meta_info = makeWritableInfo()) {
  auto functionality = DeviceMockBuilder::Functionality(return_type,
      DeviceBuilderInterface::Reader(),
      DeviceBuilderInterface::Writer());
  return std::make_shared<TestElementInfo>(meta_info, functionality);
}

inline TestElementInfoPtr makeWritable(const ElementMetaInfo& meta_info) {
  return makeWritable(DataType::BOOLEAN, meta_info);
}

inline ElementMetaInfo makeExecutableInfo(const std::string& parent_id = "",
    const std::string& name = "executable",
    const std::string& desc = "executable element mock") {
  return ElementMetaInfo(parent_id, name, desc);
}

inline TestElementInfoPtr makeExecutable(
    DataType return_type = DataType::BOOLEAN,
    Function::ParameterTypes supported_params = {},
    const ElementMetaInfo& meta_info = makeExecutableInfo()) {
  auto functionality = DeviceMockBuilder::Functionality(return_type,
      DeviceBuilderInterface::Executor(),
      DeviceBuilderInterface::Canceler(),
      supported_params);
  return std::make_shared<TestElementInfo>(meta_info, functionality);
}

inline TestElementInfoPtr makeExecutable(const ElementMetaInfo& meta_info) {
  return makeExecutable(DataType::BOOLEAN, {}, meta_info);
}

inline ElementMetaInfo makeEmptyGroupInfo(const std::string& parent_ref_id = "",
    const std::string& name = "empty-group",
    const std::string& desc = "Group element mock without any elements") {
  return ElementMetaInfo(parent_ref_id, name, desc);
}

inline TestElementInfoPtr makeEmptyGroup(
    const ElementMetaInfo& meta_info = makeEmptyGroupInfo()) {
  return std::make_shared<TestElementInfo>(meta_info);
}

enum class TestElementType {
  READABLE,
  WRITE_ONLY,
  WRITABLE,
  OBSERVABLE,
  EXECUTABLE
};

inline std::string toString(TestElementType type) {
  switch (type) {
  case TestElementType::READABLE: {
    return "READABLE";
  }
  case TestElementType::WRITE_ONLY: {
    return "WRITE_ONLY";
  }
  case TestElementType::WRITABLE: {
    return "WRITABLE";
  }
  case TestElementType::EXECUTABLE: {
    return "EXECUTABLE";
  }
  default: { return "UNKOWN"; }
  }
}

inline TestElementInfoPtr makeSubElement(
    TestElementType type, const std::string& parent_ref_id) {
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
  default: {
    throw std::logic_error(
        "Element Type " + toString(type) + " is not implemented");
  }
  }
}

inline std::vector<TestElementInfoPtr> makeSubElements(
    std::vector<TestElementType> subelement_types,
    const std::string& parent_ref_id) {
  std::vector<TestElementInfoPtr> subelements;
  for (auto subelement_type : subelement_types) {
    subelements.push_back(makeSubElement(subelement_type, parent_ref_id));
  }
  return subelements;
}

inline ElementMetaInfo makeSingleLevelGroupInfo(
    const std::string& parent_ref_id = "",
    const std::string& name = "1-level-group",
    const std::string& desc =
        "Group element mock containing only Metric/Function elements") {
  return ElementMetaInfo(parent_ref_id, name, desc);
}

inline TestElementInfoPtr makeSingleLevelGroup(
    const ElementMetaInfo& meta_info = makeSingleLevelGroupInfo(),
    std::vector<TestElementType> subelement_types =
        {// clang-format off
          TestElementType::READABLE,
          TestElementType::WRITE_ONLY,
          TestElementType::WRITABLE,
          TestElementType::EXECUTABLE
        } // clang-format on
) {
  return std::make_shared<TestElementInfo>(meta_info,
      DeviceMockBuilder::Functionality(),
      makeSubElements(subelement_types, meta_info.ref_ID_));
}

inline ElementMetaInfo makeNestedGroupInfo(
    const std::string& parent_ref_id = "",
    const std::string& name = "nested-group",
    const std::string& desc = "Nested Group element mock") {
  return ElementMetaInfo(parent_ref_id, name, desc);
}

inline TestElementInfoPtr makeNestedGroup(
    const ElementMetaInfo& meta_info = makeNestedGroupInfo(),
    std::vector<TestElementType> subsubelement_types =
        {TestElementType::READABLE,
            TestElementType::WRITE_ONLY,
            TestElementType::WRITABLE,
            TestElementType::EXECUTABLE},
    std::vector<TestElementType> subelement_types = {TestElementType::READABLE,
        TestElementType::WRITE_ONLY,
        TestElementType::WRITABLE,
        TestElementType::EXECUTABLE}) {
  std::vector<TestElementInfoPtr> elements = {
      makeSingleLevelGroup(makeSingleLevelGroupInfo("0"), subsubelement_types)};
  auto subelements = makeSubElements(subelement_types, meta_info.ref_ID_);
  elements.insert(elements.end(), subelements.begin(), subelements.end());
  return std::make_shared<TestElementInfo>(
      meta_info, DeviceMockBuilder::Functionality(), elements);
}
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_TEST_ELEMENT_BUILDER_HPP