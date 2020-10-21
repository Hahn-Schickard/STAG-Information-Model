This module provides a common set of classes for both *Data Consumer Adapter* and *Technolgoy Adapter* Implementations to use for modelling purposes. 

## Testing interface
Testing Interface is provided via [GTest](https://github.com/google/googletest) framework.

### Model Class Mocks
An extensive set of Mock implementations is included under Information_Model::testing namespace for the use of various *Data Consumer Adapter* and *Technolgoy Adapter* Implementations to use in their unit and integration test stages. 

### Data Consumer Adapter Implementation testing
*Data Consumer Adapter Implementations* should use `DeviceMockBuilder` class defined in **DeviceMockBuilder.hpp** to create a `DeviceMock` and use that to simulate interactions with `DevicePtr`. 

### Technolgoy Adapter Implementation testing
*Technolgoy Adapter Implementations* should use `DeviceBuilderInterfaceMock` class defined in **DeviceBuilderInterface_MOCK.hpp** to simulate device building.