#include <Information_Model/DeviceBuilder.hpp>
#include <Information_Model/Element.hpp>

#include <iostream>

int main() {
  Information_Model::BuildInfo info;
  Information_Model::DeviceBuilderPtr builder;
  Information_Model::DevicePtr device;
  Information_Model::ElementPtr element;
  Information_Model::ElementType element_type;
  Information_Model::ElementFunction element_function;
  Information_Model::Parameters parameters;
  Information_Model::ParameterTypes supported_parameters;
  Information_Model::DataVariant value;

  std::cout << "Integration test successful." << std::endl;

  return 0;
}