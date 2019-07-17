#include "DeviceElementGroup.hpp"
#include "DeviceElement.hpp"
#include "DeviceElementBuilder.hpp"
#include "Utils.hpp"
#include <memory>

using namespace std;
using namespace Model_Factory;

DeviceElementGroup::DeviceElementGroup(const string REF_ID, const string NAME,
                                       const string DESC)
    : iDeviceElementGroup(REF_ID, NAME, DESC) {}

string
DeviceElementGroup::addDeviceEelment(const std::string NAME,
                                     const std::string DESC,
                                     Information_Model::ElementType type) {
  const string REF_ID = generate_Reference_ID(
      dynamic_cast<Information_Model::DeviceElement *>(this));

  subelements.push_back(unique_ptr<Information_Model::DeviceElement>(
      new DeviceElementBuilder(REF_ID, NAME, DESC, type)));

  return REF_ID;
}

Information_Model::DeviceElement *
DeviceElementGroup::getSubelement(const std::string REF_ID) {
  for (auto &subelement : subelements) {
    if (subelement->getElementRefId() == REF_ID) {
      return subelement.get();
    }
  }
  return nullptr;
}

vector<std::unique_ptr<Information_Model::DeviceElement>> const &
DeviceElementGroup::getSubelements() {
  return subelements;
}