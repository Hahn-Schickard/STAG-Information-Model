#ifndef __STAG_INFORMATION_MODEL_READABLE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_READABLE_MOCK_HPP
#include "Readable.hpp"

#include <gmock/gmock.h>
#include <optional>

namespace Information_Model::testing {

struct ReadableMock : virtual public Readable {
  using ReadCallback = std::function<DataVariant()>;

  ReadableMock() = default;

  explicit ReadableMock(DataType type);

  explicit ReadableMock(const DataVariant& value);

  ReadableMock(DataType type, const ReadCallback& read_cb);

  ~ReadableMock() override = default;

  void updateType(DataType type);

  void updateCallback(const ReadCallback& read_cb);

  void updateValue(const DataVariant& value);

  MOCK_METHOD(DataType, dataType, (), (const final));
  MOCK_METHOD(DataVariant, read, (), (const final));

protected:
  DataType type_ = DataType::Boolean;
  std::optional<DataVariant> value_;

private:
  ReadCallback read_;
};

using ReadableMockPtr = std::shared_ptr<ReadableMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_READABLE_MOCK_HPP