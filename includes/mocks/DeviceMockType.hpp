#ifndef __INFORMATION_MODEL_DEVICE_MOCK_TYPE_HPP
#define __INFORMATION_MODEL_DEVICE_MOCK_TYPE_HPP

namespace Information_Model {
namespace testing {

enum class DeviceMockType {
  DEFAULT, /*!< Gmock uninteresting calls will be treated as warnnings at
              runtime */
  NICE, /*!< Gmock uninteresting calls will be ignored and suppressed at runtime
         */
  STRICT /*!< Gmock uninteresting calls will be treated as error at runtime */
};
}
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_MOCK_TYPE_HPP