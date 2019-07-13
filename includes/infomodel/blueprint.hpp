#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include "device.hpp"
#include "deviceMetric.hpp"
#include <iostream>
#include <string>

class Blueprint {
private:
  Device *device;
  std::string generateRefId(std::string parentRefId, ElementType elementType);

public:
  virtual ~Blueprint() {}
  Blueprint(std::string refId, std::string name, std::string desc);

  Blueprint() {
    this->device =
        new Device("1234", "TestDevice", "A hardcoded device for testing only");

    try {
      AddDeviceGroup("TestElementGroup", "A hardcoded device element group");
    } catch (std::exception ex) {
      std::cout << "Could not add TestElementGroup. " << ex.what() << std::endl;
    }

    const std::string groupId =
        device->getDeviceElementGroup()->getReferenceId();

    try {
      AddSubElement(groupId, "TestObservableMetric",
                    "A hardcoded observableMetric Element",
                    ElementType::Observable, ValueType::ValueDataType::String);
    } catch (std::exception ex) {
      std::cout << "Could not add TestObservableMetric. " << ex.what()
                << std::endl;
    }

    try {
      AddSubElement(groupId, "TestWriteableMetric",
                    "A hardcoded writeableMetric", ElementType::Writable,
                    ValueType::ValueDataType::Integer);
    } catch (std::exception ex) {
      std::cout << "Could not add TestWriteableMetric. " << ex.what()
                << std::endl;
    }

    try {
      AddSubElement(groupId, "TestDeviceMetric", "A hardcoded deviceMetric",
                    ElementType::Readonly, ValueType::ValueDataType::Float);
    } catch (std::exception ex) {
      std::cout << "Could not add TestDeviceMetric. " << ex.what() << std::endl;
    }

    std::string subGroupId;
    try {
      subGroupId =
          AddSubElement(groupId, "TestSubElementGroup1",
                        "A hardcoded device element group", ElementType::Group);
    } catch (std::exception ex) {
      std::cout << "Could not add TestSubElementGroup1. " << ex.what()
                << std::endl;
    }

    std::cout << "SubgroupID is set to: " << subGroupId << std::endl;
    try {
      AddSubElement(subGroupId, "TestDeviceMetric2",
                    "A hardcoded deviceMetric ", ElementType::Readonly,
                    ValueType::ValueDataType::Float);
    } catch (std::exception ex) {
      std::cout << "Could not add TestDeviceMetric2. " << ex.what()
                << std::endl;
    }
  }

  std::string AddDeviceGroup(std::string name, std::string desc);
  std::string AddSubElement(std::string groupRefId, std::string name,
                            std::string desc, ElementType elementType);
  std::string AddSubElement(std::string groupRefId, std::string name,
                            std::string desc, ElementType elementType,
                            ValueType::ValueDataType valueType);

  Device *GetDevice();
};

#endif // !BLUEPRINT_H
