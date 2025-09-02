
#ifndef __STAG_INFORMATION_MODEL_DEVICE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_DEVICE_MOCK_HPP
#include "Group.hpp"

#include <gmock/gmock.h>

namespace Information_Model::testing {

struct GroupMock : public Group {
  GroupMock() = default;

  ~GroupMock() override = default;

  MOCK_METHOD(Group::Elements, getElements, (), (const final));
  MOCK_METHOD(ElementPtr, getElement, (const std::string&), (const final));
};

using GroupMockPtr = std::shared_ptr<GroupMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_DEVICE_MOCK_HPP