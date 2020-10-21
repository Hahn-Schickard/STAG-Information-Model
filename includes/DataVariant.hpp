#ifndef __INFORMATION_MODEL_DATA_VARIANT_HPP_
#define __INFORMATION_MODEL_DATA_VARIANT_HPP_

#include <string>
#include <variant>
#include <vector>

namespace Information_Model {
/**
 * @enum DataType
 * @brief DataType enumeration, specifying the supported data types
 *
 */
enum class DataType {
  BOOLEAN,          /*!< bool */
  INTEGER,          /*!< int64_t */
  UNSIGNED_INTEGER, /*!< uint64_t */
  DOUBLE,           /*!< doable */
  OPAQUE,           /*!< std::vector<uint8_t> */
  STRING,           /*!< std::string */
  UNKNOWN           /*!< fallback type */
};

using DataVariant = std::variant<bool, int64_t, uint64_t, double,
                                 std::vector<uint8_t>, std::string>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DATA_VARIANT_HPP_