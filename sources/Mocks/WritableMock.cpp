#include "WritableMock.hpp"

namespace Information_Model::testing {
using namespace ::testing;

WritableMock::WritableMock(DataType type) : ReadableMock(type) {
  setWriteOnly(false);
}

WritableMock::WritableMock(DataType type, const ReadCallback& read_cb)
    : ReadableMock(type, read_cb) {
  setWriteOnly(false);
}

WritableMock::WritableMock(const DataVariant& value) : ReadableMock(value) {
  setWriteOnly(false);
}

WritableMock::WritableMock(DataType type, const WriteCallback& write_cb) {
  updateType(type);
  updateCallback(write_cb);
  setWriteOnly(true);
}

WritableMock::WritableMock(
    const DataVariant& value, const WriteCallback& write_cb, bool write_only) {
  updateValue(value);
  updateCallback(write_cb);
  setWriteOnly(write_only);
}

WritableMock::WritableMock(
    DataType type, const ReadCallback& read_cb, const WriteCallback& write_cb)
    : ReadableMock(type, read_cb) {
  updateCallback(write_cb);
  setWriteOnly(false);
}

WritableMock::WritableMock(const DataVariant& value,
    const ReadCallback& read_cb,
    const WriteCallback& write_cb)
    : ReadableMock(toDataType(value), read_cb) {
  updateValue(value);
  updateCallback(write_cb);
  setWriteOnly(false);
}

void WritableMock::setWriteOnly(bool write_only) {
  if (write_only) {
    ON_CALL(*this, read).WillByDefault(Throw(NonReadable()));
  }
  ON_CALL(*this, isWriteOnly).WillByDefault(Return(write_only));
}

void WritableMock::updateCallback(const WriteCallback& write_cb) {
  // NOLINTNEXTLINE(bugprone-assignment-in-if-condition)
  if ((write_ = write_cb)) { // we want to assign and check if it was unset
    ON_CALL(*this, write).WillByDefault(write_);
  } else {
    ON_CALL(*this, write).WillByDefault(Throw(WriteCallbackUnavailable()));
  }
}

void WritableMock::updateCallbacks(
    const ReadCallback& read_cb, const WriteCallback& write_cb) {
  if (read_cb) {
    ReadableMock::updateCallback(read_cb);
  } else {
    setWriteOnly(true);
  }

  updateCallback(write_cb);
}
} // namespace Information_Model::testing
