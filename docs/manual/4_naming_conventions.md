# Naming Conventions

## Element Reference ID

Every `Device` has a unique reference ID that is used as a base for every `Element` instance of that device. This way the user can see which `Element` instance is linked to which `Device`. 

The base reference ID is separated from the rest of the Element reference id with a `:` character to denote the end of the base id. For example: 

```
    Device_Ref_ID1:[DEVICE_ELEMENT_ID]
```

Each Element reference ID also lists the entire path of the base Group instance. Each grouping level is separated with a `.` character. For example: 

Let’s say that we have a Device, with reference ID `Example`, with one `Element`, with reference ID `element_1`, and one `Group` with reference ID `element_2`.  This group then has one  `Element`, with reference ID `sub_element_1`, and another Group, with reference ID `sub_element_2`, which finally has one Element, with reference ID `sub_sub_element_1`. We could depict this device as follows: 

| Example Device ID Structure |
| :---- |
| @image html images/example_device_ref_ids.png width=300px | 

The resulting reference IDs would be:
```
    Example: element_1
    Example: element_2
    Example: element_2.sub_element_1
    Example: element_2.sub_element_2
    Example: element_2.sub_element_2.sub_sub_element_1
```

## Element Naming Convention 

Each `Element` instance is named after the functionality it provides, for example, if we have a Group that organizes electric power measurements, we would name that group Power Measurements, or simply Power, and each power measurement within said group Phase 1 Power, Phase 2 Power, etc… 
Each `Group`, `Readable`, `Writable` and `Observable` SHOULD be a noun and SHOULD NOT include the word `Element` in the name. 
Each `Callable` SHOULD be a verb or at least to start with a verb and SHOULD NOT include the word `Callable` in the name. 