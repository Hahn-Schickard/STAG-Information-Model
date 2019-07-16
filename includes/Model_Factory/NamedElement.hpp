#ifndef _NAMEDELEMENT_HPP
#define _NAMEDELEMENT_HPP

#include "iNamedElement.hpp"

namespace Model_Factory {
using namespace Information_Model;

class NamedElement : public iNamedElement {
public:
  NamedElement(const std::string refId, const std::string name,
               const std::string desc);

  const std::string getElementName();
  const std::string getElementDescription();
  const std::string getElementRefId();

private:
  std::string name;
  std::string desc;
  std::string refId;
};
}

#endif //_NAMEDELEMENT_HPP