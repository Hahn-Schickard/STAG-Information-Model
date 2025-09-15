#ifndef __STAG_INFORMATION_MODEL_DATA_VARIANT_HPP_
#define __STAG_INFORMATION_MODEL_DATA_VARIANT_HPP_

#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace Information_Model {
/**
 * @addtogroup DataTypeModelling Data Type Modelling
 * @{
 */

/**
 * @brief UTC Timestamp
 *
 */
struct Timestamp {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  uint32_t microseconds;

  friend bool operator==(const Timestamp& lhs, const Timestamp& rhs);

  friend bool operator!=(const Timestamp& lhs, const Timestamp& rhs);

  friend bool operator<(const Timestamp& lhs, const Timestamp& rhs);

  friend bool operator>(const Timestamp& lhs, const Timestamp& rhs);

  friend bool operator<=(const Timestamp& lhs, const Timestamp& rhs);

  friend bool operator>=(const Timestamp& lhs, const Timestamp& rhs);
};

/**
 * @brief Checks if a given timestamp contains valid values
 *
 * @throws std::invalid_argument if
 *  - year is less than 1582 (ISO 8601 requirement)
 *  - month is 0
 *  - month is more than 12
 *  - day is 0
 *  - day is more than 31
 *  - hours is more than 24
 *  - minutes is more than 59
 *  - seconds is more than 59
 *
 * @param timestamp
 */
void verifyTimestamp(const Timestamp& timestamp);

/**
 * @brief Creates a valid Timestamp for current time
 *
 * Result is the same as calling toTimestamp(std::chrono::system_clock::now())
 *
 * @return Timestamp
 */
Timestamp makeTimestamp();

Timestamp toTimestamp(const std::chrono::system_clock::time_point& timepoint);

std::chrono::system_clock::time_point toTimepoint(const Timestamp& timestamp);

/**
 * @brief Converts a given timestamp to ISO 8601 string format
 *
 * @throws std::invalid_argument - if given timestamp store nonsensical values,
 * see @ref verifyTimestamp() documentation for exact checks
 *
 * @param timestamp
 * @return std::string
 */
std::string toString(const Timestamp& timestamp);

/**
 * @brief Converts a given timestamp to a string based on given strftime format
 * @see https://en.cppreference.com/w/cpp/chrono/c/strftime for reference
 *
 * @throws std::invalid_argument - if given timestamp store nonsensical values,
 * see @ref verifyTimestamp() documentation for exact checks
 *
 * @param timestamp
 * @return std::string
 */
std::string toString(const Timestamp& timestamp, const std::string& format);

/**
 * @brief Converts a given timepoint to ISO 8601 string format
 *
 * @param time_point
 * @return std::string
 */
std::string toString(const std::chrono::system_clock::time_point& timepoint);

/**
 * @brief Converts a given timepoint to a string based on given strftime format
 * @see https://en.cppreference.com/w/cpp/chrono/c/strftime for reference
 *
 * @param timestamp
 * @return std::string
 */
std::string toString(const std::chrono::system_clock::time_point& timepoint,
    const std::string& format);

/**
 * @enum DataType
 * @brief DataType enumeration, specifying the supported data types
 *
 */
enum class DataType : uint8_t {
  Boolean, /*!< bool */
  Integer, /*!< intmax_t */
  Unsigned_Integer, /*!< uintmax_t */
  Double, /*!< double */
  Timestamp, /*!< Information_Model::Timestamp */
  Opaque, /*!< std::vector<uint8_t> */
  String, /*!< std::string */
  None, /*!< void */
  Unknown /*!< fallback type */
};

std::string toString(DataType type);

std::string toSanitizedString(DataType type);

using DataVariant = std::variant<bool,
    intmax_t,
    uintmax_t,
    double,
    Timestamp,
    std::vector<uint8_t>,
    std::string>;

using DataVariantPtr = std::shared_ptr<DataVariant>;

std::size_t size_of(const DataVariant& variant);

std::optional<DataVariant> setVariant(DataType type);

DataType toDataType(const DataVariant& variant);

bool matchVariantType(const DataVariant& variant, DataType type);

std::string toString(const DataVariant& variant);

std::string toSanitizedString(const DataVariant& variant);

/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_DATA_VARIANT_HPP_