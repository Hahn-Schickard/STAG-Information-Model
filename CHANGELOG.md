# Changelog
## [0.3.1] - 2023.07.10
### Changed
 - conan recipe to use conan v2 syntax
 - CMake requirement to 3.24
 - conan cmake integration to use conan v2 engine

## [0.3.0] - 2023.06.26
### Added 
 - UML Class diagram documentation
 - Device composition documentation
 - Naming conventions
 - `DataType::NONE`
 - `DataType toDataType(DataVariant)` function
 - `bool matchVariantType(const DataVariant&, DataType)` function
 - `NamedElement::operator==(const NamedElement&)` method
 - `NamedElement::operator!=(const NamedElement&)` method
 - `Device` class documentation
 - `Device::getDeviceElementGroup()` method documentation
 - `Device::getDeviceElement()` method documentation
 - `DeviceElementNotFound` exception
 - `bool Device::operator==(const Device&)` method
 - `bool Device::operator!=(const Device&)` method
 - `NonemptyFunctionPtr` to `DeviceElement::SpecificInterface` field
 - `ElementType DeviceElement ::getElementType()` method
 - `bool operator==(const DeviceElement&, const DeviceElement&)` function
 - `bool operator!=(const DeviceElement&, const DeviceElement&)` function
 - `ResultReturningNotSupported` exception 
 - `CallerNotFound` exception
 - `CallerIDExists` exception
 - `CallCanceled` exception
 - `FunctionCallTimedout` exception
 - `Function` class
 - `Function::ResultFuture` class
 - `void addParameter(Function::Parameters&, uintmax_t, const Function::Parameter&)` function
 - `void addSupportedParameter(Function::Parameters&, const Function::ParameterTypes&, uintmax_t, const Function::Parameter&)` function 
 - `std::string toString(Function::ParameterTypes)` function
 - `DeviceBuilderInterface::Writer` callable alias
 - `DeviceBuilderInterface::Reader` callable alias
 - `DeviceBuilderInterface::Executor` callable alias
 - `DeviceBuilderInterface::Canceler` callable alias
 - `DeviceBuilderInterface::Functionality` class
 - `Metric::value_type_` private field
 - `WritableMetric::isWriteOnly()` method
 - `Nonempty*` alias for modeled elements

### Changed
 - `DateTime::size()` method to const
 - `Device::getDeviceElement()` return type to `NonemptyDeviceElementPtr`
 - `Device::getDeviceElement()` to throw `DeviceElementNotFound`
 - `DeviceElementGroup::getDeviceElement()` return type to `NonemptyDeviceElementPtr`
 - `DeviceElementGroup::getDeviceElement()` to throw `DeviceElementNotFound`
 - `DeviceElement::specific_interface` into `DeviceElement::functionality` 
 - `DeviceElement` ctor to only be accessible by `DeviceBuilderInterface`
 - `DeviceBuilderInterface::getResult()` method to return `UniqueDevicePtr`
 - `DeviceBuilderInterface::add*(const std::string& name, ...)` methods be non virtual and to call appropriate virtual method
 - `DeviceBuilderInterface::addFunction()` method
 - `DeviceBuilderInterface::makeDeviceElement()` method to be protected
 - `ElementType` enum to be declared in **DeviceElement.hpp** instead of **DeviceBuilderInterface.hpp**
 - `Metric::getDataType()` method to no longer be virtual
 - `WritableMetric` to inherit from `Metric` 
 - `DeviceBuilderInterfaceMock` into `DeviceBuilderMock`
 
### Removed
 - `DeviceElement()` ctor
 - `Metric()` ctor
 - `ReadFunctor` callable alias
 - `WriteFunctor` callable alias
 - `ExecuteFunctor` callable alias 

## [0.2.3] - 2023.07.04
### Changed 
 - Nonempty_Pointer dependency to fuzzy v0.2
 - `DeviceBuilderInterfaceMock::addDeviceElement()` method mock to use correct number of parameters

## [0.2.2] - 2022.11.17
### Changed
 - conan packaging recipe
 - gtest dependency to fuzzy v1.11
 - Variant_Visitor dependency to fuzzy v0.1
 - Nonempty_Pointer dependency to fuzzy v0.1

### Removed
 - `DeviceBuilderInterface::addDeviceElement()` parameter names, to avoid -Wunused_param warnings

### Added
 - exception handling mechanism for main example
 - exception handling mechanism for integration test
 - missing override specifiers for `DeviceMockBuilder`

## [0.2.1] - 2022.10.05
### Changed 
 - Nonempty_Pointer dependency to v0.1.2
 - `toString(DataVariant)` implementation to reduce code repetition
 - `size_of(DataVariant)` implementation to reduce code repetition
 - DeviceBuilderInterface.hpp unit documentation
 - `DeviceBuilderInterface::addDeviceElement()` default argument values

