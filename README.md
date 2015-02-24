# TeensyButtoner
Polls Input Pins For Teensy 2.0 In THE RASPBERRY GEAR!

Hi there!

This cool sketch allows the Teensy to do a couple things for your Raspberry Gear.
First and foremost, it polls the state of the button inputs and sends (your choice at compile time) either key press events, or joystick events.
Secondly, it determines if headphones are plugged in, and if so disables the MAX98306 amplifier.

<p> You can switch the types of events it sends by switching the <i>#define OUTPUT_MODE</i> at the top. There are two definitions, <i>MODE_KEYBOARD</i>
and <i>MODE_JOY</i>. You can also easily change the pin numbers in the collection of const ints at the top, if need be. </p>

<p> There is an array of booleans called <i>is_inverted</i> -- This lookup table determines how the Teensy determines if a button's state is
active. The RaspberryGear mixes OFF - HIGH & ON - LOW and OFF - LOW & ON - HIGH devices. (The normal button pads vs. the capacitive sensor pads, respectively.)
Instead of adding another component to standardize the logic, this lookup table serves that purpose. So -- false in this table means that 
upon startup, the pin has it's INPUT_PULLUP resistor enabled. Then, when the pin is determined to be LOW, it is considered on and vice versa. True in this
table means that there is NO INPUT_PULLUP enabled (ever) and when this pin is determined to be HIGH, it is considered on, and vice versa.</p>

<p> Important to mention that the original code for the Teensy was written by driverblock on the Adafruit GitHub. I added my own features to this already existing sketch!!
<a href="https://github.com/adafruit/USBgamepad/tree/master/teensySNES_test1/">USB Gamepad</a> </p>
