#ifndef __INFORMATION_MODEL_DATA_VARIANT_HPP_
#define __INFORMATION_MODEL_DATA_VARIANT_HPP_

#include <chrono>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace Information_Model {

/**
 * @brief POSIX time wrapper class with inbuilt comparators
 *
 */
class DateTime {
  std::time_t posix_time_;

public:
  /**
   * @brief Sets this time point as current system time at the point of calling
   *
   */
  DateTime() : posix_time_(std::time(nullptr)) {}

  /**
   * @brief Sets Specified time point
   *
   * @param time_point
   */
  DateTime(std::time_t time_point) : posix_time_(time_point) {}

  /**
   * @brief Converts POSIX time to huma nreadable format
   *
   * @return std::string
   */
  std::string toString() const {
    return std::string(std::asctime(std::localtime(&posix_time_)));
  }

  /**
   * @brief Returns the amount of seconds since POSIX time epoch
   *
   * @return std::time_t
   */
  std::time_t getValue() const { return posix_time_; }

  friend bool operator==(const DateTime &lhs, const DateTime &rhs);
  friend bool operator!=(const DateTime &lhs, const DateTime &rhs);
  friend bool operator<(const DateTime &lhs, const DateTime &rhs);
  friend bool operator>(const DateTime &lhs, const DateTime &rhs);
  friend bool operator<=(const DateTime &lhs, const DateTime &rhs);
  friend bool operator>=(const DateTime &lhs, const DateTime &rhs);
};

inline bool operator==(const DateTime &lhs, const DateTime &rhs) {
  return (lhs.posix_time_ == rhs.posix_time_);
}
inline bool operator!=(const DateTime &lhs, const DateTime &rhs) {
  return (lhs.posix_time_ != rhs.posix_time_);
}
inline bool operator<(const DateTime &lhs, const DateTime &rhs) {
  return (lhs.posix_time_ < rhs.posix_time_);
}
inline bool operator>(const DateTime &lhs, const DateTime &rhs) {
  return (lhs.posix_time_ > rhs.posix_time_);
}
inline bool operator<=(const DateTime &lhs, const DateTime &rhs) {
  return (lhs.posix_time_ <= rhs.posix_time_);
}
inline bool operator>=(const DateTime &lhs, const DateTime &rhs) {
  return (lhs.posix_time_ >= rhs.posix_time_);
}

/**
 * @enum DataType
 * @brief DataType enumeration, specifying the supported data types
 *
 */
enum class DataType {
  BOOLEAN,          /*!< bool */
  INTEGER,          /*!< int64_t */
  UNSIGNED_INTEGER, /*!< uint64_t */
  DOUBLE,           /*!< double */
  TIME,             /*!< Infromation_Model::DateTime */
  OPAQUE,           /*!< std::vector<uint8_t> */
  STRING,           /*!< std::string */
  UNKNOWN           /*!< fallback type */
};

inline std::string toString(DataType type) {
  switch (type) {
  case DataType::BOOLEAN:
    return "Boolean";
  case DataType::INTEGER:
    return "Signed Integer";
  case DataType::UNSIGNED_INTEGER:
    return "Unsigned Integer";
  case DataType::DOUBLE:
    return "Double floating point";
  case DataType::TIME:
    return "Time";
  case DataType::OPAQUE:
    return "Opaque byte array";
  case DataType::STRING:
    return "String";
  case DataType::UNKNOWN:
  default:
    return "Unknown";
  }
}

using DataVariant = std::variant<bool, int64_t, uint64_t, double, DateTime,
                                 std::vector<uint8_t>, std::string>;

DataVariant setVariant(DataType type) {
  switch (type) {
  case DataType::BOOLEAN:
    return DataVariant((bool)false);
  case DataType::INTEGER:
    return DataVariant((int64_t)0);
  case DataType::UNSIGNED_INTEGER:
    return DataVariant((uint64_t)0);
  case DataType::DOUBLE:
    return DataVariant((double)0.0);
  case DataType::TIME:
    return DataVariant(DateTime());
  case DataType::OPAQUE:
    return DataVariant(std::vector<uint8_t>());
  case DataType::STRING:
    return DataVariant(std::string());
  case DataType::UNKNOWN:
  default:
    throw std::logic_error("Can not initialise variant with unknown data type");
  }
}
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DATA_VARIANT_HPP_