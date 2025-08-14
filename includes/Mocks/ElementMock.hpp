
#ifndef __STAG_INFORMATION_MODEL_DEVICE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_DEVICE_MOCK_HPP
#include "Element.hpp"

#include "CallableMock.hpp"
#include "GroupMock.hpp"
#include "MetaInfoMock.hpp"
#include "ObservableMock.hpp"
#include "ReadableMock.hpp"
#include "WritableMock.hpp"

namespace Information_Model::testing {

struct ElementMock : virtual public Element, public MetaInfoMock {
  ElementMock() = default;

  ElementMock(ElementType type) : type_(type) {}

  ElementMock(const GroupMockPtr& mock_function)
      : type_(ElementType::Group), function_(mock_function) {}

  ElementMock(const ReadableMockPtr& mock_function)
      : type_(ElementType::Readable), function_(mock_function) {}

  ElementMock(const WritableMockPtr& mock_function)
      : type_(ElementType::Writable), function_(mock_function) {}

  ElementMock(const ObservableMockPtr& mock_function)
      : type_(ElementType::Observable), function_(mock_function) {}

  ElementMock(const CallableMockPtr& mock_function)
      : type_(ElementType::Callable), function_(mock_function) {}

  ~ElementMock() override = default;

  MOCK_METHOD(ElementType, type, (), (const final));
  MOCK_METHOD(ElementFunction, function, (), (const final));

private:
  std::optional<ElementType> type_;
  std::optional<ElementFunction> function_;
};

using ElementMockPtr = std::shared_ptr<ElementMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_DEVICE_MOCK_HPP