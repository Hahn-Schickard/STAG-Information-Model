#ifndef __INFORMATION_MODEL_DATA_VARIANT_HPP_
#define __INFORMATION_MODEL_DATA_VARIANT_HPP_

#include "Variant_Visitor.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace Information_Model {
/**
 * @addtogroup DataTypeModelling Data Type Modelling
 * @{
 */
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
  DateTime(const std::time_t& time_point) : posix_time_(time_point) {}

  /**
   * @brief Converts POSIX time to human readable format
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

  std::size_t size() const { return sizeof(posix_time_); }

  friend bool operator==(const DateTime& lhs, const DateTime& rhs);
  friend bool operator!=(const DateTime& lhs, const DateTime& rhs);
  friend bool operator<(const DateTime& lhs, const DateTime& rhs);
  friend bool operator>(const DateTime& lhs, const DateTime& rhs);
  friend bool operator<=(const DateTime& lhs, const DateTime& rhs);
  friend bool operator>=(const DateTime& lhs, const DateTime& rhs);
};

inline bool operator==(const DateTime& lhs, const DateTime& rhs) {
  return (lhs.posix_time_ == rhs.posix_time_);
}
inline bool operator!=(const DateTime& lhs, const DateTime& rhs) {
  return (lhs.posix_time_ != rhs.posix_time_);
}
inline bool operator<(const DateTime& lhs, const DateTime& rhs) {
  return (lhs.posix_time_ < rhs.posix_time_);
}
inline bool operator>(const DateTime& lhs, const DateTime& rhs) {
  return (lhs.posix_time_ > rhs.posix_time_);
}
inline bool operator<=(const DateTime& lhs, const DateTime& rhs) {
  return (lhs.posix_time_ <= rhs.posix_time_);
}
inline bool operator>=(const DateTime& lhs, const DateTime& rhs) {
  return (lhs.posix_time_ >= rhs.posix_time_);
}

/**
 * @enum DataType
 * @brief DataType enumeration, specifying the supported data types
 *
 */
enum class DataType {
  BOOLEAN, /*!< bool */
  INTEGER, /*!< intmax_t */
  UNSIGNED_INTEGER, /*!< uintmax_t */
  DOUBLE, /*!< double */
  TIME, /*!< Information_Model::DateTime */
  OPAQUE, /*!< std::vector<uint8_t> */
  STRING, /*!< std::string */
  NONE, /*!< void */
  UNKNOWN /*!< fallback type */
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
  case DataType::NONE:
    return "None";
  case DataType::UNKNOWN:
  default:
    return "Unknown";
  }
}

using DataVariant = std::variant<bool,
    intmax_t,
    uintmax_t,
    double,
    DateTime,
    std::vector<uint8_t>,
    std::string>;

inline std::size_t size_of(const DataVariant& variant) {
  std::size_t result;
  match(
      variant,
      [&](auto value) { result = sizeof(value); },
      [&](const DateTime& value) { result = value.size(); },
      [&](const std::vector<uint8_t>& value) { result = value.size(); },
      [&](const std::string& value) { result = value.size(); });
  return result;
}

inline DataVariant setVariant(DataType type) {
  switch (type) {
  case DataType::BOOLEAN:
    return DataVariant((bool)false);
  case DataType::INTEGER:
    return DataVariant((intmax_t)0);
  case DataType::UNSIGNED_INTEGER:
    return DataVariant((uintmax_t)0);
  case DataType::DOUBLE:
    return DataVariant((double)0.0);
  case DataType::TIME:
    return DataVariant(DateTime());
  case DataType::OPAQUE:
    return DataVariant(std::vector<uint8_t>());
  case DataType::STRING:
    return DataVariant(std::string());
  case DataType::NONE:
    throw std::domain_error("Data variant is not used to model None data type");
  case DataType::UNKNOWN:
  default:
    throw std::logic_error("Can not initialise variant with unknown data type");
  }
}

inline DataType toDataType(const DataVariant& variant) {
  if (std::holds_alternative<bool>(variant)) {
    return DataType::BOOLEAN;
  } else if (std::holds_alternative<intmax_t>(variant)) {
    return DataType::INTEGER;
  } else if (std::holds_alternative<uintmax_t>(variant)) {
    return DataType::UNSIGNED_INTEGER;
  } else if (std::holds_alternative<double>(variant)) {
    return DataType::DOUBLE;
  } else if (std::holds_alternative<DateTime>(variant)) {
    return DataType::TIME;
  } else if (std::holds_alternative<std::vector<uint8_t>>(variant)) {
    return DataType::OPAQUE;
  } else if (std::holds_alternative<std::string>(variant)) {
    return DataType::STRING;
  } else {
    return DataType::UNKNOWN;
  }
}

inline bool matchVariantType(const DataVariant& variant, DataType type) {
  return toDataType(variant) == type;
}

inline std::string toString(const DataVariant& variant) {
  std::string result;
  match(
      variant,
      [&](bool value) { result = (value ? "true" : "false"); },
      [&](auto value) { result = std::to_string(value); },
      [&](DateTime value) { result = value.toString(); },
      [&](std::vector<uint8_t> value) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (auto byte : value) {
          ss << std::hex << std::setw(2) << static_cast<int>(byte) << " ";
        }
        result = ss.str();
      },
      [&](std::string value) { result = value; });

  return result;
}

/** @}*/
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DATA_VARIANT_HPP_