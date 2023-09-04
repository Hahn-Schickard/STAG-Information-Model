# Handling Exceptions

Calls to some `Metric`, `WritableMetric`, `ObservableMetric` and `Function` methods could encounter an error or an exception during system operation. These methods are marked in documentation with `@throws` tag. 

In this section we will cover how **Data Consumer Adapter Interface** implementations should handle these exceptions and when **Technology Adapter Interface** implementations should throw them.

## Catching Exceptions

All **Data Consumer Adapter Interface** implementations *MUST*  wrap every method that *MAY* throw an exception in a `try-catch` block in one of the following ways:

### Handling all thrown exceptions in a generic way
```cpp
try{
    methodThatMayThrow();
}catch(const std::exception& ex){
    // handle any exception type
}
```

This is the standard C++ mechanism to handle any exception that is based on [`std::exception`](https://en.cppreference.com/w/cpp/error/exception). All of the Information Model exception types are based on it. Capturing **const** reference preserves the underlying exception information while providing a convenient way of obtaining human-readable explanation for the thrown exception via the [`what()`](https://en.cppreference.com/w/cpp/error/exception/what) method. 

### Handling specific thrown exceptions types
```cpp
try{
    methodThatMayThrow();
}catch(const std::invalid_argument& ex){
    // handle invalid_argument exception 
}catch(const std::runtime_error& ex){
    // handle runtime_error exception 
}catch(const std::exception& ex){
    // fallback for all other exceptions 
}
```

Users can also differentiate between various different exceptions, in case different exceptions need to be treated differentially. This is done by declaring multiple `catch` blocks after the initial `try` block. Each new `catch` block then declares what exception type it will catch and how said block is going to use the exception. It is recommended to always have a single `catch(const std::exception& ex)` block at the end. This way, any thrown exception, that was not caught by the specialized catch blocks, will still be properly handled and will not terminate system operation.

### Suppress all thrown exceptions
```cpp
try{
    methodThatMayThrow();
}catch(...){
    // suppress all thrown exceptions 
}
```

Users can also ignore any thrown exception by using the `...` [ellipsis](https://en.cppreference.com/w/cpp/language/try_catch) as a place holder for any thrown data-type. This is not recommended, since it is always a good idea to at-least log the thrown exception to allow for problem solving.

## Throwing Exceptions

All **Technology Adapter Interface** implementations are expected to throw a [`std::runtime_error`](https://en.cppreference.com/w/cpp/error/runtime_error) or any of it's subclasses in case the requested operation could not be carried out or to completely remove the `Device` instance by deregistering it. 

### To throw, to wait, or to deregister?

It is difficult to say, when **Technology Adapter Interface** implementation should throw an exception, wait for an error correction or completely remove the `Device` instance, without knowing what the specific technology is being implementing. Some technologies have really detailed explanations of expected failure cases and how each case should be handled, others do not. 

In cases when there are no failure handling mechanism specifications, developers are expected to: 

* **Wait** until a standard expected operation execution time has elapsed (in most cases it is 1 second, however it may be longer) or a predefined number of retries has been executed (in most cases, 3 times, but may be larger number, if the operation requires it), and then throw a `std::runtime_error` stating that  this specific operation has timed out.
* **Throw** a `std::runtime_error` or any of it's subclasses (including custom exception types that inherit from it) with a clear and understandable exception description, when there is no way that the operation can be executed at the current time, but is expected to be operational at some future time. 
* **Deregister** the `Device` instance if there is no way that the operation can be executed. Technology implementations *MUST* **throw** a `std::runtime_error` with a clear and understandable exception description stating that this operation caused the `Device` instance to deregister for every single active operation of said `Device`. This way users will not get stuck waiting for a response. If it is impractical to throw such a specific exception for all active operations, implementers *MAY* throw a more generalized exception, stating that the `Device` is being deregistrated due to encountering and unrecoverable error during an execution of one of it's operations. However, the offending operation must always throw an exception stating, that said operation caused the `Device` instance to deregister. This is to allow users to track down, which operation caused the malfunction. Implementations *MAY* reregister the `Device` instance without the capability to execute the failing operation (without modelling the element with the offending operation). 