#include "DataVariant.hpp"

#include <Variant_Visitor/Visitor.hpp>
#include <date/date.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace Information_Model {
using namespace std;

bool operator==(const Timestamp& lhs, const Timestamp& rhs) {
  // clang-format off
  return lhs.year         == rhs.year && 
         lhs.month        == rhs.month &&
         lhs.day          == rhs.day &&
         lhs.hours        == rhs.hours && 
         lhs.minutes      == rhs.minutes &&
         lhs.seconds      == rhs.seconds &&
         lhs.microseconds == rhs.microseconds;
  // clang-format on
}

bool operator!=(const Timestamp& lhs, const Timestamp& rhs) {
  return !(lhs == rhs);
}

bool operator<(const Timestamp& lhs, const Timestamp& rhs) {
  return tie(lhs.year,
             lhs.month,
             lhs.day,
             lhs.hours,
             lhs.minutes,
             lhs.seconds,
             lhs.microseconds) < tie(rhs.year,
                                     rhs.month,
                                     rhs.day,
                                     rhs.hours,
                                     rhs.minutes,
                                     rhs.seconds,
                                     rhs.microseconds);
}

bool operator>(const Timestamp& lhs, const Timestamp& rhs) { return rhs < lhs; }

bool operator<=(const Timestamp& lhs, const Timestamp& rhs) {
  return !(lhs > rhs);
}

bool operator>=(const Timestamp& lhs, const Timestamp& rhs) {
  return !(lhs < rhs);
}

void verifyTimestamp(const Timestamp& time) {
  // NOLINTBEGIN(readability-magic-numbers)
  if (time.year < 1582) {
    throw invalid_argument("Ancient time recording is not supported");
  }
  if (time.month == 0) {
    throw invalid_argument("A year can not have 0 months");
  }
  if (time.month > 12) {
    throw invalid_argument("A year can not have more than 12 months");
  }
  if (time.day == 0) {
    throw invalid_argument("A Month can not have 0 days");
  }
  if (time.day > 31) {
    throw invalid_argument("A Month can not have more than 31 days");
  }
  if (time.hours > 24) {
    throw invalid_argument("A day can not have more than 24 hours");
  }
  if (time.minutes > 59) {
    throw invalid_argument("An hour can not have more than 59 minutes");
  }
  if (time.seconds > 59) {
    // chrono::system_clock does not handle leap seconds, so we wont either
    throw invalid_argument("A minute can not have more than 59 seconds");
  }
  // NOLINTEND(readability-magic-numbers)
}

Timestamp makeTimestamp() { return toTimestamp(chrono::system_clock::now()); }

Timestamp toTimestamp(const chrono::system_clock::time_point& timepoint) {
  auto days_count = date::floor<date::days>(timepoint);
  auto calendar_date = date::year_month_day{days_count};
  auto daytime = date::make_time(
      chrono::duration_cast<chrono::microseconds>(timepoint - days_count));
  Timestamp result{// clang-format off
    .year=static_cast<uint16_t>(int{calendar_date.year()}),
    .month=static_cast<uint8_t>(unsigned{calendar_date.month()}),
    .day=static_cast<uint8_t>(unsigned{calendar_date.day()}),
    .hours=static_cast<uint8_t>(daytime.hours().count()),
    .minutes=static_cast<uint8_t>(daytime.minutes().count()),
    .seconds=static_cast<uint8_t>(daytime.seconds().count()),
    .microseconds=static_cast<uint32_t>(daytime.subseconds().count())
  }; // clang-format on
  return result;
}

chrono::system_clock::time_point toTimepoint(const Timestamp& timestamp) {
  verifyTimestamp(timestamp);
  auto result = // clang-format off
      date::sys_days{
        date::year_month_day{
          date::year{timestamp.year},
          date::month{timestamp.month},
          date::day{timestamp.day}
        }} +
      chrono::hours{timestamp.hours} + 
      chrono::minutes{timestamp.minutes} +
      chrono::seconds{timestamp.seconds} +
      chrono::microseconds{timestamp.microseconds};
  // clang-format on
  return result;
}

string toString(const Timestamp& timestamp) {
  return toString(timestamp, "%Y-%m-%dT%H:%M:%SZ");
}

string toString(const Timestamp& timestamp, const string& format) {
  return toString(toTimepoint(timestamp), format);
}

string toString(const chrono::system_clock::time_point& timepoint) {
  return toString(timepoint, "%Y-%m-%dT%H:%M:%SZ");
}

