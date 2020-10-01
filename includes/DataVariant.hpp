#ifndef __DATA_MODEL_DATA_VARIANT_HPP_
#define __DATA_MODEL_DATA_VARIANT_HPP_

#include <string>
#include <variant>
#include <vector>

namespace Information_Model {

/**
 * @enum DataTypeEnum
 * @brief DataType enumeration, specifying the supported data types
 *
 */
typedef enum DataTypeEnum {
  BOOLEAN,          /*!< bool */
  INTEGER,          /*!< int32_t */
  UNSIGNED_INTEGER, /*!< uint32_t */
  DOUBLE,           /*!< doable */
  OPAQUE,           /*!< std::vector<uint8_t> */
  STRING,           /*!< std::string */
  UNKNOWN           /*!< fallback type */
} DataType;

using DataVariant = std::variant<bool, int64_t, uint64_t, double,
                                 std::vector<uint8_t>, std::string>;
} // namespace Information_Model
#endif //__DATA_MODEL_DATA_VARIANT_HPP_