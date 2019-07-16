#include "DeviceElement.hpp"
#include "NamedElement.hpp"

using namespace std;
using namespace Model_Factory;

DeviceElement::DeviceElement(string refId, string name, string desc,
                             ElementType type)
    : NamedElement(refId, name, desc), elementType(type) {}

ElementType DeviceElement::getElementType() { return elementType; }