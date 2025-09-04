#include "GroupMock.hpp"
#include "Element.hpp"

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

vector<ElementPtr> toVector(const unordered_map<string, ElementPtr>& map) {
  vector<ElementPtr> result;

  if (!map.empty()) {
    result.reserve(map.size());
    for (const auto& [_, element] : map) {
      result.push_back(element);
    }
  }
  return result;
}

GroupMock::GroupMock(const string& id) : id_(id) {
  ON_CALL(*this, size).WillByDefault(Return(elements_.size()));
  ON_CALL(*this, asMap).WillByDefault(Return(elements_));
  ON_CALL(*this, asVector).WillByDefault(Return(toVector(elements_)));
  ON_CALL(*this, element).WillByDefault(Invoke(this, &GroupMock::getElement));
  ON_CALL(*this, visit).WillByDefault([this](const Group::Visitor& visitor) {
    for_each(elements_.begin(), elements_.end(), [&visitor](const auto& pair) {
      visitor(pair.second);
    });
  });
}

string GroupMock::generateID() {
  string new_id = (id_.back() == ':' ? id_ : id_ + '.') + to_string(next_id_);
  next_id_++;
  return new_id;
}

void GroupMock::addElement(const ElementPtr& element) {
  if (!element) {
    throw invalid_argument("Given element is empty");
  }
  if (element->id().compare(0, id_.length(), id_) != 0) {
    throw invalid_argument("Given element is not part of this group");
  }
  if (element->id() == id_) {
    throw invalid_argument("Given element has the same ID as this group");
  }

  auto sub_id = element->id().substr(id_.size() + 1);
  if (!elements_.try_emplace(sub_id, element).second) {
    throw logic_error(
        "Element with id " + element->id() + " is already in this group");
  }
  if (element->type() == ElementType::Group) {
    auto subgroup_id = sub_id.substr(0, sub_id.find('.'));
    if (!subgroups_
            .try_emplace(subgroup_id, dynamic_pointer_cast<Group>(element))
            .second) {
      throw logic_error("Subgroup with id " + subgroup_id + " for element " +
          element->id() + " is already indexed");
    }
  }
}

ElementPtr GroupMock::getElement(const string& ref_id) {
  if (ref_id.compare(0, id_.length(), id_) != 0) {
    // if referenced id does not start with this id, than the element is not in
    // this group
    throw ElementNotFound(ref_id);
  }
  if (ref_id == id_) {
    throw IDPointsThisGroup(ref_id);
  }

  auto sub_id = ref_id.substr(id_.size() + 1);
  auto group_marker = sub_id.find('.');
  if (group_marker == string::npos) {
    if (auto it = elements_.find(sub_id); it != elements_.end()) {
      return it->second;
    } else {
      // no element with given id exists
      throw ElementNotFound(ref_id);
    }
  } else {
    auto subgroup_id = sub_id.substr(0, group_marker);
    if (auto it = subgroups_.find(subgroup_id); it != subgroups_.end()) {
      return it->second->element(ref_id);
    } else {
      // no subgroup containing given id exists
      throw ElementNotFound(ref_id);
    }
  }
}

} // namespace Information_Model::testing