## [0.2.0] - 2022.02.02
### Added
 - Dependency to project Nonempty_Pointer
 - type `NonemptyNamedElementPtr`
 - type `NonemptyDevicePtr`
 - type `NonemptyDeviceElementPtr`
 - type `NonemptyDeviceElementGroupPtr`
 - type `NonemptyMetricPtr`
 - type `NonemptyWritableMetricPtr`

### Changed
 - `DeviceElement` inheritance into a variant of pointers to more specific
   interfaces
 - Moved `ElementType` from DeviceElement.hpp to DeviceElementBuilder.hpp
 - `Device::getDeviceElementGroup` return type from `DeviceElementGroupPtr` to
   `NonemptyDeviceElementGroupPtr`
 - `DeviceElementGroup::DeviceElements` to contain `NonemptyPtr`
 - `MockDeviceElementGroup::DeviceElementsMap` to contain `NonemptyPtr`

### Removed
 - `DeviceElement` inheritance from `DeviceElementGroup`, `Metric`, and
   `WritableMetric`
 - cases `UNDEFINED`, `OBSERVABLE`, and `FUNCTION` from `ElementType`
 - `toString(ElementType)`
 - arguments `name` and `desc` from `MockDeviceElementGroup` constructor
 - arguments `ref_id`, `name` and `desc` from `MockMetric` constructors
 - arguments `ref_id`, `name` and `desc` from `MockWritableMetric` constructors

### Fixed
 - typo in source code

## [0.1.6] - 2021.12.14
### Changed
 - `DataVariant` `int64_t` to `intmax_t`
 - `DataVariant` `uint64_t` to `uintmax_t`
 - `DataVariant setVariant(DataType::INTEGER)` to set `(intmax_t)0` instead of `(int64_t)0`
 - `DataVariant setVariant(DataType::UNSIGNED_INTEGER)` to set `(uintmax_t)0` instead of `(uint64_t)0`
 - `MetricTests` `DataType::INTEGER` test parameter to be set to `intmax_t` instead of `int64_t`
 - `MetricTests` `DataType::UNSIGNED_INTEGER` test parameter to be set to `uintmax_t` instead of `uint64_t`
 - `WritableMetricTests::canSetIntegerValue` to set `Metric` value to `intmax_t` instead of `int64_t`
 - `Device::getDeviceElementGroup()` method from pure virtual, to virtual method with base implementation
 - `Device::getDeviceElement()` method from pure virtual, to virtual method with base implementation
 - `DeviceBuilderInterface::buildDeviceBase` method from pure virtual, to virtual method with base implementation
 - `DeviceBuilderInterface::addDeviceElementGroup` to root group method from pure virtual, to virtual method with base implementation
 - `DeviceBuilderInterface::addDeviceElementGroup` to subgroup method from pure virtual, to virtual method with base implementation
 - `DeviceBuilderInterface::addReadableMetric` to root group method from pure virtual, to virtual method with base implementation
 - `DeviceBuilderInterface::addReadableMetric` to subgroup method from pure virtual, to virtual method with base implementation
 - `DeviceBuilderInterface::addWritableMetric` to root group method from pure virtual, to virtual method with base implementation
 - `DeviceBuilderInterface::addWritableMetric` to subgroup method from pure virtual, to virtual method with base implementation
 - `DeviceBuilderInterface::addDeviceElement` method from pure virtual, to virtual method with base implementation
 - `DeviceBuilderInterface::getResult` method from pure virtual, to virtual method with base implementation
 - `DeviceElementGroup::getSubelements` method from pure virtual, to virtual method with base implementation
 - `DeviceElementGroup::getSubelement` method from pure virtual, to virtual method with base implementation
 - `Metric::getMetricValue` method from pure virtual, to virtual method with base implementation
 - `Metric::getDataType` method from pure virtual, to virtual method with base implementation
 - `WritableMetric::getMetricValue` method from pure virtual, to virtual method with base implementation
 - `WritableMetric::setMetricValue` method from pure virtual, to virtual method with base implementation
 - `WritableMetric::getDataType` method from pure virtual, to virtual method with base implementation
 - unused method parameter names to comment blocks
 - `MetricTests` to build `MockMetric` for each test from the provided `Expectations` to avoid dangling resources after global test teardown

### Added
  - `size_of(DataVariant)` function
  - `MockMetric::clearExpectations` method to allow the user to unset `ON_CALL` and `EXPECT_CALL` actions
  - stdexcept header to Device, DeviceBuilderInterface, DeviceElementGroup, Metric and WritableMetric units
  - optional header to DeviceElementGroup_MOCK

### Fixed
 - typos in Changelog
 - typos in source code

