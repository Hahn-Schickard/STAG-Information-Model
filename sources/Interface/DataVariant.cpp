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
using namespace chrono;

DateTime::DateTime()
    : timestamp_(date::floor<microseconds>(chrono::system_clock::now())) {}

DateTime::DateTime(size_t posix_time_point)
    : timestamp_(seconds(posix_time_point)) {}

string DateTime::toString() const { return DateTime::toString("%FT%TZ"); }

string DateTime::toString(const string& format) const {
  return date::format(format, date::floor<microseconds>(timestamp_));
}

DateTime::TimePoint DateTime::timePoint() const { return timestamp_; }

size_t DateTime::size() const { return sizeof(timestamp_); }

bool operator==(const DateTime& lhs, const DateTime& rhs) {
  return lhs.timePoint() == rhs.timePoint();
}
bool operator!=(const DateTime& lhs, const DateTime& rhs) {
  return !(lhs == rhs);
}
bool operator<(const DateTime& lhs, const DateTime& rhs) {
  return lhs.timePoint() < rhs.timePoint();
}
bool operator>(const DateTime& lhs, const DateTime& rhs) { return rhs < lhs; }

bool operator<=(const DateTime& lhs, const DateTime& rhs) {
  return !(lhs > rhs);
}
bool operator>=(const DateTime& lhs, const DateTime& rhs) {
  return !(lhs < rhs);
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
      [](auto value) -> size_t { return sizeof(value); },
      [](const DateTime& value) -> size_t { return value.size(); },
      [](const vector<uint8_t>& value) -> size_t { return value.size(); },
      [](const string& value) -> size_t { return value.size(); });
}

optional<DataVariant> setVariant(DataType type) {
  switch (type) {
  case DataType::Boolean:
    return DataVariant(false);
  case DataType::Integer:
    return DataVariant((intmax_t)0);
  case DataType::Unsigned_Integer:
    return DataVariant((uintmax_t)0);
  case DataType::Double:
    return DataVariant(0.0);
  case DataType::Time:
    return DataVariant(DateTime());
  case DataType::Opaque:
    return DataVariant(vector<uint8_t>());
  case DataType::String:
    return DataVariant(string());
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
  } else if (holds_alternative<DateTime>(variant)) {
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
      [](bool value) -> string { return (value ? "true" : "false"); },
      [](auto value) -> string { return to_string(value); },
      [](const DateTime& value) -> string { return value.toString(); },
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
          auto abs_value = abs(value);
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
          auto abs_value = abs(static_cast<intmax_t>(value));
          result = "Neg" + to_string(abs_value);
        } else {
          result = to_string(value);
        }
        return result;
      },
      [](const DateTime& value) -> string {
        auto result = value.toString("%YY%mM%dD%HH%MM%S");
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
          return "Hex" + ss.str();
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
