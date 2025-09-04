#include "ElementMock.hpp"

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

ElementMock::ElementMock(ElementType type,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(type) {}

ElementMock::ElementMock(const GroupMockPtr& group,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(ElementType::Group), function_(group) {}

ElementMock::ElementMock(const ReadableMockPtr& readable,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(ElementType::Readable),
      function_(readable) {}

ElementMock::ElementMock(const WritableMockPtr& writable,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(ElementType::Writable),
      function_(writable) {}

ElementMock::ElementMock(const ObservableMockPtr& observable,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(ElementType::Observable),
      function_(observable) {}

ElementMock::ElementMock(const CallableMockPtr& callable,
    const std::string& id,
    const std::optional<FullMetaInfo>& meta)
    : MetaInfoMock(id, meta), type_(ElementType::Callable),
      function_(callable) {}

} // namespace Information_Model::testing
