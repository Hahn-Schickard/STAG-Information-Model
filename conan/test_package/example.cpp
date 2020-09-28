#include "NamedElement.hpp"

#include <iostream>

using namespace std;

struct ElementImplementation : public Information_Model::NamedElement {
  ElementImplementation(const std::string REF_ID, const std::string NAME,
                        const std::string DESC)
      : NamedElement(REF_ID, NAME, DESC) {}
};

int main() {
  auto element = ElementImplementation("1", "element", "Dummy element");
  cout << "Element id: " << element.getElementRefId()
       << " with name: " << element.getElementName()
       << " described as: " << element.getElementDescription() << endl;
  exit(EXIT_SUCCESS);
}
