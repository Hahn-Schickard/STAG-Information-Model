# Device Composition

To better understand how a Device Abstraction looks like, let us look over Example Device Abstraction, shown bellow. 

In this figure, we see that each Device organizes multiple DeviceElement instances, made up of Metric, Function or DeviceElementGroup entities. Furthermore, each DeviceElementGroup contains other DeviceElement instances, which in turn can be Metric, Function or even other DeviceElementGroup entities as well. 
This nesting approach allows *Technology Adapter Implementations* to logically structure various parts of sensor/actor system abstraction and hide more complex parts within dedicated DeviceElementGroup instances. 
Finally, this approach is reflected in how DeviceElement reference ID are generated.

| Example Device Abstraction |
| :---- |
| @image html images/Device_Element_Composition.png width=300px | 
