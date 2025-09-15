# Device Composition

<table cellspacing="0" cellpadding="0" style="border-collapse: collapse; border: none;">
<tr>
<td style="vertical-align:top;width:70%">

To better understand how a Device Abstraction looks like, let us look over Example Device Abstraction, shown on the right side. 

In this figure, we see that each Device organizes multiple Element instances, made up of Metric, Callable or Group entities. 

Furthermore, each Group contains other Element instances, which in turn can be Metric, Callable or even other Group entities as well. 

This nesting approach allows *Technology Adapter Implementations* to logically structure various parts of sensor/actor system abstraction and hide more complex parts within dedicated Group instances. 

This approach is also reflected in how Element reference ID are generated.

</td>
<td style="vertical-align:top;width:20%">
| Example Device Abstraction |
| :---- |
| @image html images/Device_Element_Composition.png | 
</td>
</tr>
</table>