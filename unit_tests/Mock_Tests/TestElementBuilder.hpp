#ifndef __INFORMATION_MODEL_TEST_ELEMENT_BUILDER_HPP
#define __INFORMATION_MODEL_TEST_ELEMENT_BUILDER_HPP

#include "Device.hpp"
#include "mocks/DeviceMockBuilder.hpp"

namespace Information_Model {
namespace testing {
struct ElementMetaInfo {
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
      DeviceMockBuilder::Functionality functionality_,
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

  template <typename Element>
  std::shared_ptr<Element> build(TestElementInfoPtr /* param */) {
    throw std::logic_error(
        "Element builder can not build the given element type");
  }
};

template <>
DeviceElementPtr TestElementBuilder::build<DeviceElement>(
    TestElementInfoPtr param) {
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
        std::make_shared<::testing::NiceMock<MockDeviceElementGroup>>(ref_id);
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
    TestElementInfoPtr param) {
  auto base_group = getGroupImplementation(param->meta_info.ref_ID_);
  auto ref_id = base_group->generateReferenceID();
  auto group =
      std::make_shared<::testing::NiceMock<MockDeviceElementGroup>>(ref_id);
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
DevicePtr TestElementBuilder::build<Device>(TestElementInfoPtr param) {
  for (auto element : param->subelements) {
    build<DeviceElement>(element);
  }
  return std::move(device_);
}
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_TEST_ELEMENT_BUILDER_HPP