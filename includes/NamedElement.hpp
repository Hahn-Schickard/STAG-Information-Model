#ifndef __INFORMATION_MODEL_NAMED_ELEMENT_HPP_
#define __INFORMATION_MODEL_NAMED_ELEMENT_HPP_

#include <memory>
#include <string>

#include "Nonempty_Pointer/NonemptyPtr.hpp"

namespace Information_Model {

/**
 * @addtogroup NamedElementModeling Named Element Modelling
 * @{
 */
/**
 * @brief Base modeling entity for Information Model
 *
 * Contains meta information of the modeled entity.
 *
 * MUST ONLY be inherited by Device or DeviceElement classes
 */
class NamedElement {
  std::string name_;
  std::string desc_;
  std::string refID_;

protected:
  NamedElement(const std::string& ref_id,
      const std::string& name,
      const std::string& desc)
      : name_(name), desc_(desc), refID_(ref_id) {}

public:
  const std::string getElementName() const { return name_; }
  const std::string getElementDescription() const { return desc_; }
  const std::string getElementId() const { return refID_; }

  virtual ~NamedElement() = default;
};

using NamedElementPtr = std::shared_ptr<NamedElement>;
using NonemptyNamedElementPtr = NonemptyPointer::NonemptyPtr<NamedElementPtr>;

/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_NAMED_ELEMENT_HPP_