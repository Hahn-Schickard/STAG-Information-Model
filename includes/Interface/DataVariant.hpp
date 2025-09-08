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
 * @brief POSIX time wrapper class with inbuilt comparators
 *
 */
struct DateTime {
  using TimePoint = std::chrono::system_clock::time_point;
  /**
   * @brief Sets this time point as current system time at the point of calling
   *
   */
  DateTime();

  /**
   * @brief Sets Specified time point
   *
   * @param time_point
   */
  explicit DateTime(size_t posix_time_point);
  /**
   * @brief Converts POSIX time to human readable format
   *
   * @return std::string
   */
  std::string toString() const;

  std::string toString(const std::string& format) const;

  /**
   * @brief Returns the amount of seconds since POSIX time epoch
   *
   * @return std::time_t
   */
  TimePoint timePoint() const;

  std::size_t size() const;

  friend bool operator==(const DateTime& lhs, const DateTime& rhs);

  friend bool operator!=(const DateTime& lhs, const DateTime& rhs);

  friend bool operator<(const DateTime& lhs, const DateTime& rhs);

  friend bool operator>(const DateTime& lhs, const DateTime& rhs);

  friend bool operator<=(const DateTime& lhs, const DateTime& rhs);

  friend bool operator>=(const DateTime& lhs, const DateTime& rhs);

private:
  TimePoint timestamp_;
};
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

std::string toString(DataType type);

std::string toSanitizedString(DataType type);

using DataVariant = std::variant<bool,
    intmax_t,
    uintmax_t,
    double,
    DateTime,
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