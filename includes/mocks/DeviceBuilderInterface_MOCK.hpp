#ifndef __INFORMATION_MODEL_DEVICE_BUILDER_MOCK_HPP
#define __INFORMATION_MODEL_DEVICE_BUILDER_MOCK_HPP

#include "../DeviceBuilderInterface.hpp"

#include <gmock/gmock.h>

namespace Information_Model {
namespace testing {
/**
 * @brief DeviceBuilderInterfaceMock provides a mock class for Technology
 * Adapter Implementations to test in unit and/or integration test stages
 *
 */
class DeviceBuilderInterfaceMock : public DeviceBuilderInterface {
public:
  MOCK_METHOD(void, buildDeviceBase,
      (const std::string& /* unique_id */, const std::string& /* name */,
          const std::string& /* desc */),
      (override));
  MOCK_METHOD(std::string, addDeviceElementGroup,
      (const std::string& /* name */, const std::string& /* desc */),
      (override));
  MOCK_METHOD(std::string, addDeviceElementGroup,
      (const std::string& /* group_refid */, const std::string& /* name */,
          const std::string& /* desc */),
      (override));
  MOCK_METHOD(std::string, addReadableMetric,
      (const std::string& /* name */, const std::string& /* desc */,
          DataType /* data_type */, ReadFunctor /* read_cb */),
      (override));
  MOCK_METHOD(std::string, addReadableMetric,
      (const std::string& /* group_refid */, const std::string& /* name */,
          const std::string& /* desc */, DataType /* data_type */,
          ReadFunctor /* read_cb */),
      (override));
  MOCK_METHOD(std::string, addWritableMetric,
      (const std::string& /* name */, const std::string& /* desc */,
          DataType /* data_type */, std::optional<ReadFunctor> /* read_cb */,
          WriteFunctor /* write_cb */),
      (override));
  MOCK_METHOD(std::string, addWritableMetric,
      (const std::string& /* group_refid */, const std::string& /* name */,
          const std::string& /* desc */, DataType /* data_type */,
          std::optional<ReadFunctor> /* read_cb */,
          WriteFunctor /* write_cb */),
      (override));
  MOCK_METHOD(std::string, addDeviceElement,
      (const std::string& /* group_refid */, const std::string& /* name */,
          const std::string& /* desc */, ElementType /* type */,
          DataType /* data_type */, std::optional<ReadFunctor> /* read_cb */,
          std::optional<WriteFunctor> /* write_cb */),
      (override));
  MOCK_METHOD(std::shared_ptr<Device>, getResult, (), (override));
};
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_BUILDER_MOCK_HPP