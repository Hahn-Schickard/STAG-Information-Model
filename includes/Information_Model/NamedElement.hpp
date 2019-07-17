#ifndef _NAMEDELEMENT_HPP
#define _NAMEDELEMENT_HPP

#include <string>

namespace Information_Model {

class NamedElement {
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