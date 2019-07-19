#include "NamedElement.hpp"

using namespace std;
using namespace Information_Model;

NamedElement::NamedElement(string refId, string name, string desc) {
  this->name = name;
  this->desc = desc;
  this->refId = refId;
}

const string NamedElement::getElementDescription() { return desc; }

const string NamedElement::getElementName() { return name; }

const string NamedElement::getElementRefId() { return refId; }