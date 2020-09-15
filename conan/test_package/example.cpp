#include "NamedElement.hpp"

#include <iostream>

using namespace std;

int main() {
  auto element =
      Information_Model::NamedElement("1", "element", "Dummy element");
  cout << "Element id: " << element.getElementRefId()
       << " with name: " << element.getElementName()
       << " described as: " << element.getElementDescription() << endl;
  exit(EXIT_SUCCESS);
}
