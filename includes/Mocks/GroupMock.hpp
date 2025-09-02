
#ifndef __STAG_INFORMATION_MODEL_DEVICE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_DEVICE_MOCK_HPP
#include "Group.hpp"

#include <gmock/gmock.h>

namespace Information_Model::testing {

struct GroupMock : public Group {
  explicit GroupMock(const std::string& id);

  ~GroupMock() override = default;

  MOCK_METHOD(Group::Elements, getElements, (), (const final));
  MOCK_METHOD(ElementPtr, getElement, (const std::string&), (const final));

  std::string generateID();
  void addDeviceElement(const ElementPtr& element);

private:
  ElementPtr _getElement(const std::string& ref_id);

  std::unordered_map<std::string, ElementPtr> elements_;
  std::unordered_map<std::string, GroupPtr> subgroups_;
  size_t next_id_;
  std::string id_;
};

using GroupMockPtr = std::shared_ptr<GroupMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_DEVICE_MOCK_HPP