#include "DeviceMockBuilder.hpp"

#include "gtest/gtest.h"

#include <memory>
#include <string>

using namespace std;
using namespace Information_Model;
using namespace Information_Model::testing;

class DeviceMockBuilderTests : public ::testing::Test {
protected:
  // NOLINTNEXTLINE
  void SetUp() { builder = make_shared<DeviceMockBuilder>(); }

  shared_ptr<DeviceMockBuilder> builder;
};

TEST_F(DeviceMockBuilderTests, throwsRuntimeErrorOnNoBase) {
  EXPECT_THROW(builder->addDeviceElementGroup("1234", "Mocky", "Mocked devie"),
               runtime_error);
}

TEST_F(DeviceMockBuilderTests, canAddGroup) {
  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked devie"));

  string ref_id;
  string group_name = "Group 1";
  string group_desc = "Mocked Group 1";
  EXPECT_NO_THROW(ref_id =
                      builder->addDeviceElementGroup(group_name, group_desc));

  EXPECT_EQ(ref_id, "1234:0");

  DevicePtr device;
  EXPECT_NO_THROW(device = builder->getResult());

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234:", base_group->getElementId());
  EXPECT_EQ("Mocky", base_group->getElementName());
  EXPECT_EQ("Mocked devie", base_group->getElementDescription());
  EXPECT_EQ(ElementType::GROUP, base_group->getElementType());

  auto group_element = base_group->getSubelement(ref_id);
  EXPECT_EQ(ref_id, group_element->getElementId());
  EXPECT_EQ(group_name, group_element->getElementName());
  EXPECT_EQ(group_desc, group_element->getElementDescription());
  EXPECT_EQ(ElementType::GROUP, group_element->getElementType());

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST_F(DeviceMockBuilderTests, canAddSubGroup) {
  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked devie"));

  string ref_id;
  string subgroup_ref_id;
  string group_name = "Sub Group 1";
  string group_desc = "Mocked Sub Group 1";
  EXPECT_NO_THROW(
      ref_id = builder->addDeviceElementGroup("group_name", "group_desc"));
  EXPECT_EQ(ref_id, "1234:0");

  EXPECT_NO_THROW(subgroup_ref_id = builder->addDeviceElementGroup(
                      ref_id, group_name, group_desc));
  EXPECT_EQ(subgroup_ref_id, "1234:0.0");

  DevicePtr device;
  EXPECT_NO_THROW(device = builder->getResult());

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234:", base_group->getElementId());
  EXPECT_EQ("Mocky", base_group->getElementName());
  EXPECT_EQ("Mocked devie", base_group->getElementDescription());
  EXPECT_EQ(ElementType::GROUP, base_group->getElementType());

  auto group_element = base_group->getSubelement(subgroup_ref_id);
  EXPECT_EQ(subgroup_ref_id, group_element->getElementId());
  EXPECT_EQ(group_name, group_element->getElementName());
  EXPECT_EQ(group_desc, group_element->getElementDescription());
  EXPECT_EQ(ElementType::GROUP, group_element->getElementType());

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST_F(DeviceMockBuilderTests, canAddMetric) {
  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked devie"));

  string ref_id;
  string element_name = "Metric";
  string element_desc = "Mocked Metric";
  EXPECT_NO_THROW(
      ref_id = builder->addReadableMetric(element_name, element_desc,
                                          DataType::DOUBLE, ReadFunctor()));
  EXPECT_EQ(ref_id, "1234:0");

  DevicePtr device;
  EXPECT_NO_THROW(device = builder->getResult());

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234:", base_group->getElementId());
  EXPECT_EQ("Mocky", base_group->getElementName());
  EXPECT_EQ("Mocked devie", base_group->getElementDescription());
  EXPECT_EQ(ElementType::GROUP, base_group->getElementType());

  auto element = base_group->getSubelement(ref_id);
  EXPECT_EQ(ref_id, element->getElementId());
  EXPECT_EQ(element_name, element->getElementName());
  EXPECT_EQ(element_desc, element->getElementDescription());
  EXPECT_EQ(ElementType::READABLE, element->getElementType());

  auto mocked_metric = static_pointer_cast<MockMetric>(element);

  EXPECT_CALL(*mocked_metric.get(), getDataType());
  mocked_metric->getDataType();

  EXPECT_CALL(*mocked_metric.get(), getMetricValue());
  mocked_metric->getMetricValue();

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST_F(DeviceMockBuilderTests, canAddWritableMetric) {
  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked devie"));

  string ref_id;
  string element_name = "Writable Metric";
  string element_desc = "Mocked Writable Metric";
  EXPECT_NO_THROW(ref_id = builder->addWritableMetric(
                      element_name, element_desc, DataType::DOUBLE,
                      ReadFunctor(), WriteFunctor()));
  EXPECT_EQ(ref_id, "1234:0");

  DevicePtr device;
  EXPECT_NO_THROW(device = builder->getResult());

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234:", base_group->getElementId());
  EXPECT_EQ("Mocky", base_group->getElementName());
  EXPECT_EQ("Mocked devie", base_group->getElementDescription());
  EXPECT_EQ(ElementType::GROUP, base_group->getElementType());

  auto element = base_group->getSubelement(ref_id);
  EXPECT_EQ(ref_id, element->getElementId());
  EXPECT_EQ(element_name, element->getElementName());
  EXPECT_EQ(element_desc, element->getElementDescription());
  EXPECT_EQ(ElementType::WRITABLE, element->getElementType());

  auto mocked_metric = static_pointer_cast<MockWritableMetric>(element);

  EXPECT_CALL(*mocked_metric.get(), getDataType());
  mocked_metric->getDataType();

  EXPECT_CALL(*mocked_metric.get(), getMetricValue());
  mocked_metric->getMetricValue();

  EXPECT_CALL(*mocked_metric.get(), setMetricValue(::testing::_));
  mocked_metric->setMetricValue((double)20.2);

  EXPECT_THROW(builder->getResult(), runtime_error);
}