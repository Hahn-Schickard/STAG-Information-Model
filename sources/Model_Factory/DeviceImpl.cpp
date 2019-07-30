#include "DeviceImpl.hpp"
#include <memory>
#include "informationModelExceptions.hpp"

using namespace std;
using namespace Model_Factory;
using namespace Information_Model;

string DeviceImpl::addDeviceElementGroup(const std::string NAME,
                                         const std::string DESC)
{
  const string REF_ID = this->getElementRefId() + ":";

  unique_ptr<Information_Model::DeviceElementGroup> device_element_group_ptr(
      new DeviceElementGroupImpl(REF_ID, NAME, DESC));

  device_element_group = move(device_element_group_ptr);

  return REF_ID;
}

Information_Model::DeviceElementGroup *DeviceImpl::getDeviceElementGroup()
{
  if (device_element_group == nullptr)
    throw GroupElementDoesNotExistException("Device has no Element Group");
  return device_element_group.get();
}