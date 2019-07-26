#include "Utils.hpp"
#include "DeviceElement.hpp"
#include "DeviceElementGroup.hpp"
#include <string>

using namespace std;
using namespace Information_Model;

string generate_Reference_ID(DeviceElement *parent) {
  const string BASE_ID = parent->getElementRefId();
  string element_id = "";

  switch (parent->getElementType()) {
  case ElementType::Group: {
    auto parent_element = static_cast<DeviceElementGroup *>(parent);
    auto elId =  parent_element->getNumericElementId();
    
    element_id = to_string(elId);

    break;
  }
  case ElementType::Readonly:
  case ElementType::Observable:
  case ElementType::Writable:
  case ElementType::Function: {
    int element_number = stoi(parent->getElementRefId()) + 1;
    element_id = "." + to_string(element_number);
    break;
  }
  case ElementType::Undefined:
  default: {
    //@TODO: Handle defualt behaviour for Undefined elements
  }
  }
  return BASE_ID + element_id;
}