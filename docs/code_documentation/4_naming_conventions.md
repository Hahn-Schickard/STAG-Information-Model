# Naming Conventions

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