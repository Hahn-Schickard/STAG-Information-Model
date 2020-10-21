#ifndef __INFORMATION_MODEL_DEVICE_BUILDER_INTERFACE_HPP
#define __INFORMATION_MODEL_DEVICE_BUILDER_INTERFACE_HPP

#include "DataVariant.hpp"
#include "Device.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace Information_Model {
using ReadFunctor = std::function<DataVariant()>;
using WriteFunctor = std::function<void(DataVariant)>;

/**
 * @brief This Interface is used by Technology Adapter implementations to build
 * a device witihin the Information Model
 *
 */
class DeviceBuilderInterface {
public:
  virtual ~DeviceBuilderInterface() = default;

  /**
   * @brief Create a base element for the device
   *
   * @param unique_id
   * @param name
   * @param desc
   */
  virtual void buildDeviceBase(const std::string &unique_id,
                               const std::string &name,
                               const std::string &desc) = 0;

  /**
   * @brief Adds a group element to the device
   *
   * @param name
   * @param desc
   * @return std::string
   */
  virtual std::string addDeviceElementGroup(const std::string &name,
                                            const std::string &desc) = 0;

  /**
   * @brief Adds a group elemenet to another group
   *
   * @param group_refid
   * @param name
   * @param desc
   * @return std::string
   */
  virtual std::string addDeviceElementGroup(const std::string &group_refid,
                                            const std::string &name,
                                            const std::string &desc) = 0;

  /**
   * @brief Adds a readable metric with a given data type to the root group
   *
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @return std::string
   */
  virtual std::string addReadableMetric(const std::string &name,
                                        const std::string &desc,
                                        DataType data_type,
                                        ReadFunctor read_cb) = 0;

  /**
   * @brief Adds a readable metric with a given data type to a given subroup
   *
   * @param group_refid
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @return std::string
   */
  virtual std::string addReadableMetric(const std::string &group_refid,
                                        const std::string &name,
                                        const std::string &desc,
                                        DataType data_type,
                                        ReadFunctor read_cb) = 0;

  /**
   * @brief Adds a writable metric with a given data type to the root group
   *
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @param write_cb
   * @return std::string
   */
  virtual std::string addWritableMetric(const std::string &name,
                                        const std::string &desc,
                                        DataType data_type, ReadFunctor read_cb,
                                        WriteFunctor write_cb) = 0;

  /**
   * @brief Adds a writable metrich with a given data type to a given subgroup
   *
   * @param group_refid
   * @param name
   * @param desc
   * @param type
   * @param data_type
   * @param read_cb
   * @param write_cb
   * @return std::string
   */
  virtual std::string addWritableMetric(const std::string &group_refid,
                                        const std::string &name,
                                        const std::string &desc,
                                        DataType data_type, ReadFunctor read_cb,
                                        WriteFunctor write_cb) = 0;

  /**
   * @brief Provides a generalized approach to creating a device element
   *
   * To add a Group call with 3 parameters (group_refid cann be an empty
   * string): addDeviceElement(group_refid, name, desc)
   *
   * To add a Readable metric call with 4 parameters (group_refid cann be an
   * empty string): addDeviceElement(group_refid, name, desc, read_callback)
   *
   * To add a Writable Metric call with 5 parameters (group_refid cann be an
   * empty string): addDeviceElement(group_refid, name, desc, read_callback,
   * write_callback)
   *
   * @param group_refid
   * @param name
   * @param desc
   * @param type
   * @param data_type
   * @param read_cb
   * @param write_cb
   * @return std::string
   */
  virtual std::string
  addDeviceElement(const std::string &group_refid, const std::string &name,
                   const std::string &desc, ElementType type,
                   DataType data_type, std::optional<ReadFunctor> read_cb,
                   std::optional<WriteFunctor> write_cb) = 0;

  virtual DevicePtr getResult() = 0;
};
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_BUILDER_INTERFACE_HPP