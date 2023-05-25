# Information Model

## Description
The STAG *Information model* is a collection of various classes used to model sensor/actor systems abstractions. It functions as a base modelling language for all STAG components and is the primary way of interacting with the STAG Project. 
Each *Technology Adapter Implementation* must build sensor/actor system abstractions with this model via the *Information Builder Interface* and register resulting *Device* instances via the *Model Registry Interface* to make the *Device* instances available for every registered *Data Consumer Adapter Implementation*.

## Class Diagram
The entire Information Model consists of seven interfaces, one concrete class, two enumerations and one variant. A simplified class diagram is shown below. 

| Information Model Class Diagram |
| :---- |
| @image html images/Simplified_Information_Model.png width=800px | 

And the full class diagram is available bellow:

<details>
<summary>Full Information Model Class Diagram</summary>
@image html images/Full_Information_Model.png width=1600px
</details>

### NamedElement
NamedElement (marked as element 1. NamedElement in the diagram above) is the base class for every single interface within the STAG Information Model, it contains static Meta information, like the human readable name of the element, element’s reference ID and its description.

### Device
Device (marked as element 2. Device in the diagram above) is the sensor/actor system’s abstraction and groups all of the systems features and capabilities under one namespace. It is the primary interface for Data Consumer Adapter Implementations [3] to interact with and is the main system’s abstraction that Technology Adapter Implementations [1] create.

### DeviceElement
DeviceElement (marked as element 3. DeviceElement in the diagram above) is an abstraction of a single sensor/actor system’s feature, for example the measured temperature value of a thermistor. It provides the user with a common interface for various element types described in the ElementType (marked as element 4. ElementType in the diagram above) enumeration.
### DeviceElementGroup
DeviceElementGroup (marked as element 5. DeviceElementGroup in the diagram above) is the first of five possible DeviceElement interface types. This interface groups other DeviceElement instances under its namespace and allows the user to logically structure various sensor/actor system’s features and capabilities.

### Metric
Metric (marked as element 6. Metric in the diagram above) is the second of five possible DeviceElement interface types. This interface models a single read only capability of the sensor/actor system. For example, the previously mentioned measured temperature value, would be modeled as a Metric. Metric instances only allow to model a single DataType. Returning, from the internal callback function, a different DataType than the one that is modeled will result in an exception being thrown to the value setter.

### WritableMetric
WritableMetric (marked as element 10. WritableMetric in the diagram above) is the third of five possible DeviceElement interface types. This interface models read and write or write only capability of the sensor/actor system. For example, we could model sensor labels as a WritableMetric and allow users to read and write the label. Similar to Metric instances WritableMetrics only allow to model a single DataType.

### ObservableMetric
ObservableMetric (marked as element 11. ObservableMetric in the diagram above) is the fourth of five possible DeviceElement interface types. This interface models a sensor/actor system’s capability to notify its listeners when a data change occurs. This interface is not fully defined as of v0.3.0 specification and is subject to change. Similar to Metric instances ObservableMetric instances only allow to model a single DataType.

### Function
Function (marked as element 7. Function in the diagram above) is the fifth of five possible DeviceElement interface types. This interface models a sensor/actor system’s capability to execute a given functionality. For example, the ability to reset error codes, average value calculations, etc. Unlike Metric instances, Functions allow for multiple optional/mandatory parameter types, indexed with a parameter number (to avoid having to use parameter names, but still allow for multiple identical DataType Parameters) as well as returning a single DataType value. Modeled Function instances contain an indexed map of supported DataType instances to check if provided Parameters are supported by the modeled Function.

### DataVariant
DataVariant (marked as element 9. DataVariant in the diagram above) is a heterogeneous data type container. It provides users a convenient way of obtaining Metric type values or interacting with Function parameters or results. The complete list of modeled data values is shown in the diagram bellow.

<div class="row">
  <div class="col-md-8" markdown="1">
Each data type is mapped to C++ types as follows:
1.	DataVariant – [std::variant<class…Types>](https://en.cppreference.com/w/cpp/utility/variant) type
2.	Boolean – [bool](https://en.cppreference.com/w/cpp/keyword/bool) type
3.	Integer – [max fixed width signed integer](https://en.cppreference.com/w/cpp/types/integer) type
4.	Unsigned_Integer – [max fixed width unsigned integer](https://en.cppreference.com/w/cpp/types/integer) type
5.	Double – [double precision floating](https://en.cppreference.com/w/cpp/keyword/double) type
6.	String – [std::string](https://en.cppreference.com/w/cpp/string/basic_string) type
7.	Opaque – [std::vector<uint8_t>](https://en.cppreference.com/w/cpp/container/vector) type
8.	Time – Custom struct to [std::time_t](https://en.cppreference.com/w/cpp/chrono/c/time_t) posix time type
  </div>
  <div class="col-md-4" markdown="1">
| DataVariant Class Diagram |
| :---- |
| @image html images/Data_Variant_Class_Diagram.png width=800px | 
 </div>
</div>

## Device Composition

To better understand how a Device Abstraction looks like, let us look over Example Device Abstraction, shown bellow. 
<div class="row">
  <div class="col-md-8" markdown="1">
In this figure, we see that each Device organizes multiple DeviceElement instances, made up of Metric, Function or DeviceElementGroup entities. Furthermore, each DeviceElementGroup contains other DeviceElement instances, which in turn can be Metric, Function or even other DeviceElementGroup entities as well. 
This nesting approach allows * Technology Adapter Implementations* to logically structure various parts of sensor/actor system abstraction and hide more complex parts within dedicated DeviceElementGroup instances. 
Finally, this approach is reflected in how DeviceElement reference ID are generated.
  </div>
  <div class="col-md-4" markdown="1">
| Example Device Abstraction |
| :---- |
| @image html images/Device_Element_Composition.png width=300px | 
 </div>
</div>

##	NamedElement Reference ID 

Every Device has a unique reference ID that is used as a base for every DeviceElement instance of that device. This way the user can see which DeviceElement instance is linked to which Device. 

The base reference ID is separated from the rest of the DeviceElement reference id with a `:` character to denote the end of the base id. For example: 

```
    Device_Ref_ID1:[DEVICE_ELEMENT_ID]
```

Each DeviceElement reference ID also lists the entire path of the base DeviceElementGroup instance. Each grouping level is separated with a `.` character. For example: 

Let’s say that we have a Device, with reference ID `Example`, with one DeviceElement, with reference ID `element_1`, and one DeviceElementGroup with reference ID `element_2`.  This group then has one  DeviceElement, with reference ID `sub_element_1`, and another DeviceElementGroup, with reference ID `sub_element_2`, which finally has one DeviceElement, with reference ID `sub_sub_element_1`. We could depict this device as follows: 

| Example Device ID Structure |
| :---- |
| @image html images/example_device_ref_ids.png width=200px | 

The resulting reference IDs would be:
```
    Example: element_1
    Example: element_2
    Example: element_2. sub_element_1
    Example: element_2. sub_element_2
    Example: element_2. sub_element_2. sub_sub_element_1
```

## DeviceElement Naming Convention
Each DeviceElement instance is named after the functionality it provides, for example, if we have a DeviceElementGroup that organizes electric power measurements, we would name that group Power Measurements, or simply Power, and each power measurement within said group Phase 1 Power, Phase 2 Power, etc… 
Each DeviceElementGroup, Metric, WritableMetric and ObservableMetric SHOULD be a noun and SHOULD NOT include the word DeviceElement in the name. 
Each Function SHOULD be a verb or at least to start with a verb and SHOULD NOT include the word Function in the name. 
