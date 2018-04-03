### Flash Arduino
```
avrdude -v -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -D -U flash:w:Blink.ino.hex:i
```
