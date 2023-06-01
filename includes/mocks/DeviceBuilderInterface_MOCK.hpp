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
struct DeviceBuilderInterfaceMock : public DeviceBuilderInterface {
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
          ReadFunctor /* read_cb */),
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
          WriteFunctor /* write_cb */,
          ReadFunctor /* read_cb */),
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
          ExecuteFunctor /*execute_cb*/,
          CancelFunctor /*cancel_cb*/,
          Function::ParameterTypes /*supported_params*/),
      (override));
  /** @}*/

  /**
   * @addtogroup DeviceModeling Device Modelling
   * @{
   */
  MOCK_METHOD(UniqueDevicePtr, getResult, (), (override));
  /** @}*/
};
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_BUILDER_MOCK_HPP