#include "DeviceBuilder.hpp"
#include "DeviceElementBuilder.hpp"
#include "Utils.hpp"
#include "informationModelExceptions.hpp"
#include <iostream>

using namespace std;
using namespace Model_Factory;
using namespace Information_Model;

DeviceBuilder::DeviceBuilder(const std::string NAME, const std::string REF_ID,
                             const std::string DESC) {
  unique_ptr<Information_Model::Device> device_ptr(
      new DeviceImpl(REF_ID, NAME, DESC));
  device = move(device_ptr);
}

string DeviceBuilder::addDeviceElementGroup(const std::string NAME, const std::string DESC) {
  DeviceImpl *device_impl = dynamic_cast<DeviceImpl *>(device.get());
  string REF_ID = device_impl->addDeviceElementGroup(NAME, DESC);
  return REF_ID;
}

string DeviceBuilder::addDeviceElement(const std::string NAME, const std::string DESC, ElementType type) {
  DeviceElementGroupImpl *device_element_group =  dynamic_cast<DeviceElementGroupImpl *>(device->getDeviceElementGroup());
  const string REF_ID = device_element_group->addDeviceElement(NAME, DESC, type);
  return REF_ID;
}

 






string DeviceBuilder::addDeviceElement(const std::string GROUP_REFID, const std::string NAME,
                                       const std::string DESC,
                                       ElementType type) 
{ 
  try { 
    shared_ptr<DeviceElementGroupImpl> device_element_group = getDeviceElementGroup();
    auto refId = device_element_group->getElementRefId();
    
   // string refId = nullptr;
    if (device_element_group == nullptr)
    {      
      return nullptr;
    }

    if (device_element_group->getElementRefId() != GROUP_REFID) {
      refId = addDeviceElementToSubgroup(device_element_group.get(), GROUP_REFID, NAME, DESC, type );
    }
    else {
        refId = device_element_group->addDeviceElement(NAME, DESC, type); 
    }
    
    return refId;  
  }
  catch(exception ex)
  {
    cerr << "addDeviceElement threw an excepton: " << ex.what() << ". End" << endl;
  }
  return nullptr;
}

shared_ptr<DeviceElementGroupImpl>  DeviceBuilder::getDeviceElementGroup()
{
  try {
 
    auto group = static_cast<DeviceElementGroupImpl*>(device->getDeviceElementGroup());

    shared_ptr<DeviceElementGroupImpl> device_element_group(group);
    return device_element_group;
  }
  catch (exception ex)
  {
    throw;
  }
}




string DeviceBuilder::addDeviceElementToSubgroup(DeviceElementGroupImpl * parentGroup, const string GROUP_REFID, const string NAME, const string DESC, ElementType type)
{
  try {
   auto subelementGroup = getSubelementGroup(parentGroup, GROUP_REFID);
   if (subelementGroup == nullptr) {
      subelementGroup = findElementGroup(GROUP_REFID, parentGroup);
   }       
   if (subelementGroup != nullptr) {      
      return subelementGroup->addDeviceElement(NAME, DESC, type);
   }
    
  return nullptr;
  }
  catch(exception ex)
  {
    cerr << "addDeviceElementToSubgroup threw an exception: "  << ex.what() << endl;
    throw;
  }
}

shared_ptr<DeviceElementGroupImpl> DeviceBuilder::getSubelementGroup(DeviceElementGroupImpl * deviceElementGroup, string REFID)
{
  DeviceElement * el = deviceElementGroup->getSubelement(REFID);
  if (el != nullptr && el->getElementRefId() == REFID)
  {
    
    auto subelement = static_cast<DeviceElementGroupImpl*>(el);
    if (subelement != nullptr && subelement->getElementType() == ElementType::Group)
      {
        shared_ptr<DeviceElementGroupImpl> device_element_group(subelement);
        return device_element_group;
      }
  }
  return nullptr;
  
    
}

shared_ptr<DeviceElementGroupImpl> DeviceBuilder::findElementGroup(string RefId, DeviceElementGroupImpl * deviceElementGroup)
 {  
    auto subElements = deviceElementGroup->getSubelements();
          
     for (auto element : subElements)
     {
             
       if (element->getElementRefId() == RefId)
       {
         if (element->getElementType() == ElementType::Group)
         {
           return dynamic_pointer_cast<DeviceElementGroupImpl>(element);
         }
       }
       else if (element->getElementType() == ElementType::Group)
        {
          auto elementGroup = dynamic_pointer_cast<DeviceElementGroupImpl>(element);        
          return findElementGroup(RefId, elementGroup.get());
        }                                
     }
     return nullptr;
 }



unique_ptr<Information_Model::Device> DeviceBuilder::getDevice() {
  return move(device);
}