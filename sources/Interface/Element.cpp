#include "Element.hpp"

#include <stdexcept>

namespace Information_Model {
using namespace std;

string toString(ElementType type) {
  switch (type) {
  case ElementType::Group: {
    return "Group";
  }
  case ElementType::Readable: {
    return "Readable";
  }
  case ElementType::Writable: {
    return "Writable";
  }
  case ElementType::Observable: {
    return "Observable";
  }
  case ElementType::Callable: {
    return "Callable";
  }
  default: {
    throw logic_error("Could not decode ElementType enum value");
  }
  }
}

bool operator==(const ElementPtr& lhs, const ElementPtr& rhs) {
  return lhs->id() == rhs->id() && lhs->name() == rhs->name() &&
      lhs->description() == rhs->description() && lhs->type() == rhs->type();
}

bool operator!=(const ElementPtr& lhs, const ElementPtr& rhs) {
  return !(lhs == rhs);
}
/** @}*/
} // namespace Information_Model
