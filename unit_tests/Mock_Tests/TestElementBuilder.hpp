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
DeviceElementGroupPtr TestElementBuilder::build<DeviceElementGroup>(
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
DevicePtr TestElementBuilder::build<Device>(TestElementInfoPtr param) {
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
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_TEST_ELEMENT_BUILDER_HPP