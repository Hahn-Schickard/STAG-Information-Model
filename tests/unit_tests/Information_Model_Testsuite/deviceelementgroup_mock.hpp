#ifndef __DEVICEELEMENTGROUP_MOCK_HPP
#define __DEVICEELEMENTGROUP_MOCK_HPP

#include "gmock/gmock.h"

#include "DeviceElementGroup.hpp"

class MockListener : public Information_Model::DeviceElementGroup {
public:
  MOCK_METHOD(std::vector<std::shared_ptr<DeviceElement>>, getSubelements, (),
              (override));
  MOCK_METHOD(Information_Model::DeviceElement *, getSubelement,
              (const std::string REF_ID), (override));
  MOCK_METHOD(unsigned int, getNumericElementId, (), (override));
};

#endif //__DEVICEELEMENTGROUP_MOCK_HPP