BriSe - DIY Ambient Light Sensor
================================

BriSe is a simple brightness sensor built on the base of a Arduino Leonardo
compatible board in simple conditions. It can be accessed via USB Serial
protocol (ACM class) and it uses a very simple ASCII text line based protocol
to print results to user.


Hardware Building
-----------------

See Github Wiki for the construction guide. Connect via Mini-USB.


Sofware Building
----------------

```
apt install arduino
```

Load the tempt\_reader.ino file in the arduino IDE and compile and flash to target.


Sample usage
------------

On Linux:

`
$ screen /dev/ttyACM0
`

On Windows:

Use Putty, select Serial type, use the right port, configure baud rate of 9600 with Software flow control.

Sample output:
```
Smooth: 19;
Raw: 20;
Smooth: 18;
Raw: 16;
Smooth: 19;
Raw: 20;
Smooth: 20;
Raw: 20;
Smooth: 19;
Raw: 16;
```

How it works
------------

The brightness values are collected at a semi-fixed polling rate from the sensor but the forwarding to the serial channel is filtered.
The low-pass filter uses a sample size of 32 and integer based value rounding,
limiting the push of new calculated sample only when the value differs by at
least one, in additional roughly every fifth second even without value changes.



Known Bugs and Limitations
--------------------------

- The sensor readings apparently receive a boost of 5-10 when the serial channel is highly active. It's not clear yet whether this can be attributed to power supply changes through IO pins, to reading interval changes (having some impact on the sensor operation) or electromagnetic influence.
- The RX/TX pins are permanently active and should be turned off after some seconds.
- Makefile or other script for headless building might be good
