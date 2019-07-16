#ifndef _NAMEDELEMENT_HPP
#define _NAMEDELEMENT_HPP

#include "iNamedElement.hpp"

namespace Model_Factory {
using namespace Information_Model;

class NamedElement : public iNamedElement {
private:
  std::string name;
  std::string desc;
  std::string refId;

public:
  NamedElement(const std::string refId, const std::string name,
               const std::string desc);

  const std::string getElementName();
  const std::string getElementDescription();
  const std::string getElementRefId();
};
}

#endif //_NAMEDELEMENT_HPP