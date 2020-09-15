#ifndef __DATA_MODEL_DEVICE_ELEMENT_HPP
#define __DATA_MODEL_DEVICE_ELEMENT_HPP

#include "NamedElement.hpp"

#include <memory>
#include <string>

namespace Information_Model {
/**
 * @enum ElementTypeEnum
 * @brief ElementType enumeration, specifying the abailable DeviceElement types
 *
 */
typedef enum ElementTypeEnum {
  UNDEFINED,  /*!< Fallback type */
  GROUP,      /*!< Groupping element, aka list */
  READABLE,   /*!< Metric with read access */
  OBSERVABLE, /*!< Metric with observation access */
  WRITABLE,   /*!< Metric with write access */
  FUNCTION    /*!< Function elemenets */
} ElementType;

class DeviceElement : public NamedElement {
public:
  DeviceElement(const std::string REF_ID, const std::string NAME,
                const std::string DESC, ElementType type)
      : NamedElement(REF_ID, NAME, DESC), type_(type) {}

  ElementType getElementType() { return type_; }
  virtual ~DeviceElement() = default;

private:
  ElementType type_ = ElementType::UNDEFINED;
};

} // namespace Information_Model

#endif //__DATA_MODEL_DEVICE_ELEMENT_HPP