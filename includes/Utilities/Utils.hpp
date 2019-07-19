#ifndef __UTILS_HPP
#define __UTILS_HPP

#include "DeviceElement.hpp"

/**
 * @brief This function generates a Reference ID for a child that will be
 * constructed by Model_Factory::DeviceBuilder
 * 
 * @param parent parent class of the child class
 * @return std::string Reference ID of child
 */
std::string generate_Reference_ID(Information_Model::DeviceElement *parent);

#endif //__UTILS_HPP