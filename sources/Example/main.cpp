#include "DataVariant.hpp"

#include <Variant_Visitor/Visitor.hpp>

#include <functional>
#include <iostream>

using namespace std;
using namespace Information_Model;

// NOLINTBEGIN(readability-magic-numbers,bugprone-*)
int main() {
  // You can set default variant value, based on given a DataType
  auto variant_value =
      setVariant(DataType::Boolean)
          .value(); // dont forget to get the value, since the result is
                    // std::optional<DataVariant>, not DataVariant

  // You can check the internal variant value size
  cout << "Given variant size is: " << size_of(variant_value) << " bytes"
       << endl;

  // You can check if a given variant holds a certain DataType
  if (!matchVariantType(variant_value, DataType::Integer)) {
    cout << "Given variant value is not an integer" << endl;
  } else {
    try {
      // If you know the underlying data type, you can use the std::get to
      // access it
      auto uint_value = get<uintmax_t>(variant_value);
      cout << uint_value << endl;
    } catch (const bad_variant_access&) {
      // But if you are wrong, you will get a bad_variant_access exception
      cerr << "Opps, variant value is not a integer" << endl;
    }
  }

  // You can get the exact DataType of the stored variant value
  auto data_type = toDataType(variant_value);

  // You can convert any DataType into a human readable string
  cout << "Stored data variant is of " << toString(data_type) << " type"
       << endl;

  // You can remove all of the whitespaces from the human readable string
  cout << "Stored data variant is of " << toSanitizedString(data_type)
       << " type (sanitized)" << endl;

  // A safer way to process the DataVariant value is to use data type pattern
  // matching with Variant_Visitor library, just make sure all of the
  // possibilities are covered in your lambdas
  Variant_Visitor::match(
      variant_value,
      [](bool value) { cout << "matched to a boolean " << value << endl; },
      [](uintmax_t value) { cout << "matched to an uint " << value << endl; },
      [](intmax_t value) { cout << "matched to an int " << value << endl; },
      [](double value) { cout << "matched to a double " << value << endl; },
      [](const Timestamp& value) {
        // you can convert a time stamp into a custom formatted string by using
        // strftime flags
        cout << "matched to a timestamp" << toString(value, "%Y-%M-%D") << endl;
      },
      [](const vector<uint8_t>&) {
        // you can suppress capture value, if all you need to do is match for
        // specific type
        cout << "matched to a byte vector" << endl;
      },
      [](const string& value) {
        cout << "matched to a string " << value << endl;
      });

  // If you only need to cover certain data types, you can use auto keyword as a
  // suppressor or a general matcher
  Variant_Visitor::match(
      variant_value,
      [](const auto&) { cout << "matched to an integer like" << endl; },
      [](const Timestamp&) { cout << "matched to a timestamp" << endl; },
      [](const vector<uint8_t>&) {
        cout << "matched to a byte vector" << endl;
      },
      // You can also capture the string as a string_view if you only want to
      // read the data
      [](string_view value) {
        cout << "matched as a string view " << value << endl;
      });

  // You can convert the variant value into a human readable string
  cout << "Stored variant value as string is: " << toString(variant_value)
       << endl;

  // You can convert the variant value into an alphanumeric string
  cout << "Stored variant value as a sanitized string is: "
       << toSanitizedString(variant_value) << endl;

  // You can generate a timestamp of a current value
  auto current_time = makeTimestamp();

  // You can convert a given Timestamp into an ISO 8601 string
  cout << "Current system time is " << toString(current_time) << endl;

  // You can manually pass a time_point value, though it must come from
  // system_clock
  auto current_timepoint = chrono::system_clock::now();
  auto as_timestamp = toTimestamp(current_timepoint);

  // You can also convert a given Timestamp into system_clock time point
  auto as_time_point = toTimepoint(as_timestamp);
  if (chrono::time_point_cast<chrono::seconds>(current_timepoint) !=
      chrono::time_point_cast<chrono::seconds>(as_time_point)) {
    // But don't expect the timepoints to match on subsecond level
    cerr << "Current timepoint " << current_timepoint.time_since_epoch().count()
         << "does not match converted "
         << as_time_point.time_since_epoch().count() << " time point" << endl;
    cerr << "This should not happen" << endl;
  }

  // You need to be careful when manually creating timestamps
  auto bad_timestamp = Timestamp{.year = 0,
      .month = 25,
      .day = 100,
      .hours = 86,
      .minutes = 255,
      .seconds = 92,
      .microseconds = 100};

  try {
    // You can check if a given timestamp is correct
    verifyTimestamp(bad_timestamp);
  } catch (const invalid_argument& ex) {
    // thought it only throws an error message for the first offending field,
    // not all of them
    cout << "Bad time format: " << ex.what() << endl;
  }

  return 0;
}
// NOLINTEND(readability-magic-numbers,bugprone-*)