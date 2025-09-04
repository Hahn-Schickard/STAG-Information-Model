#ifndef __STAG_INFORMATION_MODEL_WRITABLE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_WRITABLE_MOCK_HPP
#include "Writable.hpp"

#include "ReadableMock.hpp"

namespace Information_Model::testing {

struct WritableMock : virtual public Writable, public ReadableMock {
  using WriteCallback = std::function<void(const DataVariant&)>;

  WritableMock() = default;

  explicit WritableMock(DataType type);

  WritableMock(DataType type, const ReadCallback& read_cb);

  explicit WritableMock(const DataVariant& value);

  WritableMock(DataType type, const WriteCallback& write_cb);

  WritableMock(
      const DataVariant& value, const WriteCallback& write_cb, bool write_only);

  WritableMock(DataType type,
      const ReadCallback& read_cb,
      const WriteCallback& write_cb);

  WritableMock(const DataVariant& value,
      const ReadCallback& read_cb,
      const WriteCallback& write_cb);

  ~WritableMock() override = default;

  void setWriteOnly(bool write_only);

  void updateReadCallback(const ReadCallback& read_cb);

  void updateWriteCallback(const WriteCallback& write_cb);

  void updateCallbacks(
      const ReadCallback& read_cb, const WriteCallback& write_cb);

  MOCK_METHOD(bool, isWriteOnly, (), (const final));
  MOCK_METHOD(void, write, (const DataVariant&), (const final));

private:
  WriteCallback write_;
};

using WritableMockPtr = std::shared_ptr<WritableMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_WRITABLE_MOCK_HPP