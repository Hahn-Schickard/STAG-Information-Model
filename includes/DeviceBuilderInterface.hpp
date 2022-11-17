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
using ExecuteFunctor = std::function<bool(std::string)>;

/**
 * @enum ElementTypeEnum
 * @brief ElementType enumeration, specifying the available DeviceElement
 * types.
 *
 */
enum class ElementType {
  GROUP, /*!< Grouping element, aka list */
  READABLE, /*!< Metric with read access */
  WRITABLE, /*!< Metric with write access */
  OBSERVABLE, /*!< Metric with read access and ability to self report
                 changes */
  FUNCTION /*!< Metric with execute access */
};

/**
 * @brief This Interface is used by Technology Adapter implementations to build
 * a device within the Information Model.
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
   * DeviceElement ID of the newly created DeviceElementGroup. The returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string will
   * be as follows:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:ELEMENT_ID
   * @endcode
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
   * @brief Adds a group element to another DeviceElementGroup. The parent group
   * element MUST exist.
   *
   * This method creates a new DeviceElementGroup instance, adds it to the
   * specified DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created DeviceElementGroup.
   *
   * The requested group_refid argument is obtained from the previous
   * addDeviceElementGroup() or addDeviceElement() call.
   *
   * The returned string will be based on the unique id, provided by the user
   * with the previously called buildDeviceBase() method. The format of this
   * string will be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If the specified parent DeviceElementGroup is a subgroup itself, the
   * returned string will represent it as a new ID element as such:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:SUBPARENT_ELEMENT_ID:ELEMENT_ID.
   * @endcode
   * A similar format will be used in the nth level of subgroups as well.
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
   * DeviceElement ID of the newly created Metric. The returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string will
   * be as follows:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:ELEMENT_ID
   * @endcode
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
   * DeviceElementGroup. The parent group element MUST exist.
   *
   * This method creates a new Metric instance, adds it to the specified
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created Metric.
   *
   * The requested group_refid argument is obtained from the previous
   * addDeviceElementGroup() or addDeviceElement() call.
   *
   * The returned string will be based on the unique id, provided by the user
   * with the previously called buildDeviceBase() method. The format of this
   * string the be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If specified parent DeviceElementGroup is a subgroup itself, the returned
   * string will represent it as a new ID element as such:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:SUBPARENT_ELEMENT_ID:ELEMENT_ID.
   * @endcode
   * A similar format will be used nth level of subgroup as well.
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
   * DeviceElement ID of the newly created WritableMetric. The returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string will
   * be as follows:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:ELEMENT_ID
   * @endcode
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
   * DeviceElementGroup. The parent group element MUST exist.
   *
   * This method creates a new WritableMetric instance, adds it to the specified
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created WritableMetric.
   *
   * The requested group_refid argument is obtained from the previous
   * addDeviceElementGroup() or addDeviceElement() call.
   *
   * The returned string will be based on the unique id, provided by the user
   * with the previously called buildDeviceBase() method. The format of this
   * string the be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If specified parent DeviceElementGroup is a subgroup itself, the returned
   * string will represent it as a new ID element as such:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:SUBPARENT_ELEMENT_ID:ELEMENT_ID.
   * @endcode
   * A similar format will be used nth level of subgroup as well.
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
   * @todo Decide on how to best create a binding to an Observable
   *
   */

  virtual std::string addObservableMetric(const std::string& /*name*/,
      const std::string& /*desc*/ /* @todo: Observer binding mechanism */) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addObservableMetric for root group");
  }

  /**
   * @todo Decide on how to best create a binding to an Observable
   *
   */
  virtual std::string addObservableMetric(const std::string& /*group_refid*/,
      const std::string& /*name*/, const std::string& /*desc*/,
      DataType /*data_type*/ /* @todo: Observer binding mechanism */) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addObservableMetric for subgroup");
  }

  /**
   * @brief Adds a function to the device root level DeviceElementGroup.
   *
   * This method creates a new Function instance, adds it to the root
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created Function. The returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string will
   * be as follows:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:ELEMENT_ID
   * @endcode
   *
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @param write_cb
   * @return std::string
   */
  virtual std::string addFunction(const std::string& /*name*/,
      const std::string& /*desc*/, ExecuteFunctor /*execute_cb*/) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addFunction for root group");
  }

  /**
   * @brief Adds a function to another DeviceElementGroup. The parent group
   * element MUST exist.
   *
   * This method creates a new Function instance, adds it to the specified
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created Function.
   *
   * The requested group_refid argument is obtained from the previous
   * addDeviceElementGroup() or addDeviceElement() call.
   *
   * The returned string will be based on the unique id, provided by the user
   * with the previously called buildDeviceBase() method. The format of this
   * string the be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If specified parent DeviceElementGroup is a subgroup itself, the returned
   * string will represent it as a new ID element as such:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:SUBPARENT_ELEMENT_ID:ELEMENT_ID.
   * @endcode
   * A similar format will be used nth level of subgroup as well.
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
  virtual std::string addFunction(const std::string& /*group_refid*/,
      const std::string& /*name*/, const std::string& /*desc*/,
      ExecuteFunctor /*execute_cb*/) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addFunction for subgroup");
  }

  /**
   * @brief Provides a generalized approach to creating any device element.
   *
   * To add a new root DeviceElementGroup call this method as follows:
   * @code {cpp}
   * addDeviceElement("",name,desc)
   * @endcode
   *
   * To add a new DeviceElementGroup to an existing DeviceElementGroup call this
   * method as follows:
   * @code {cpp}
   * addDeviceElement(parent_id,name,desc)
   * @endcode
   * The requested parent_id argument is obtained from previous
   * addDeviceElementGroup() or addDeviceElement() call.
   *
   * To add a new root Readable metric call this method as follows:
   * @code {cpp}
   * addDeviceElement("",name,desc,ElementType::READABLE,data_type,read_cb)
   * @endcode
   * Where data_type specifies the desired DataType that this Metric
   * will model and read_cb specifies the functor callback used when trying to
   * access the value.
   *
   * To add a new Readable metric to an existing DeviceElementGroup call this
   * method as follows:
   * @code {cpp}
   * addDeviceElement(parent_id,name,ElementType::READABLE,data_type,read_cb)
   * @endcode
   * The requested parent_id argument is obtained from previous
   * addDeviceElementGroup() or addDeviceElement() call, data_type
   * specifies the desired DataType that this Metric will model and read_cb
   * specifies the functor callback used when trying to access the value.
   *
   * To add a new root Writable Metric call this method as follows:
   * @code {cpp}
   * addDeviceElement("",name,desc,ElementType::WRITABLE,data_type,read_cb,
   *     write_cb)
   * @endcode
   * Where data_type specifies the desired DataType that this Metric
   * will model, read_cb specifies the functor callback used when trying to
   * access the value and write_cb specifies the functor callback used when
   * trying to set the value. All WritableMetric instances are also
   * Readable instances! If you want to create a Write-Only Metric, set the
   * read_cb to a std::nullopt.
   *
   * To add a new Writable Metric to an existing DeviceElementGroup call this
   * method as follows:
   * @code {cpp}
   * addDeviceElement(parent_id,name,desc,ElementType::WRITABLE,data_type,read_cb,
   *     write_cb)
   * @endcode
   * The requested parent_id argument is obtained from  previous
   * addDeviceElementGroup() or addDeviceElement() call, data_type
   * specifies the desired DataType that this Metric will model, read_cb
   * specifies the functor callback used when trying to access the value
   * and write_cb specifies the functor callback used when trying to set the
   * value. All WritableMetric instances are also Readable instances! If
   * you want to create a Write-Only Metric, set the read_cb to a std::nullopt.
   *
   * To add a new root Function call this method as follows:
   * @code {cpp}
   * addDeviceElement("",name,desc,ElementType::FUNCTION, execute_cb)
   * @endcode
   * Where execute_cb specifies the execute function callback.
   *
   * To add a new Function to an existing DeviceElementGroup call this
   * method as follows:
   * @code {cpp}
   * addDeviceElement(parent_id,name,desc,ElementType::FUNCTION,execute_cb)
   * @endcode
   * The requested parent_id argument is obtained from  previous
   * addDeviceElementGroup() or addDeviceElement() call and execute_cb specifies
   * the execute function callback.
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
  virtual std::string addDeviceElement( // clang-format off
      const std::string& /*group_refid*/,
      const std::string& /*name*/, 
      const std::string& /*desc*/,
      ElementType /* type */,
      DataType /* data_type */,
      std::optional<ReadFunctor> /* read_cb */,
      std::optional<WriteFunctor> /* write_cb */,
      std::optional<ExecuteFunctor> /* execute_cb */) {
    throw std::runtime_error("Called base implementation of "
                             "DeviceBuilderInterface::addDeviceElement");
  }

  /**
   * @brief Checks if the built result is valid and returns it.
   *
   * If the device creation was not properly finished, throws an exception.
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
