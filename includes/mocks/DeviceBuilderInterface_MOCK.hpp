#ifndef __INFORMATION_MODEL_DEVICE_BUILDER_MOCK_HPP
#define __INFORMATION_MODEL_DEVICE_BUILDER_MOCK_HPP

#include "../DeviceBuilderInterface.hpp"

#include <gmock/gmock.h>

namespace Information_Model {
namespace testing {
/**
 * @brief DeviceBuilderInterface mock class for Technology
 * Adapter Implementations to test in unit and/or integration test stages
 *
 * @attention
 * Use for testing only
 */
struct DeviceBuilderMock : public DeviceBuilderInterface {
  /**
   * @addtogroup DeviceModeling Device Modelling
   * @{
   */
  MOCK_METHOD(void,
      buildDeviceBase,
      (const std::string& /* unique_id */,
          const std::string& /* name */,
          const std::string& /* desc */),
      (override));
  /** @}*/

  /**
   * @addtogroup GroupModeling Device Element Group Modelling
   * @{
   */
  MOCK_METHOD(std::string,
      addDeviceElementGroup,
      (const std::string& /* group_refid */,
          const std::string& /* name */,
          const std::string& /* desc */),
      (override));
  /** @}*/

  /**
   * @addtogroup ReadableModeling Metric Modelling
   * @{
   */
  MOCK_METHOD(std::string,
      addReadableMetric,
      (const std::string& /* group_refid */,
          const std::string& /* name */,
          const std::string& /* desc */,
          DataType /* data_type */,
          const DeviceBuilderInterface::Reader& /* read_cb */),
      (override));
  /** @}*/

  /**
   * @addtogroup WritableModeling Writable Metric Modelling
   * @{
   */
  MOCK_METHOD(std::string,
      addWritableMetric,
      (const std::string& /* group_refid */,
          const std::string& /* name */,
          const std::string& /* desc */,
          DataType /* data_type */,
          const DeviceBuilderInterface::Writer& /* write_cb */,
          const DeviceBuilderInterface::Reader& /* read_cb */),
      (override));
  /** @}*/

  /**
   * @addtogroup ExecutableModeling Function Modelling
   * @{
   */
  MOCK_METHOD(std::string,
      addFunction,
      (const std::string& /* group_refid */,
          const std::string& /* name */,
          const std::string& /* desc */,
          DataType /* data_type */,
          const DeviceBuilderInterface::Executor& /*execute_cb*/,
          const DeviceBuilderInterface::Canceler& /*cancel_cb*/,
          const Function::ParameterTypes& /*supported_params*/),
      (override));
  /** @}*/

  /**
   * @addtogroup ExecutableModeling Function Modelling
   * @{
   */
  MOCK_METHOD(std::pair<std::string, ObservedValue>,
      addObservableMetric,
      (const std::string& /*group_ref_id*/,
          const std::string& /*name*/,
          const std::string& /*desc*/,
          DataType /*data_type*/,
          const Reader& /*read_cb*/,
          const ObserveInitializer& /*observe_cb*/),
      (override));
  /** @}*/

  /**
   * @addtogroup DeviceModeling Device Modelling
   * @{
   */
  MOCK_METHOD(UniqueDevicePtr, getResult, (), (override));
  /** @}*/
};
using DeviceBuilderMockPtr = std::shared_ptr<DeviceBuilderMock>;
using NonemptyDeviceBuilderMockPtr = Nonempty::Pointer<DeviceBuilderMockPtr>;
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_BUILDER_MOCK_HPP