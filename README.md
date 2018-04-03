# Robotic_LIDAR_trolley_winch
A wifi controlled i-beam trolley and winch.

# Raspberry Pi Setup
```
sudo apt-get install python-serial
Sudo apt-get install avrdude
```

# Useful Commands
```
avrdude -v -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -D -U flash:w:Blink.ino.hex:i
```