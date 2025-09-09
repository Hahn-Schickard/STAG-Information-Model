#include "Callable.hpp"

namespace Information_Model {
using namespace std;

ResultFuture::ResultFuture(
    shared_ptr<uintmax_t> id, future<DataVariant>&& result)
    : id_(id), result_(move(result)) {}

DataVariant ResultFuture::get() {
  auto result = result_.get();
  return result;
}

uintmax_t ResultFuture::id() const { return *id_; }

bool operator==(const ParameterType& lhs, const ParameterType& rhs) {
  return lhs.mandatory == rhs.mandatory && lhs.type == rhs.type;
}

bool operator!=(const ParameterType& lhs, const ParameterType& rhs) {
  return !(lhs == rhs);
}

void checkParameter(uintmax_t position,
    const optional<DataVariant>& given,
    const ParameterType& expected) {
  if (const auto& given_value = given) {
    auto given_type = toDataType(*given_value);
    if (given_type != expected.type) {
      throw ParameterTypeMismatch(position, expected.type, given_type);
    }
  } else if (expected.mandatory) {
    throw MandatoryParameterHasNoValue(position, expected.type);
  }
}

void addSupportedParameter(Parameters& map,
    const ParameterTypes& supported_types,
    uintmax_t position,
    const optional<DataVariant>& parameter,
    bool strict_assign) {
  auto it = supported_types.find(position);
  if (it == supported_types.end()) {
    throw ParameterDoesNotExist(position);
  }

  checkParameter(position, parameter, it->second);

  if (strict_assign) {
    map.insert_or_assign(position, parameter);
  } else {
    map.try_emplace(position, parameter);
  }
}

void checkParameters(
    const Parameters& input_parameters, const ParameterTypes& supported_types) {
  for (const auto& [pos, expected] : supported_types) {

    auto it = input_parameters.find(pos);
    if (it == input_parameters.end() && expected.mandatory) {
      throw MandatoryParameterMissing(pos, expected.type);
    }

    if (it != input_parameters.end()) {
      checkParameter(pos, it->second, expected);
    }
  }
}

string toString(const ParameterTypes& supported_types) {
  if (supported_types.empty()) {
    return "{}";
  }
  string result = "{";
  for (const auto& [position, parameter] : supported_types) {
    result += "{" + to_string(position) + "," + toString(parameter.type) +
        (parameter.mandatory ? "mandatory" : "optional") + "},";
  }
  result.pop_back(); // pop last , character
  return result += "}";
}

string toString(const Parameters& parameters) {
  if (parameters.empty()) {
    return "{}";
  }
  string result = "{";
  for (const auto& [position, parameter] : parameters) {
    result += "{" + to_string(position) + "," +
        toString(parameter.value_or("NullOpt")) + "},";
  }
  result.pop_back(); // pop last , character
  return result += "}";
}

} // namespace Information_Model
