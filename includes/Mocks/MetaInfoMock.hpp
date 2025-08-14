#ifndef __STAG_INFORMATION_MODEL_META_INFO_MOCK_HPP
#define __STAG_INFORMATION_MODEL_META_INFO_MOCK_HPP
#include "MetaInfo.hpp"

#include <gmock/gmock.h>

namespace Information_Model::testing {

struct MetaInfoMock : virtual public MetaInfo {
  MetaInfoMock() = default;

  ~MetaInfoMock() override = default;

  MOCK_METHOD(std::string, id, (), (const final));
  MOCK_METHOD(std::string, name, (), (const final));
  MOCK_METHOD(std::string, description, (), (const final));
};
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_META_INFO_MOCK_HPP