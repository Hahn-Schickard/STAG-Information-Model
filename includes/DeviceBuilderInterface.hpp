#ifndef __INFORMATION_MODEL_DEVICE_BUILDER_INTERFACE_HPP
#define __INFORMATION_MODEL_DEVICE_BUILDER_INTERFACE_HPP

#include "DataVariant.hpp"
#include "Device.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

namespace Information_Model {
using ReadFunctor = std::function<DataVariant()>;
using WriteFunctor = std::function<void(DataVariant)>;

/**
 * @enum ElementTypeEnum
 * @brief ElementType enumeration, specifying the available DeviceElement types
 *
 */
enum class ElementType {
  GROUP, /*!< Grouping element, aka list */
  READABLE, /*!< Metric with read access */
  WRITABLE /*!< Metric with write access */
};

/**
 * @brief This Interface is used by Technology Adapter implementations to build
 * a device within the Information Model
 *
 */
class DeviceBuilderInterface {
public:
  virtual ~DeviceBuilderInterface() = default;

  /**
   * @brief Create a base element for the device.
   *
   * This method MUST be called first when creating a new device, because this
   * method initializes the base implementation as well as assigns the device
   * ID, which will be used as a seed for DeviceElement ID generation
   *
   * @param unique_id
   * @param name
   * @param desc
   */
  virtual void buildDeviceBase(const std::string& /*unique_id*/,
      const std::string& /*name*/, const std::string& /*desc*/) {
    throw std::runtime_error("Called base implementation of "
                             "DeviceBuilderInterface::buildDeviceBase");
  }

  /**
   * @brief Adds a group element to the device root level DeviceElementGroup.
   *
   * This method creates a new DeviceElementGroup instance, adds it to the root
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the said newly created DeviceElementGroup. Returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string the
   * be as follows: DEVICE_UNIQUE_ID:ELEMENT_ID
   *
   * @param name
   * @param desc
   * @return std::string
   */
  virtual std::string addDeviceElementGroup(
      const std::string& /*name*/, const std::string& /*desc*/) {
    throw std::runtime_error("Called base implementation of "
                             "DeviceBuilderInterface::addDeviceElementGroup");
  }

  /**
   * @brief Adds a group element to another DeviceElementGroup. Parent group
   * element MUST exist.
   *
   * This method creates a new DeviceElementGroup instance, adds it to the
   * specified DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the said newly created DeviceElementGroup.
   *
   * The requested group_refid argument is obtained from the previous
   * addDeviceElementGroup(name,desc) or addDeviceElement(...) call
   *
   * Returned string will be based on the unique id, provided by the user with
   * the previously called buildDeviceBase() method. The format of this string
   * the be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If specified parent DeviceElementGroup is a subgroup itself, the returned
   * string will represent it as a new ID element as such:
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:SUBPARENT_ELEMENT_ID:ELEMENT_ID. Similar
   * format will be used nth level of subgroup as well.
   *
   * @param group_refid
   * @param name
   * @param desc
   * @return std::string
   */
  virtual std::string addDeviceElementGroup(const std::string& /*group_refid*/,
      const std::string& /*name*/, const std::string& /*desc*/) {
    throw std::runtime_error("Called base implementation of "
                             "DeviceBuilderInterface::addDeviceElementGroup");
  }
  /**
   * @brief Adds a readable metric with a given data type to the device root
   * level DeviceElementGroup.
   *
   * This method creates a new Metric instance, adds it to the root
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the said newly created Metric. Returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string the
   * be as follows: DEVICE_UNIQUE_ID:ELEMENT_ID
   *
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @return std::string
   */
  virtual std::string addReadableMetric(const std::string& /*name*/,
      const std::string& /*desc*/, DataType /*data_type*/,
      ReadFunctor /*read_cb*/) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addReadableMetric for root group");
  }

