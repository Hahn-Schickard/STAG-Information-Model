#ifndef __STAG_INFORMATION_MODEL_DATA_VARIANT_HPP_
#define __STAG_INFORMATION_MODEL_DATA_VARIANT_HPP_

#include <Variant_Visitor/Visitor.hpp>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <memory>
#include <optional>
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
  explicit DateTime(const std::time_t& time_point) : posix_time_(time_point) {}

  /**
   * @brief Converts POSIX time to human readable format
   *
   * @return std::string
   */
  std::string toString() const {
    std::string result;
    result.reserve(std::size("yyyy-mm-ddThh:mm:ssZ"));
    std::strftime(std::data(result),
        std::size(result),
        "%FT%TZ",
        std::localtime(&posix_time_));
    return result;
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
enum class DataType : uint8_t {
  Boolean, /*!< bool */
  Integer, /*!< intmax_t */
  Unsigned_Integer, /*!< uintmax_t */
  Double, /*!< double */
  Time, /*!< Information_Model::DateTime */
  Opaque, /*!< std::vector<uint8_t> */
  String, /*!< std::string */
  None, /*!< void */
  Unknown /*!< fallback type */
};

inline std::string toString(DataType type) {
  switch (type) {
  case DataType::Boolean:
    return "Boolean";
  case DataType::Integer:
    return "Signed Integer";
  case DataType::Unsigned_Integer:
    return "Unsigned Integer";
  case DataType::Double:
    return "Double Floating Point";
  case DataType::Time:
    return "Time";
  case DataType::Opaque:
    return "Opaque Byte Array";
  case DataType::String:
    return "String";
  case DataType::None:
    return "None";
  case DataType::Unknown:
  default:
    return "Unknown";
  }
}

inline std::string toSanitizedString(DataType type) {
  auto not_sanitized = toString(type);
  not_sanitized.erase(std::remove_if(not_sanitized.begin(),
                          not_sanitized.end(),
                          [](unsigned char c) { return std::isspace(c); }),
      not_sanitized.end());
  return not_sanitized;
}

using DataVariant = std::variant<bool,
    intmax_t,
    uintmax_t,
    double,
    DateTime,
    std::vector<uint8_t>,
    std::string>;

using DataVariantPtr = std::shared_ptr<DataVariant>;

inline std::size_t size_of(const DataVariant& variant) {
  return Variant_Visitor::match(
      variant,
      [](auto value) -> std::size_t { return sizeof(value); },
      [](const DateTime& value) -> std::size_t { return value.size(); },
      [](const std::vector<uint8_t>& value) -> std::size_t {
        return value.size();
      },
      [](const std::string& value) -> std::size_t { return value.size(); });
}

inline std::optional<DataVariant> setVariant(DataType type) {
  switch (type) {
  case DataType::Boolean:
    return DataVariant((bool)false);
  case DataType::Integer:
    return DataVariant((intmax_t)0);
  case DataType::Unsigned_Integer:
    return DataVariant((uintmax_t)0);
  case DataType::Double:
    return DataVariant((double)0.0);
  case DataType::Time:
    return DataVariant(DateTime());
  case DataType::Opaque:
    return DataVariant(std::vector<uint8_t>());
  case DataType::String:
    return DataVariant(std::string());
  case DataType::None:
    return std::nullopt;
  case DataType::Unknown:
    [[fallthrough]];
  default:
    throw std::logic_error("Can not initialise variant with unknown data type");
  }
}

inline DataType toDataType(const DataVariant& variant) {
  if (std::holds_alternative<bool>(variant)) {
    return DataType::Boolean;
  } else if (std::holds_alternative<intmax_t>(variant)) {
    return DataType::Integer;
  } else if (std::holds_alternative<uintmax_t>(variant)) {
    return DataType::Unsigned_Integer;
  } else if (std::holds_alternative<double>(variant)) {
    return DataType::Double;
  } else if (std::holds_alternative<DateTime>(variant)) {
    return DataType::Time;
  } else if (std::holds_alternative<std::vector<uint8_t>>(variant)) {
    return DataType::Opaque;
  } else if (std::holds_alternative<std::string>(variant)) {
    return DataType::String;
  } else {
    return DataType::Unknown;
  }
}

inline bool matchVariantType(const DataVariant& variant, DataType type) {
  return toDataType(variant) == type;
}

inline std::string toString(const DataVariant& variant) {
  return Variant_Visitor::match(
      variant,
      [](bool value) -> std::string { return (value ? "true" : "false"); },
      [](auto value) -> std::string { return std::to_string(value); },
      [](const DateTime& value) -> std::string { return value.toString(); },
      [](const std::vector<uint8_t>& value) -> std::string {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (auto byte : value) {
          ss << std::hex << std::setw(2) << static_cast<int>(byte) << " ";
        }
        return ss.str();
      },
      [](const std::string& value) -> std::string { return value; });
}

/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_DATA_VARIANT_HPP_