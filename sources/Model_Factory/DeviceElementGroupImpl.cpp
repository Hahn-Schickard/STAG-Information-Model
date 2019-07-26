#include "DeviceElementGroupImpl.hpp"
#include "DeviceElement.hpp"
#include "DeviceElementBuilder.hpp"
#include "Utils.hpp"
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
                                         Information_Model::ElementType type) {
  incrementElementId();
  const string REF_ID = generate_Reference_ID(dynamic_cast<Information_Model::DeviceElement *>(this));
 
  pair<string,shared_ptr<DeviceElement>> elPair (REF_ID, shared_ptr<DeviceElement>(new DeviceElementBuilder(REF_ID, NAME, DESC, type)));

  subelements.insert(elPair);

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