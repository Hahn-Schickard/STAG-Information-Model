#include "infomodel\blueprint.hpp"
#include <iostream>

int main() {
  BluePrint *blueprint = new BluePrint("123","TestDevice","A Test Device");
  auto refId = blueprint->AddDeviceGroup("Test", "Test Blueprint");
  
  std::cout << refId << std::endl;
  
  delete blueprint;

  return 0;
}