## [0.1.5] - 2021.03.29
### Changed
 - install include paths to resolve potential header name collisions
 - DeviceBuilderInterface_MOCK.hpp to use relative path in includes
 - DeviceElementGroup_MOCK.hpp to use relative path in includes
 - `DeviceElementGroup_MOCK::addReadableMetric` method to set the read callback
 - `DeviceElementGroup_MOCK::addWritableMetric` method to set the read and write callbacks
 - DeviceMockBuilder.hpp to use relative path in includes
 - `DeviceMockBuilder` to allow adding metrics without any callbacks
 - Device_MOCK.hpp to use relative path in includes
 - `MockDevice` to use `NiceMock<MockDeviceElementGroupPtr>` to suppress GTest unused mock warnings
 - Metric_MOCK.hpp to use relative path in includes
 - WritableMetric_MOCK.hpp to use relative path in includes
 - conan example.cpp to use correct inclusion paths

### Added
 - Variant Visitor dependency
 - example main.cpp runner to showcase usage
 - valgrind checks to main.cpp
 - `toString(DataVariant)` function
 - `setVariant(DataType)` function
 - `toString(ElementType)` function
 - `delegateToFake` method to `MockMetric`
 - `delegateToFake` method to `MockWritableMetric`

## [0.1.4] - 2020.11.05
### Added
 - POSIX time data type
 - `DateTime` test case
 - missing virtual destructors

### Changed
 - Metric Unit Test suit to Parametrized tests with full Data Type coverage

## [0.1.3] - 2020.10.21
### Added
 - `DeviceBuilderInterface` from Technology_Adapter_Interface
 - `DeviceBuilderInterface_MOCK`
 - `DeviceMockBuilder`
 - `NamedElementPtr` alias
 - `DeviceElementPtr` alias
 - `DeviceElementGroupPtr` alias
 - `DeviceElements` internal class alias
 - `MetricPtr` alias
 - `WritableMetricPtr` alias
 - `DevicePtr` alias
 - New mocking documentation
 - main.md
 - `DeviceMockBuilderTests`

### Changed
 - Header formatting, to better fit project standard

### Removed
 - `DeviceElement_FAKE`
 - Duplicate test in DeviceTests.cc

## [0.1.2] - 2020.10.01
### Added
 - `std::vector<uint8_t>` type to `DataVariant`
 - `uint64_t` type to `DataVariant`
 - `UNSIGNED_INTEGER` type to `DataTypeEnum`
 - `OPAQUE` type to `DataTypeEnum`

### Changed
 - conan package type to header only

### Removed
 - `uint8_t` type from `DataVariant`
 - `uint16_t` type from `DataVariant`
 - `uint32_t` type from `DataVariant`
 - `float` type from `DataVariant`
 - Variant visitor

## [0.1.1] - 2020.09.28
### Added
 - Metric Test suite
 - Writable Metric Test suite
 - old Device Test cases into Device Test suite

### Changed
 - `ElementRefID()` to `ElementId()`

## [0.1.0] - 2020.09.15
### Added
 - Changelog
 - Authors
 - License
 - Notice
 - conanfile for conan package configurator
 - `DataVariant` type and data type pattern matcher consumer.
 - Cpp CXX17 Standard requirement
 - conan package support
 - conan.cmake
 - Separate _MOCK definitions
 - conan packaging and integration testing

### Changed
 - Various test cases
 - Writable and Readable Metrics to use `DataVariant` instead of `DataWrapper`
 - Writable and Readable Metrics into interfaces
 - CMakeLists.txt to use Cmake 3.x best practices
 - Gtest runner to use *.cc extension instead of *.cpp
 - Moved Mocks and Fakes into mocks subdirectory
 - Readme to show Python 3 and conan dependencies
 - Updated gtest to 1.10
 - Documentation style
 - Contributing guide
 - reworked utilities scripts into python
 - Unit tests structure

### Removed
 - Downloading Gtest via Cmake module `ExternalProjectAdd`
 - `InformationModelExceptions`
 - `DataWrapper`
 - CPack support
 - Old test cases

## [0.0.1] - 2019.12.02
### Added
 - Draft Writable Metric template implementation
 - Writable Metric test cases and FAKE implementation
 - Draft `Metric` template implementation
 - `Metric` test cases and FAKE implementation
 - `DataWrapper` template to encapsulate used data types
 - `DataWrapper` template FAKE for gtest mocking
 - Added CPack configuration
 - Updated to Cpp Project template v0.8.27
 - Reworked raw pointers into smart pointers for the `Device` elements
 - `InformationModelExceptions`
 - `DeviceElementGroup` interface
 - `DeviceElement` interface
 - `Device` interface
 - `NamedElement`
 - Test cases for `DeviceElement` methods
 - Test cases for `DeviceElementGroup` methods
 - Test cases for `Device` methods
 - GTest runner
 - CMakeLists.txt
 - Readme

 ## [Initial] - 2019.05.20
 ### Added
  - CONTRIBUTING.md
