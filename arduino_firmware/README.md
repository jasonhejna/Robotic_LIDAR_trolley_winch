# Getting Started
* main.ino contains the firmware.
* You will need the Arduino IDE running on your computer.
* Compile main.ino into hex using arduino. You will need to copy the file from the arduino folder on your computer.
* To find the file, turn on verbose mode in the Arduino IDE.

### Flash Arduino
```
avrdude -v -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -D -U flash:w:Blink.ino.hex:i
```
