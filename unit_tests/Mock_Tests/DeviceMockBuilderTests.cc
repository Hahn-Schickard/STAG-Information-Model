#include "DeviceMockBuilder.hpp"

#include "gtest/gtest.h"

#include <memory>
#include <string>

using namespace std;
using namespace Information_Model;
using namespace Information_Model::testing;

TEST(DeviceMockBuilderTests, throwsRuntimeErrorOnNoBase) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_THROW(builder->addDeviceElementGroup("1234", "Mocky", "Mocked device"),
      runtime_error);
  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST(DeviceMockBuilderTests, throwsRuntimeErrorOnOverridingBuild) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));
  EXPECT_THROW(builder->buildDeviceBase("4321", "Schmuck", "Overriding device"),
      runtime_error);
  EXPECT_NO_THROW(builder->getResult());
}

TEST(DeviceMockBuilderTests, canAddGroup) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string ref_id;
  string group_name = "Group 1";
  string group_desc = "Mocked Group 1";
  EXPECT_NO_THROW(
      ref_id = builder->addDeviceElementGroup(group_name, group_desc));

  EXPECT_EQ(ref_id, "1234:0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto group_element = base_group->getSubelement(ref_id);
  EXPECT_EQ(ref_id, group_element->getElementId());
  EXPECT_EQ(group_name, group_element->getElementName());
  EXPECT_EQ(group_desc, group_element->getElementDescription());
  EXPECT_TRUE(std::holds_alternative<NonemptyDeviceElementGroupPtr>(
      group_element->functionality));

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST(DeviceMockBuilderTests, canAddSubGroup) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string ref_id;
  string subgroup_ref_id;
  string group_name = "Sub Group 1";
  string group_desc = "Mocked Sub Group 1";
  EXPECT_NO_THROW(
      ref_id = builder->addDeviceElementGroup("group_name", "group_desc"));
  EXPECT_EQ(ref_id, "1234:0");

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  EXPECT_NO_THROW(subgroup_ref_id = builder->addDeviceElementGroup(
                      ref_id, group_name, group_desc));
  EXPECT_EQ(subgroup_ref_id, "1234:0.0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto group_element = base_group->getSubelement(subgroup_ref_id);
  EXPECT_EQ(subgroup_ref_id, group_element->getElementId());
  EXPECT_EQ(group_name, group_element->getElementName());
  EXPECT_EQ(group_desc, group_element->getElementDescription());
  EXPECT_TRUE(std::holds_alternative<NonemptyDeviceElementGroupPtr>(
      group_element->functionality));

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST(DeviceMockBuilderTests, canAddSubSubGroup) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string ref_id;
  string subgroup_ref_id;
  string subsubgroup_ref_id;
  string group_name = "Sub Sub Group 1";
  string group_desc = "Mocked Sub-sub Group 1";
  EXPECT_NO_THROW(
      ref_id = builder->addDeviceElementGroup("group_name", "group_desc"));
  EXPECT_EQ(ref_id, "1234:0");

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  EXPECT_NO_THROW(subgroup_ref_id = builder->addDeviceElementGroup(
                      ref_id, "Sub Group 1", "Mocked Sub Group 1"));
  EXPECT_EQ(subgroup_ref_id, "1234:0.0");

  EXPECT_NO_THROW(subsubgroup_ref_id = builder->addDeviceElementGroup(
                      subgroup_ref_id, group_name, group_desc));
  EXPECT_EQ(subsubgroup_ref_id, "1234:0.0.0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto group_element = base_group->getSubelement(subsubgroup_ref_id);
  EXPECT_EQ(subsubgroup_ref_id, group_element->getElementId());
  EXPECT_EQ(group_name, group_element->getElementName());
  EXPECT_EQ(group_desc, group_element->getElementDescription());
  EXPECT_TRUE(std::holds_alternative<NonemptyDeviceElementGroupPtr>(
      group_element->functionality));

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST(DeviceMockBuilderTests, canAddMetric) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string ref_id;
  string element_name = "Metric";
  string element_desc = "Mocked Metric";
  EXPECT_NO_THROW(ref_id = builder->addReadableMetric(
                      element_name, element_desc, DataType::DOUBLE));
  EXPECT_EQ(ref_id, "1234:0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto element = base_group->getSubelement(ref_id);
  EXPECT_EQ(ref_id, element->getElementId());
  EXPECT_EQ(element_name, element->getElementName());
  EXPECT_EQ(element_desc, element->getElementDescription());
  EXPECT_TRUE(
      std::holds_alternative<NonemptyMetricPtr>(element->functionality));

  auto metric = std::get<NonemptyMetricPtr>(element->functionality);
  auto mocked_metric = static_pointer_cast<MockMetric>(metric.base());

  EXPECT_EQ(DataType::DOUBLE, metric->getDataType());

  EXPECT_CALL(*mocked_metric, getMetricValue());
  try {
    metric->getMetricValue();
  } catch (exception& ex) {
    FAIL()
        << "Caught an unhandled exception while trying to read metric value: "
        << ex.what() << endl;
  }

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST(DeviceMockBuilderTests, canAddWritableMetric) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string ref_id;
  string element_name = "Writable Metric";
  string element_desc = "Mocked Writable Metric";
  EXPECT_NO_THROW(ref_id = builder->addWritableMetric(
                      element_name, element_desc, DataType::DOUBLE));
  EXPECT_EQ(ref_id, "1234:0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto element = base_group->getSubelement(ref_id);
  EXPECT_EQ(ref_id, element->getElementId());
  EXPECT_EQ(element_name, element->getElementName());
  EXPECT_EQ(element_desc, element->getElementDescription());

  auto writable_metric =
      std::get<NonemptyWritableMetricPtr>(element->functionality);
  auto mocked_metric =
      static_pointer_cast<MockWritableMetric>(writable_metric.base());

  EXPECT_EQ(DataType::DOUBLE, writable_metric->getDataType());

  EXPECT_CALL(*mocked_metric.get(), getMetricValue());
  try {
    writable_metric->getMetricValue();
  } catch (exception& ex) {
    FAIL()
        << "Caught an unhandled exception while trying to read metric value: "
        << ex.what() << endl;
  }

  EXPECT_CALL(*mocked_metric.get(), setMetricValue(::testing::_));
  try {
    // NOLINTNEXTLINE(readability-magic-numbers)
    mocked_metric->setMetricValue((double)20.2);
  } catch (exception& ex) {
    FAIL()
        << "Caught an unhandled exception while trying to write metric value: "
        << ex.what() << endl;
  }

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST(DeviceMockBuilderTests, canAddDefaultObservableMetric) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string ref_id;
  DeviceBuilderInterface::ObservedValue value_changed;
  string element_name = "Observable Metric";
  string element_desc = "Mocked Observable Metric";

  EXPECT_NO_THROW({
    auto result_pair = builder->addObservableMetric(
        element_name, element_desc, DataType::STRING);

    ref_id = result_pair.first;
    value_changed = result_pair.second;
  });

  EXPECT_EQ(ref_id, "1234:0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto element = base_group->getSubelement(ref_id);
  EXPECT_EQ(ref_id, element->getElementId());
  EXPECT_EQ(element_name, element->getElementName());
  EXPECT_EQ(element_desc, element->getElementDescription());

  auto metric = std::get<NonemptyObservableMetricPtr>(element->functionality);

  EXPECT_EQ(DataType::STRING, metric->getDataType());

  auto observer = std::make_shared<MockMetricObserver>(metric);
  EXPECT_CALL(*observer, handleEvent(::testing::_));
  EXPECT_TRUE(metric->hasListeners());
  EXPECT_EQ(1, metric->currentListenerCount());
  value_changed("New Value");
  this_thread::sleep_for(20ms); // wait until observer->handleEvent() was called

  observer.reset();
  EXPECT_FALSE(metric->hasListeners());
  EXPECT_EQ(0, metric->currentListenerCount());

  auto mocked = static_pointer_cast<MockObservableMetric>(metric.base());

  EXPECT_CALL(*mocked, getMetricValue());
  try {
    metric->getMetricValue();
  } catch (exception& ex) {
    FAIL()
        << "Caught an unhandled exception while trying to read metric value: "
        << ex.what() << endl;
  }

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST(DeviceMockBuilderTests, canAddDefaultSubObservableMetric) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string element_ref_id;
  string group_ref_id;
  string subgroup_ref_id;
  DeviceBuilderInterface::ObservedValue value_changed;
  string element_name = "Observable Metric";
  string element_desc = "Mocked Observable Metric";

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  EXPECT_NO_THROW(group_ref_id = builder->addDeviceElementGroup(
                      "group_name", "group_desc"));
  EXPECT_EQ(group_ref_id, "1234:0");

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  EXPECT_NO_THROW(subgroup_ref_id = builder->addDeviceElementGroup(
                      group_ref_id, "subgroup_name", "subgroup_desc"));
  EXPECT_EQ(subgroup_ref_id, "1234:0.0");

  EXPECT_NO_THROW({
    auto result_pair = builder->addObservableMetric(
        subgroup_ref_id, element_name, element_desc, DataType::STRING);

    element_ref_id = result_pair.first;
    value_changed = result_pair.second;
  });

  EXPECT_EQ(element_ref_id, "1234:0.0.0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto element = base_group->getSubelement(element_ref_id);
  EXPECT_EQ(element_ref_id, element->getElementId());
  EXPECT_EQ(element_name, element->getElementName());
  EXPECT_EQ(element_desc, element->getElementDescription());

  auto metric = std::get<NonemptyObservableMetricPtr>(element->functionality);

  EXPECT_EQ(DataType::STRING, metric->getDataType());

  auto observer = std::make_shared<MockMetricObserver>(metric);
  EXPECT_CALL(*observer, handleEvent(::testing::_));
  EXPECT_TRUE(metric->hasListeners());
  EXPECT_EQ(1, metric->currentListenerCount());
  value_changed("New Value");
  this_thread::sleep_for(20ms); // wait until observer->handleEvent() was called

  observer.reset();
  EXPECT_FALSE(metric->hasListeners());
  EXPECT_EQ(0, metric->currentListenerCount());

  auto mocked = static_pointer_cast<MockObservableMetric>(metric.base());

  EXPECT_CALL(*mocked, getMetricValue());
  try {
    metric->getMetricValue();
  } catch (exception& ex) {
    FAIL()
        << "Caught an unhandled exception while trying to read metric value: "
        << ex.what() << endl;
  }

  EXPECT_THROW(builder->getResult(), runtime_error);
}

struct Observed {
  ~Observed() {
    if (event_dispatcher_) {
      event_dispatcher_->join();
      event_dispatcher_.reset();
    }
    callback_ = nullptr;
  }

  void setCallback(DeviceBuilderInterface::ObservedValue&& callback) {
    callback_ = move(callback);
  }

  void isObserved(bool observed) {
    if (observed) {
      if (callback_) {
        // delaying event dispatch so observer has time to initialize
        event_dispatcher_ = make_unique<thread>(
            [](const DeviceBuilderInterface::ObservedValue& change_value) {
              this_thread::sleep_for(10ms);
              change_value("Hello World");
            },
            callback_);
      }
    }
  }

private:
  unique_ptr<thread> event_dispatcher_;
  DeviceBuilderInterface::ObservedValue callback_;
};

TEST(DeviceMockBuilderTests, canAddObservableMetric) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string ref_id;
  string element_name = "Observable Metric";
  string element_desc = "Mocked Observable Metric";

  auto observable = make_shared<Observed>();

  EXPECT_NO_THROW({
    auto result_pair = builder->addObservableMetric(element_name,
        element_desc,
        DataType::STRING,
        // we must bind to Observed* so dtor is properly called
        bind(&Observed::isObserved, observable.get(), placeholders::_1));

    ref_id = result_pair.first;
    observable->setCallback(move(result_pair.second));
  });

  EXPECT_EQ(ref_id, "1234:0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto element = base_group->getSubelement(ref_id);
  EXPECT_EQ(ref_id, element->getElementId());
  EXPECT_EQ(element_name, element->getElementName());
  EXPECT_EQ(element_desc, element->getElementDescription());

  auto observable_metric =
      std::get<NonemptyObservableMetricPtr>(element->functionality);

  EXPECT_EQ(DataType::STRING, observable_metric->getDataType());

  auto observer = std::make_shared<MockMetricObserver>(observable_metric);
  EXPECT_CALL(*observer, handleEvent(::testing::_));
  EXPECT_TRUE(observable_metric->hasListeners());
  EXPECT_EQ(1, observable_metric->currentListenerCount());
  this_thread::sleep_for(50ms); // wait until observer->handleEvent() was called

  observer.reset();
  EXPECT_FALSE(observable_metric->hasListeners());
  EXPECT_EQ(0, observable_metric->currentListenerCount());

  EXPECT_THROW(builder->getResult(), runtime_error);
  observable.reset();
}

TEST(DeviceMockBuilderTests, canAddSubMetric) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string element_ref_id;
  string group_ref_id;
  string subgroup_ref_id;
  string element_name = "Metric";
  string element_desc = "Mocked Metric";

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  EXPECT_NO_THROW(group_ref_id = builder->addDeviceElementGroup(
                      "group_name", "group_desc"));
  EXPECT_EQ(group_ref_id, "1234:0");

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  EXPECT_NO_THROW(subgroup_ref_id = builder->addDeviceElementGroup(
                      group_ref_id, "subgroup_name", "subgroup_desc"));
  EXPECT_EQ(subgroup_ref_id, "1234:0.0");

  EXPECT_NO_THROW(
      // NOLINTNEXTLINE(readability-suspicious-call-argument)
      element_ref_id = builder->addReadableMetric(
          subgroup_ref_id, element_name, element_desc, DataType::DOUBLE));
  EXPECT_EQ(element_ref_id, "1234:0.0.0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto element = base_group->getSubelement(element_ref_id);
  EXPECT_EQ(element_ref_id, element->getElementId());
  EXPECT_EQ(element_name, element->getElementName());
  EXPECT_EQ(element_desc, element->getElementDescription());
  EXPECT_TRUE(
      std::holds_alternative<NonemptyMetricPtr>(element->functionality));

  auto metric = std::get<NonemptyMetricPtr>(element->functionality);
  auto mocked_metric = static_pointer_cast<MockMetric>(metric.base());

  EXPECT_EQ(DataType::DOUBLE, metric->getDataType());

  EXPECT_CALL(*mocked_metric, getMetricValue());
  try {
    metric->getMetricValue();
  } catch (exception& ex) {
    FAIL()
        << "Caught an unhandled exception while trying to read metric value: "
        << ex.what() << endl;
  }

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST(DeviceMockBuilderTests, canAddSubWritableMetric) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string element_ref_id;
  string group_ref_id;
  string subgroup_ref_id;
  string element_name = "Writable Metric";
  string element_desc = "Mocked Writable Metric";

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  EXPECT_NO_THROW(group_ref_id = builder->addDeviceElementGroup(
                      "group_name", "group_desc"));
  EXPECT_EQ(group_ref_id, "1234:0");

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  EXPECT_NO_THROW(subgroup_ref_id = builder->addDeviceElementGroup(
                      group_ref_id, "subgroup_name", "subgroup_desc"));
  EXPECT_EQ(subgroup_ref_id, "1234:0.0");

  EXPECT_NO_THROW(
      // NOLINTNEXTLINE(readability-suspicious-call-argument)
      element_ref_id = builder->addWritableMetric(
          subgroup_ref_id, element_name, element_desc, DataType::DOUBLE));
  EXPECT_EQ(element_ref_id, "1234:0.0.0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto element = base_group->getSubelement(element_ref_id);
  EXPECT_EQ(element_ref_id, element->getElementId());
  EXPECT_EQ(element_name, element->getElementName());
  EXPECT_EQ(element_desc, element->getElementDescription());

  auto writable_metric =
      std::get<NonemptyWritableMetricPtr>(element->functionality);
  auto mocked_metric =
      static_pointer_cast<MockWritableMetric>(writable_metric.base());

  EXPECT_EQ(DataType::DOUBLE, writable_metric->getDataType());

  EXPECT_CALL(*mocked_metric.get(), getMetricValue());
  try {
    writable_metric->getMetricValue();
  } catch (exception& ex) {
    FAIL()
        << "Caught an unhandled exception while trying to read metric value: "
        << ex.what() << endl;
  }

  EXPECT_CALL(*mocked_metric.get(), setMetricValue(::testing::_));
  try {
    // NOLINTNEXTLINE(readability-magic-numbers)
    mocked_metric->setMetricValue((double)20.2);
  } catch (exception& ex) {
    FAIL()
        << "Caught an unhandled exception while trying to write metric value: "
        << ex.what() << endl;
  }

  EXPECT_THROW(builder->getResult(), runtime_error);
}

TEST(DeviceMockBuilderTests, canAddSubObservableMetric) {
  auto builder = make_shared<DeviceMockBuilder>();

  EXPECT_NO_THROW(builder->buildDeviceBase("1234", "Mocky", "Mocked device"));

  string element_ref_id;
  string group_ref_id;
  string subgroup_ref_id;
  string element_name = "Observable Metric";
  string element_desc = "Mocked Observable Metric";

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  EXPECT_NO_THROW(group_ref_id = builder->addDeviceElementGroup(
                      "group_name", "group_desc"));
  EXPECT_EQ(group_ref_id, "1234:0");

  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  EXPECT_NO_THROW(subgroup_ref_id = builder->addDeviceElementGroup(
                      group_ref_id, "subgroup_name", "subgroup_desc"));
  EXPECT_EQ(subgroup_ref_id, "1234:0.0");

  auto observable = make_shared<Observed>();

  EXPECT_NO_THROW({
    // NOLINTNEXTLINE(readability-suspicious-call-argument)
    auto result_pair = builder->addObservableMetric(subgroup_ref_id,
        element_name,
        element_desc,
        DataType::STRING,
        // we must bind to Observed* so dtor is properly called
        bind(&Observed::isObserved, observable.get(), placeholders::_1));

    element_ref_id = result_pair.first;
    observable->setCallback(move(result_pair.second));
  });
  EXPECT_EQ(element_ref_id, "1234:0.0.0");

  DevicePtr device;
  EXPECT_NO_THROW(device = move(builder->getResult()));

  auto base_group = device->getDeviceElementGroup();
  EXPECT_EQ("1234", device->getElementId());
  EXPECT_EQ("Mocky", device->getElementName());
  EXPECT_EQ("Mocked device", device->getElementDescription());
  EXPECT_EQ(1, base_group->getSubelements().size());

  auto element = base_group->getSubelement(element_ref_id);
  EXPECT_EQ(element_ref_id, element->getElementId());
  EXPECT_EQ(element_name, element->getElementName());
  EXPECT_EQ(element_desc, element->getElementDescription());
  EXPECT_TRUE(std::holds_alternative<NonemptyObservableMetricPtr>(
      element->functionality));

  auto metric = std::get<NonemptyObservableMetricPtr>(element->functionality);
  auto observable_metric =
      std::get<NonemptyObservableMetricPtr>(element->functionality);

  EXPECT_EQ(DataType::STRING, observable_metric->getDataType());

  auto observer = std::make_shared<MockMetricObserver>(observable_metric);
  EXPECT_CALL(*observer, handleEvent(::testing::_));
  EXPECT_TRUE(observable_metric->hasListeners());
  EXPECT_EQ(1, observable_metric->currentListenerCount());
  this_thread::sleep_for(50ms); // wait until observer->handleEvent() was called

  observer.reset();
  EXPECT_FALSE(observable_metric->hasListeners());
  EXPECT_EQ(0, observable_metric->currentListenerCount());

  EXPECT_THROW(builder->getResult(), runtime_error);
  observable.reset();
}