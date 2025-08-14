#ifndef __STAG_INFORMATION_MODEL_MOCK_BUILDER_HPP
#define __STAG_INFORMATION_MODEL_MOCK_BUILDER_HPP
#include "DeviceBuilder.hpp"

#include "DeviceMock.hpp"
#include "ElementMock.hpp"

namespace Information_Model::testing {

struct MockBuilder : public DeviceBuilder {
  // implement DeviceBuilder interface
};
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_MOCK_BUILDER_HPP