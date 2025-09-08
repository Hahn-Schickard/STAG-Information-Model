#include "DeviceMock.hpp"

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

DeviceMock::DeviceMock(
    const string& base_id, const optional<FullMetaInfo>& meta)
    : MetaInfoMock(base_id, meta),
      group_(make_shared<NiceMock<GroupMock>>(base_id)) {
  ON_CALL(*this, group).WillByDefault(Return(group_));
  ON_CALL(*this, element).WillByDefault(Invoke(group_.get(), &Group::element));
}

string DeviceMock::generateID() { return group_->generateID(); }

void DeviceMock::addElement(const ElementPtr& element) {
  group_->addElement(element);
}

} // namespace Information_Model::testing