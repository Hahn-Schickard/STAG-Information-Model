#include "DeviceElementGroupImpl.hpp"
#include "DeviceElement.hpp"
#include "DeviceElementBuilder.hpp"
#include "DeviceGroupBuilder.hpp"
#include "informationModelExceptions.hpp"
#include <memory>

using namespace std;
using namespace Information_Model;

DeviceElementGroupImpl::DeviceElementGroupImpl(const string REF_ID,
                                               const string NAME,
                                               const string DESC)
    : DeviceElementGroup(REF_ID, NAME, DESC) {
      elementId = 0;
    }

string
DeviceElementGroupImpl::addDeviceElement(const std::string NAME,
                                         const std::string DESC,
                                         ElementType ELEMENT_TYPE) {
  incrementElementId();
  const string REF_ID = generate_Reference_ID(ELEMENT_TYPE);
 
  switch (ELEMENT_TYPE)
  {
    case ElementType::Group: {
       pair<string,shared_ptr<DeviceElementGroup>> elPair (REF_ID, shared_ptr<DeviceElementGroup>(new Model_Factory::DeviceGroupBuilder(REF_ID, NAME, DESC)));
       subelements.insert(elPair);
    }; break;
    case ElementType::Observable: 
    case ElementType:: Writable:
    case ElementType::Readonly:
    case ElementType::Function: {
      pair<string,shared_ptr<DeviceElement>> elPair (REF_ID, shared_ptr<DeviceElement>(new Model_Factory::DeviceElementBuilder(REF_ID, NAME, DESC, ELEMENT_TYPE)));
      subelements.insert(elPair);
    };break;
    default: {}
  }
  

  

  return REF_ID;
}

Information_Model::DeviceElement *
DeviceElementGroupImpl::getSubelement(const std::string REF_ID) {
  if (subelements.find(REF_ID) != subelements.end())
    return subelements.at(REF_ID).get(); 
  return nullptr;
}

vector<std::shared_ptr<Information_Model::DeviceElement>>
DeviceElementGroupImpl::getSubelements() {
  vector<shared_ptr<Information_Model::DeviceElement>> v_subelements;
  for (auto el : subelements)
  {
    v_subelements.push_back(el.second);
  }
  return v_subelements;
}



void DeviceElementGroupImpl::incrementElementId()
{
   this->elementId++; 
}

unsigned int DeviceElementGroupImpl::getNumericElementId()
{
  return elementId;
}


string DeviceElementGroupImpl::generate_Reference_ID(Information_Model::ElementType elementType) {
  const string BASE_ID = this->getElementRefId();
  string element_id = "";

  switch (elementType) {
    case ElementType::Group: {
      auto elId =  getNumericElementId()-1;
      incrementElementId();
      
      element_id = to_string(elId) + ".";

      break;
    }
    case ElementType::Readonly:
    case ElementType::Observable:
    case ElementType::Writable:
    case ElementType::Function: {
      auto element_number = getNumericElementId()-1;
      element_id = to_string(element_number);
      incrementElementId();
      break;
    }
    case ElementType::Undefined:
    default: {
      throw UndefinedElementTypeException("Failed to generate refid because ElementType is undefined.");
    }
  }
  return BASE_ID + element_id;
}