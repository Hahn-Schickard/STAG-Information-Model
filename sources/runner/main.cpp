#include "blueprint.hpp"
#include <iostream>

using namespace std;

int main() {
  Blueprint *blueprint = new Blueprint("123", "TestDevice", "A Test Device");
  auto refId = blueprint->AddDeviceGroup("Test", "Test Blueprint");

  cout << refId << endl;

  delete blueprint;

  Blueprint *default_blueprint = new Blueprint();
  Device *default_device = default_blueprint->GetDevice();

  cout << "Device Name: " << default_device->getElementName() << endl;
  cout << "Device Description: " << default_device->getElementDescription()
       << endl;
  cout << "Device identifier: " << default_device->getReferenceId() << endl;
  vector<DeviceElementGroup> device_groups =
      default_device->getDeviceElementGroups();
  cout << "Device has " << device_groups.size() << " number of groups" << endl;

  for (size_t i = 0; i < device_groups.size(); i++) {
    vector<DeviceElement> subelements = device_groups[i].getSubElements();

    cout << "Group: " << device_groups[i].getElementName() << " has "
         << subelements.size() << " number of subelements. "
         << "This group is identified by: " << device_groups[i].getReferenceId()
         << endl;
    for (size_t j = 0; j < subelements.size(); j++) {
      cout << "Subgroup: " << subelements[j].getElementName()
           << " is of type: " << subelements[j].getElementType()
           << " is identified by: " << subelements[j].getReferenceId()
           << " and has a description: "
           << subelements[j].getElementDescription() << endl;
    }
  }

  delete default_blueprint;

  return 0;
}