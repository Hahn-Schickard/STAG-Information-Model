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
      (const std::string& /* name */, const std::string& /* desc */),
      (override));

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
      (const std::string& /* name */,
          const std::string& /* desc */,
          DataType /* data_type */,
          ReadFunctor /* read_cb */),
      (override));

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
      (const std::string& /* name */,
          const std::string& /* desc */,
          DataType /* data_type */,
          std::optional<ReadFunctor> /* read_cb */,
          WriteFunctor /* write_cb */),
      (override));
  MOCK_METHOD(std::string,
      addWritableMetric,
      (const std::string& /* group_refid */,
          const std::string& /* name */,
          const std::string& /* desc */,
          DataType /* data_type */,
          std::optional<ReadFunctor> /* read_cb */,
          WriteFunctor /* write_cb */),
      (override));
  /** @}*/

  /**
   * @addtogroup ExecutableModeling Function Modelling
   * @{
   */
  MOCK_METHOD(std::string,
      addFunction,
      (const std::string& /* name */,
          const std::string& /* desc */,
          DataType /* data_type */,
          std::optional<Function::ParameterTypes> /*supported_params*/,
          ExecuteFunctor /*execute_cb*/,
          CancelFunctor /*cancel_cb*/),
      (override));

  MOCK_METHOD(std::string,
      addFunction,
      (const std::string& /* group_refid */,
          const std::string& /* name */,
          const std::string& /* desc */,
          DataType /* data_type */,
          std::optional<Function::ParameterTypes> /*supported_params*/,
          ExecuteFunctor /*execute_cb*/,
          CancelFunctor /*cancel_cb*/),
      (override));
  /** @}*/

  /**
   * @addtogroup ElementModeling Device Element Modelling
   * @{
   */
  MOCK_METHOD(std::string,
      addDeviceElement,
      (const std::string& /* group_refid */,
          const std::string& /* name */,
          const std::string& /* desc */,
          ElementType /* type */,
          DataType /* data_type */,
          std::optional<ReadFunctor> /* read_cb */,
          std::optional<WriteFunctor> /* write_cb */,
          std::optional<ExecuteFunctor> /* execute_cb */),
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