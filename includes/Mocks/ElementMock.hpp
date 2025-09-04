
#ifndef __STAG_INFORMATION_MODEL_ELEMENT_MOCK_HPP
#define __STAG_INFORMATION_MODEL_ELEMENT_MOCK_HPP
#include "CallableMock.hpp"
#include "Element.hpp"
#include "GroupMock.hpp"
#include "MetaInfoMock.hpp"
#include "ObservableMock.hpp"
#include "ReadableMock.hpp"
#include "WritableMock.hpp"

namespace Information_Model::testing {

struct ElementMock : virtual public Element, public MetaInfoMock {
  ElementMock(const GroupMockPtr& group,
      const std::string& id,
      const std::optional<FullMetaInfo>& meta = std::nullopt);

  ElementMock(const ReadableMockPtr& readable,
      const std::string& id,
      const std::optional<FullMetaInfo>& meta = std::nullopt);

  ElementMock(const WritableMockPtr& writable,
      const std::string& id,
      const std::optional<FullMetaInfo>& meta = std::nullopt);

  ElementMock(const ObservableMockPtr& observable,
      const std::string& id,
      const std::optional<FullMetaInfo>& meta = std::nullopt);

  ElementMock(const CallableMockPtr& callable,
      const std::string& id,
      const std::optional<FullMetaInfo>& meta = std::nullopt);

  ~ElementMock() override = default;

  MOCK_METHOD(ElementType, type, (), (const final));
  MOCK_METHOD(ElementFunction, function, (), (const final));

private:
  void setOnCall();

  ElementType type_;
  ElementFunction function_;
};

using ElementMockPtr = std::shared_ptr<ElementMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_ELEMENT_MOCK_HPP