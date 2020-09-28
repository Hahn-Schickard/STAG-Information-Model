#ifndef __DATA_MODEL_NAMED_ELEMENT_HPP_
#define __DATA_MODEL_NAMED_ELEMENT_HPP_

#include <string>

namespace Information_Model {
class NamedElement {
protected:
  NamedElement(const std::string REF_ID, const std::string NAME,
               const std::string DESC)
      : name_(NAME), desc_(DESC), refID_(REF_ID) {}

public:
  const std::string getElementName() const { return name_; }
  const std::string getElementDescription() const { return desc_; }
  const std::string getElementId() const { return refID_; }

  virtual ~NamedElement() = default;

private:
  std::string name_;
  std::string desc_;
  std::string refID_;
};

} // namespace Information_Model

#endif //__DATA_MODEL_NAMED_ELEMENT_HPP_