#ifndef __DEVICE_ELEMENT_BUILDER_HPP
#define __DEVICE_ELEMENT_BUILDER_HPP

#include "DeviceElement.hpp"
#include <memory>
#include <string>

namespace Model_Factory {

class DeviceElementBuilder : public Information_Model::DeviceElement {
public:
  DeviceElementBuilder(const std::string REF_ID, const std::string NAME,
                       const std::string DESC,
                       Information_Model::ElementType type)
      : DeviceElement(REF_ID, NAME, DESC, type) {}
};
}

#endif //__DEVICE_ELEMENT_BUILDER_HPP