  /**
   * @brief Adds a readable metric with a given data type to another
   * DeviceElementGroup. Parent group element MUST exist.
   *
   * This method creates a new Metric instance, adds it to the specified
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the said newly created Metric.
   *
   * The requested group_refid argument is obtained from the previous
   * addDeviceElementGroup(name,desc) or addDeviceElement(...) call
   *
   * Returned string will be based on the unique id, provided by the user with
   * the previously called buildDeviceBase() method. The format of this string
   * the be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If specified parent DeviceElementGroup is a subgroup itself, the returned
   * string will represent it as a new ID element as such:
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:SUBPARENT_ELEMENT_ID:ELEMENT_ID. Similar
   * format will be used nth level of subgroup as well.
   *
   * @param group_refid
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @return std::string
   */
  virtual std::string addReadableMetric(const std::string& /*group_refid*/,
      const std::string& /*name*/, const std::string& /*desc*/,
      DataType /*data_type*/, ReadFunctor /*read_cb*/) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addReadableMetric for subgroup");
  }

  /**
   * @brief Adds a writable metric with a given data type to the device root
   * level DeviceElementGroup.
   *
   * This method creates a new WritableMetric instance, adds it to the root
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the said newly created WritableMetric. Returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string the
   * be as follows: DEVICE_UNIQUE_ID:ELEMENT_ID
   *
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @param write_cb
   * @return std::string
   */
  virtual std::string addWritableMetric(const std::string& /*name*/,
      const std::string& /*desc*/, DataType /*data_type*/,
      std::optional<ReadFunctor> /*read_cb*/, WriteFunctor /*write_cb*/) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addWritableMetric for root group");
  }

  /**
   * @brief Adds a writable metric with a given data type to another
   * DeviceElementGroup. Parent group element MUST exist.
   *
   * This method creates a new WritableMetric instance, adds it to the specified
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the said newly created WritableMetric.
   *
   * The requested group_refid argument is obtained from the previous
   * addDeviceElementGroup(name,desc) or addDeviceElement(...) call
   *
   * Returned string will be based on the unique id, provided by the user with
   * the previously called buildDeviceBase() method. The format of this string
   * the be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If specified parent DeviceElementGroup is a subgroup itself, the returned
   * string will represent it as a new ID element as such:
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:SUBPARENT_ELEMENT_ID:ELEMENT_ID. Similar
   * format will be used nth level of subgroup as well.
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
  virtual std::string addWritableMetric(const std::string& /*group_refid*/,
      const std::string& /*name*/, const std::string& /*desc*/,
      DataType /*data_type*/, std::optional<ReadFunctor> /*read_cb*/,
      WriteFunctor /*write_cb*/) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addWritableMetric for subgroup");
  }

  /**
   * @brief Provides a generalized approach to creating any device element
   *
   * To add a new root DeviceElementGroup call this method as follows:
   * @code
   * addDeviceElement("",name,desc)
   * @endcode
   *
   * To add a new DeviceElementGroup to an existing DeviceElementGroup call this
   * method as follows:
   * @code
   * addDeviceElement(parent_id,name,desc)
   * @endcode
   * The requested parent_id argument is obtained from  previous
   * addDeviceElementGroup(name,desc) or addDeviceElement(...) call
   *
   * To add a new root Readable metric call this method as follows:
   * @code
   * addDeviceElement("",name,desc,ElementType::READABLE,data_type,read_cb)
   * @endcode
   * Where data_type specifies the desired DataType that this Metric
   * will model and read_cb specifies the functor callback used when trying to
   * access the real value
   *
   * To add a new Readable metric to an existing DeviceElementGroup call this
   * method as follows:
   * @code
   * addDeviceElement(parent_id,name,ElementType::READABLE,data_type,read_cb)
   * @endcode
   * The requested parent_id argument is obtained from  previous
   * addDeviceElementGroup(name,desc) or addDeviceElement(...) call, data_type
   * specifies the desired DataType that this Metric will model and read_cb
   * specifies the functor callback used when trying to access the real value
   *
   * To add a new root Writable Metric call this method as follows:
   * @code
   * addDeviceElement("",name,desc,ElementType::WRITABLE,data_type,read_cb,
   * write_cb)
   * @endcode
   * Where data_type specifies the desired DataType that this Metric
   * will model, read_cb specifies the functor callback used when trying to
   * access the real value and write_cb specifies the functor callback used when
   * trying to set the real value. All WritableMetric instances are also
   * Readable instances! If you want to create a Write-Only Metric, set the
   * read_cb to a real function with an empty body. DO NOT provide a
   * std::nullopt as a read_cb since, this will throw an exception when calling
   * the read operation.
   *
   * To add a new Writable Metric to an existing DeviceElementGroup call this
   * method as follows:
   * @code
   * addDeviceElement(parent_id,name,desc,ElementType::WRITABLE,data_type,read_cb,
   *                  write_cb)
   * @endcode
   * The requested parent_id argument is obtained from  previous
   * addDeviceElementGroup(name,desc) or addDeviceElement(...) call, data_type
   * specifies the desired DataType that this Metric will model, read_cb
   * specifies the functor callback used when trying to access the real value
   * and write_cb specifies the functor callback used when trying to set the
   * real value. All WritableMetric instances are also Readable instances! If
   * you want to create a Write-Only Metric, set the read_cb to a real function
   * with an empty body. DO NOT provide a std::nullopt as a read_cb since, this
   * will throw an exception when calling the read operation.
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
  virtual std::string addDeviceElement(const std::string& /*group_refid*/,
      const std::string& /*name*/, const std::string& /*desc*/,
      ElementType type = ElementType::GROUP,
      DataType data_type = DataType::UNKNOWN,
      std::optional<ReadFunctor> read_cb = std::nullopt,
      std::optional<WriteFunctor> write_cb = std::nullopt) {
    throw std::runtime_error("Called base implementation of "
                             "DeviceBuilderInterface::addDeviceElement");
  }

  /**
   * @brief Checks if the built result is valid and returns it.
   *
   * If the device creation was not properly finished, throws an exception
   *
   * @return DevicePtr
   */
  virtual DevicePtr getResult() {
    throw std::runtime_error("Called base implementation of "
                             "DeviceBuilderInterface::getResult");
  }
};
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_BUILDER_INTERFACE_HPP