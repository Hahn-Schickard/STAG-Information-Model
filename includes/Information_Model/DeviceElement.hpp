#ifndef _I_DEVICE_ELEMENT_HPP
#define _I_DEVICE_ELEMENT_HPP

#include "NamedElement.hpp"

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
  /**
 * @brief This class is the base for all of the elements within the
 * Information_Model::Device.
 * @attention This class should be instantiated by its children only!
 * @author Dovydas Girdvainis
 * @date 18.07.2019
 */
  class DeviceElement : public NamedElement {
   protected:
    ElementType type_ = ElementType::UNDEFINED;

    DeviceElement(const std::string& REF_ID,
        const std::string& NAME,
        const std::string& DESC,
        ElementType type)
        : NamedElement(REF_ID, NAME, DESC)
        , type_(type) {}

    /**
     * @brief Set the Element Type object
     * Only called from by children.
     * 
     * @param type 
     */
    void setElementType(ElementType type) {
      type_ = type;
    }

   public:
    ElementType getElementType() {
      return type_;
    }
    virtual ~DeviceElement() = default;
  };
}   // namespace Information_Model

#endif   //_I_DEVICE_ELEMENT_HPP