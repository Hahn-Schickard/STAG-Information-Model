#ifndef __STAG_INFORMATION_MODEL_WRITABLE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_WRITABLE_MOCK_HPP
#include "Writable.hpp"

#include "ReadableMock.hpp"

namespace Information_Model::testing {

struct WritableMock : virtual public Writable, public ReadableMock {
  using WriteCallback = std::function<void(DataVariant)>;

  WritableMock() = default;

  explicit WritableMock(DataType type) : ReadableMock(type) {
    setWriteOnly(false);
  }

  WritableMock(DataType type, const ReadCallback& read_cb)
      : ReadableMock(type, read_cb) {
    setWriteOnly(false);
  }

  WritableMock(const DataVariant& value) : ReadableMock(value) {
    setWriteOnly(false);
  }

  WritableMock(DataType type, const WriteCallback& write_cb) {
    updateType(type);
    updateCallback(write_cb);
    setWriteOnly(true);
  }

  WritableMock(const DataVariant& value,
      const WriteCallback& write_cb,
      bool write_only) {
    updateValue(value);
    updateCallback(write_cb);
    setWriteOnly(write_only);
  }

  WritableMock(
      DataType type, const ReadCallback& read_cb, const WriteCallback& write_cb)
      : ReadableMock(type, read_cb) {
    updateCallback(write_cb);
    setWriteOnly(false);
  }

  WritableMock(const DataVariant& value,
      const ReadCallback& read_cb,
      const WriteCallback& write_cb)
      : ReadableMock(toDataType(value), read_cb) {
    updateValue(value);
    updateCallback(write_cb);
    setWriteOnly(false);
  }

  ~WritableMock() override = default;

  void setWriteOnly(bool write_only) {
    if (write_only) {
      ON_CALL(*this, read).WillByDefault(::testing::Throw(NonReadable()));
    }
    ON_CALL(*this, isWriteOnly).WillByDefault(::testing::Return(write_only));
  }

  void updateCallback(const WriteCallback& write_cb) {
    if ((write_ = write_cb)) {
      ON_CALL(*this, write).WillByDefault(write_);
    } else {
      ON_CALL(*this, write)
          .WillByDefault(::testing::Throw(WriteCallbackUnavailable()));
    }
  }

  void updateCallbacks(
      const ReadCallback& read_cb, const WriteCallback& write_cb) {
    if (read_cb) {
      ReadableMock::updateCallback(read_cb);
    } else {
      setWriteOnly(true);
    }

    updateCallback(write_cb);
  }

  MOCK_METHOD(bool, isWriteOnly, (), (const final));
  MOCK_METHOD(void, write, (const DataVariant&), (const final));

private:
  WriteCallback write_;
};

using WritableMockPtr = std::shared_ptr<WritableMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_WRITABLE_MOCK_HPP