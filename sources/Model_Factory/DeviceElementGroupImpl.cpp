#include "DeviceElementGroupImpl.hpp"
#include "DeviceElement.hpp"
#include "DeviceElementBuilder.hpp"
#include "DeviceGroupBuilder.hpp"
#include "informationModelExceptions.hpp"
#include <memory>

using namespace std;
using namespace Model_Factory;
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
  const string REF_ID = generate_Reference_ID();
 
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


Information_Model::DeviceElement * DeviceElementGroupImpl::findSubelement(const std::string REF_ID)
{
   if (subelements.find(REF_ID) != subelements.end())
    return subelements.at(REF_ID).get(); 

   for (auto el = subelements.begin(); el != subelements.end(); ++ el)
    {
      if (el->second->getElementType() == ElementType::Group)
      {
          auto groupElement = static_cast<DeviceElementGroupImpl*>(el->second.get());
          auto subEl = groupElement->findSubelement(REF_ID);
          if (subEl != nullptr)
          {
            return subEl;
          }
           
      }
    }

    return nullptr;
}

Information_Model::DeviceElement *
DeviceElementGroupImpl::getSubelement(const std::string REF_ID) {
  
 
  
  auto el = findSubelement(REF_ID);
  if (el != nullptr)
    return el;
  else
    throw InvalidReferenceIdException("RefId " + REF_ID + " not found.");
  
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


string DeviceElementGroupImpl::generate_Reference_ID() {
  const string BASE_ID = this->getElementRefId();
  string element_id = "";
  auto elId =  getNumericElementId();

  if (BASE_ID.back() == ':')
    element_id = to_string(elId);
  else
    element_id = "." + to_string(elId);
   incrementElementId();
  return BASE_ID + element_id;

}