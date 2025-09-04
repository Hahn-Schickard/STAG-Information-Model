#include "ElementMock.hpp"

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

ElementMock::ElementMock(const GroupMockPtr& group,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(ElementType::Group), function_(group) {
  setOnCall();
}

ElementMock::ElementMock(const ReadableMockPtr& readable,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(ElementType::Readable),
      function_(readable) {
  setOnCall();
}

ElementMock::ElementMock(const WritableMockPtr& writable,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(ElementType::Writable),
      function_(writable) {
  setOnCall();
}

ElementMock::ElementMock(const ObservableMockPtr& observable,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(ElementType::Observable),
      function_(observable) {
  setOnCall();
}

ElementMock::ElementMock(const CallableMockPtr& callable,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(ElementType::Callable),
      function_(callable) {
  setOnCall();
}

void ElementMock::setOnCall() {
  ON_CALL(*this, type).WillByDefault(Return(type_));
  ON_CALL(*this, function).WillByDefault(Return(function_));
}

} // namespace Information_Model::testing
