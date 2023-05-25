
# Class Diagrams
The entire Information Model consists of seven interfaces, one concrete class, two enumerations and one variant. A simplified class diagram is shown below. 

| Information Model Class Diagram |
| :---- |
| @image html images/Simplified_Information_Model.png width=800px | 

And the full class diagram is available bellow:

<details>
<summary>Full Information Model Class Diagram</summary>
@image html images/Full_Information_Model.png width=1400px
</details>

## NamedElement
NamedElement (marked as element 1. NamedElement in the diagram above) is the base class for every single interface within the STAG Information Model, it contains static Meta information, like the human readable name of the element, element’s reference ID and its description.

## Device
Device (marked as element 2. Device in the diagram above) is the sensor/actor system’s abstraction and groups all of the systems features and capabilities under one namespace. It is the primary interface for Data Consumer Adapter Implementations [3] to interact with and is the main system’s abstraction that Technology Adapter Implementations [1] create.

## DeviceElement
DeviceElement (marked as element 3. DeviceElement in the diagram above) is an abstraction of a single sensor/actor system’s feature, for example the measured temperature value of a thermistor. It provides the user with a common interface for various element types described in the ElementType (marked as element 4. ElementType in the diagram above) enumeration.
## DeviceElementGroup
DeviceElementGroup (marked as element 5. DeviceElementGroup in the diagram above) is the first of five possible DeviceElement interface types. This interface groups other DeviceElement instances under its namespace and allows the user to logically structure various sensor/actor system’s features and capabilities.

## Metric
Metric (marked as element 6. Metric in the diagram above) is the second of five possible DeviceElement interface types. This interface models a single read only capability of the sensor/actor system. For example, the previously mentioned measured temperature value, would be modeled as a Metric. Metric instances only allow to model a single DataType. Returning, from the internal callback function, a different DataType than the one that is modeled will result in an exception being thrown to the value setter.

## WritableMetric
WritableMetric (marked as element 10. WritableMetric in the diagram above) is the third of five possible DeviceElement interface types. This interface models read and write or write only capability of the sensor/actor system. For example, we could model sensor labels as a WritableMetric and allow users to read and write the label. Similar to Metric instances WritableMetrics only allow to model a single DataType.

## ObservableMetric
ObservableMetric (marked as element 11. ObservableMetric in the diagram above) is the fourth of five possible DeviceElement interface types. This interface models a sensor/actor system’s capability to notify its listeners when a data change occurs. This interface is not fully defined as of v0.3.0 specification and is subject to change. Similar to Metric instances ObservableMetric instances only allow to model a single DataType.

## Function
Function (marked as element 7. Function in the diagram above) is the fifth of five possible DeviceElement interface types. This interface models a sensor/actor system’s capability to execute a given functionality. For example, the ability to reset error codes, average value calculations, etc. Unlike Metric instances, Functions allow for multiple optional/mandatory parameter types, indexed with a parameter number (to avoid having to use parameter names, but still allow for multiple identical DataType Parameters) as well as returning a single DataType value. Modeled Function instances contain an indexed map of supported DataType instances to check if provided Parameters are supported by the modeled Function.

## DataVariant
<table cellspacing="0" cellpadding="0" style="border-collapse: collapse; border: none;">
<tr>
<td style="vertical-align:top;">
DataVariant (marked as element 9. DataVariant in the diagram above) is a heterogeneous data type container. It provides users a convenient way of obtaining Metric type values or interacting with Function parameters or results. The complete list of modeled data values is shown in the diagram bellow.

Each data type is mapped to C++ types as follows:
1.	DataVariant – [std::variant<class…Types>](https://en.cppreference.com/w/cpp/utility/variant) type
2.	Boolean – [bool](https://en.cppreference.com/w/cpp/keyword/bool) type
3.	Integer – [max fixed width signed integer](https://en.cppreference.com/w/cpp/types/integer) type
4.	Unsigned_Integer – [max fixed width unsigned integer](https://en.cppreference.com/w/cpp/types/integer) type
5.	Double – [double precision floating](https://en.cppreference.com/w/cpp/keyword/double) type
6.	String – [std::string](https://en.cppreference.com/w/cpp/string/basic_string) type
7.	Opaque – [std::vector<uint8_t>](https://en.cppreference.com/w/cpp/container/vector) type
8.	Time – Custom struct to [std::time_t](https://en.cppreference.com/w/cpp/chrono/c/time_t) posix time type
</td>
<td>
| DataVariant Class Diagram |
| :---- |
| @image html images/Data_Variant_Class_Diagram.png width=800px | 
</td>
</tr>
</table>