#ifndef __INFORMATION_MODEL_NAMED_ELEMENT_HPP_
#define __INFORMATION_MODEL_NAMED_ELEMENT_HPP_

#include <memory>
#include <string>

#include "Nonempty/Pointer.hpp"

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
  std::string refID_;
  std::string name_;
  std::string desc_;

  NamedElement() = delete;

protected:
  NamedElement(const std::string& ref_id,
      const std::string& name,
      const std::string& desc)
      : refID_(ref_id), name_(name), desc_(desc) {}

public:
  virtual ~NamedElement() = default;

  const std::string getElementId() const { return refID_; }
  const std::string getElementName() const { return name_; }
  const std::string getElementDescription() const { return desc_; }

  bool operator==(const NamedElement& other) const noexcept {
    return (refID_ == other.refID_) && //
        (name_ == other.name_) && //
        (desc_ == other.desc_); //
  }

  bool operator!=(const NamedElement& other) const noexcept {
    return !operator==(other);
  }
};

using NamedElementPtr = std::shared_ptr<NamedElement>;
using NonemptyNamedElementPtr = Nonempty::Pointer<NamedElementPtr>;
/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_NAMED_ELEMENT_HPP_