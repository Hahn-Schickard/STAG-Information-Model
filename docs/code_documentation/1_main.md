# Information Model {#mainpage}

The STAG *Information model* is a collection of various classes used to model sensor/actor systems abstractions. It functions as a base modelling language for all STAG components and is the primary way of interacting with the STAG Project. 
Each *Technology Adapter Implementation* must build sensor/actor system abstractions with this model via the *Information Builder Interface* and register resulting *Device* instances via the *Model Registry Interface* to make the *Device* instances available for every registered *Data Consumer Adapter Implementation*.