string toString(
    const chrono::system_clock::time_point& timepoint, const string& format) {
  return date::format(format, date::floor<chrono::microseconds>(timepoint));
}

string toString(DataType type) {
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

string toSanitizedString(DataType type) {
  auto not_sanitized = toString(type);
  not_sanitized.erase(remove_if(not_sanitized.begin(),
                          not_sanitized.end(),
                          [](unsigned char c) { return isspace(c); }),
      not_sanitized.end());
  return not_sanitized;
}

size_t size_of(const DataVariant& variant) {
  return Variant_Visitor::match(
      variant,
      [](auto value) { return sizeof(value); },
      [](const vector<uint8_t>& value) { return value.size(); },
      [](const string& value) { return value.size(); });
}

optional<DataVariant> setVariant(DataType type) {
  switch (type) {
  case DataType::Boolean:
    return DataVariant(false);
  case DataType::Integer:
    return DataVariant((intmax_t)1);
  case DataType::Unsigned_Integer:
    return DataVariant((uintmax_t)-1);
  case DataType::Double:
    return DataVariant(0.1); // NOLINT(readability-magic-numbers)
  case DataType::Time:
    return DataVariant(makeTimestamp());
  case DataType::Opaque:
    return DataVariant(vector<uint8_t>{});
  case DataType::String:
    return DataVariant(string{});
  case DataType::None:
    return nullopt;
  case DataType::Unknown:
    [[fallthrough]];
  default:
    throw logic_error("Can not initialise variant with unknown data type");
  }
}

DataType toDataType(const DataVariant& variant) {
  if (holds_alternative<bool>(variant)) {
    return DataType::Boolean;
  } else if (holds_alternative<intmax_t>(variant)) {
    return DataType::Integer;
  } else if (holds_alternative<uintmax_t>(variant)) {
    return DataType::Unsigned_Integer;
  } else if (holds_alternative<double>(variant)) {
    return DataType::Double;
  } else if (holds_alternative<Timestamp>(variant)) {
    return DataType::Time;
  } else if (holds_alternative<vector<uint8_t>>(variant)) {
    return DataType::Opaque;
  } else if (holds_alternative<string>(variant)) {
    return DataType::String;
  } else {
    return DataType::Unknown;
  }
}

bool matchVariantType(const DataVariant& variant, DataType type) {
  return toDataType(variant) == type;
}

string toString(const DataVariant& variant) {
  return Variant_Visitor::match(
      variant,
      [](bool value) -> string { return (value ? "True" : "False"); },
      [](auto value) -> string { return to_string(value); },
      [](const Timestamp& value) -> string { return toString(value); },
      [](const vector<uint8_t>& value) -> string {
        stringstream ss;
        ss << hex << setfill('0');
        for (auto byte : value) {
          ss << hex << setw(2) << static_cast<int>(byte) << " ";
        }
        return ss.str();
      },
      [](const string& value) -> string { return value; });
}

string toSanitizedString(const DataVariant& variant) {
  return Variant_Visitor::match(
      variant,
      [](bool value) -> string { return (value ? "True" : "False"); },
      [](double value) -> string {
        string result;
        if (value < 0) {
          auto abs_value = std::abs(value);
          result = "Neg" + to_string(abs_value);
        } else {
          result = to_string(value);
        }
        replace(result.begin(), result.end(), '.', 'P');
        return result;
      },
      [](auto value) -> string {
        string result;
        if (value < 0) {
          // avoid ambiguous abs()
          auto abs_value = std::abs(static_cast<intmax_t>(value));
          result = "Neg" + to_string(abs_value);
        } else {
          result = to_string(value);
        }
        return result;
      },
      [](const Timestamp& value) -> string {
        auto result = toString(value, "%YY%mM%dD%HH%MM%SZ");
        replace(result.begin(), result.end(), '.', 'P');
        return result;
      },
      [](const vector<uint8_t>& value) -> string {
        if (value.empty()) {
          return "NullOpaque";
        } else {
          stringstream ss;
          ss << hex << setfill('0');
          for (auto byte : value) {
            ss << hex << setw(2) << static_cast<int>(byte);
          }
          return "0x" + ss.str();
        }
      },
      [](string value) -> string {
        replace(value.begin(), value.end(), ' ', '_');
        if (value.empty()) {
          return "NullString";
        } else {
          return value;
        }
      });
}
} // namespace Information_Model
