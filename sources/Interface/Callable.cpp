#include "Callable.hpp"

#include <numeric>

namespace Information_Model {
using namespace std;

ResultFuture::ResultFuture(uintmax_t caller,
    future<DataVariant>&& future_result,
    const ResultFuture::CallClearer& clearer)
    : call_id_(caller), future_result_(move(future_result)),
      clear_caller_(clearer) {}

DataVariant ResultFuture::get() {
  auto result = future_result_.get();
  clear_caller_(call_id_);
  return result;
}

uintmax_t ResultFuture::callerID() const { return call_id_; }

void addParameter(Callable::Parameters& map,
    uintmax_t param_number,
    const Callable::Parameter& param) {
  if (!map.try_emplace(param_number, param).second) {
    auto existing = map[param_number].value_or(DataVariant());
    auto param_value = param.value_or(DataVariant());
    string error_msg = "Could not add " + toString(toDataType(param_value)) +
        "(" + toString(param_value) + ")" + " as parameter number " +
        to_string(param_number) + " because parameter " +
        toString(toDataType(existing)) + "(" + toString(existing) +
        ") is already assigned to that number";
    throw range_error(error_msg);
  }
}

void addSupportedParameter(Callable::Parameters& map,
    const Callable::ParameterTypes& supported_types,
    uintmax_t param_number,
    const Callable::Parameter& param) {
  if (auto it = supported_types.find(param_number);
      it != supported_types.end()) {
    auto [type, optional] = it->second;
    if (param) {
      const auto& param_value = param.value();
      if (matchVariantType(param_value, type)) {
        addParameter(map, param_number, param);
      } else {
        string error_msg = "Parameter number " + to_string(param_number) + " " +
            toString(type) + " does not support " +
            toString(toDataType(param_value)) + "(" + toString(param_value) +
            ") value";
        throw invalid_argument(error_msg);
      }
    } else if (optional) {
      addParameter(map, param_number, nullopt);
    } else {
      string error_msg = "Parameter number " + to_string(param_number) + " " +
          toString(type) + " does not support empty values";
      throw invalid_argument(error_msg);
    }
  } else {
    string error_msg =
        "Parameter number " + to_string(param_number) + " is not supported";
    throw invalid_argument(error_msg);
  }
}

string toString(Callable::ParameterTypes params) {
  return accumulate(next(params.begin()),
      params.end(),
      toString(params[0].first),
      [](string& result, pair<uintmax_t, Callable::ParameterType> parameter) {
        return result + ", " + toString(parameter.second.first);
      });
}

} // namespace Information_Model
