/*------------------------------------------------------------------------------

  LIDARLite Arduino Library
  GetDistanceI2c

  This example shows how to initialize, configure, and read distance from a
  LIDAR-Lite connected over the I2C interface.

  Connections:
  LIDAR-Lite 5 Vdc (red) to Arduino 5v
  LIDAR-Lite I2C SCL (green) to Arduino SCL
  LIDAR-Lite I2C SDA (blue) to Arduino SDA
  LIDAR-Lite Ground (black) to Arduino GND

  (Capacitor recommended to mitigate inrush current when device is enabled)
  680uF capacitor (+) to Arduino 5v
  680uF capacitor (-) to Arduino GND

  See the Operation Manual for wiring diagrams and more information:
  http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf

------------------------------------------------------------------------------*/

#include <Wire.h>
#include <LIDARLite.h>

LIDARLite myLidarLite;

boolean EMERGENCY_STOP = false;
boolean in_motion = false;
int DISTANCE = 0;
int min_position = 10;
int max_position = 300;

void setup()
{
  Serial.begin(9600); // Initialize serial connection to display distance readings

  /*
    begin(int configuration, bool fasti2c, char lidarliteAddress)

    Starts the sensor and I2C.

    Parameters
    ----------------------------------------------------------------------------
    configuration: Default 0. Selects one of several preset configurations.
    fasti2c: Default 100 kHz. I2C base frequency.
      If true I2C frequency is set to 400kHz.
    lidarliteAddress: Default 0x62. Fill in new address here if changed. See
      operating manual for instructions.
  */
  myLidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz

  /*
    configure(int configuration, char lidarliteAddress)

    Selects one of several preset configurations.

    Parameters
    ----------------------------------------------------------------------------
    configuration:  Default 0.
      0: Default mode, balanced performance.
      1: Short range, high speed. Uses 0x1d maximum acquisition count.
      2: Default range, higher speed short range. Turns on quick termination
          detection for faster measurements at short range (with decreased
          accuracy)
      3: Maximum range. Uses 0xff maximum acquisition count.
      4: High sensitivity detection. Overrides default valid measurement detection
          algorithm, and uses a threshold value for high sensitivity and noise.
      5: Low sensitivity detection. Overrides default valid measurement detection
          algorithm, and uses a threshold value for low sensitivity and noise.
    lidarliteAddress: Default 0x62. Fill in new address here if changed. See
      operating manual for instructions.
  */
  myLidarLite.configure(0); // Change this number to try out alternate configurations

  /*
   * Define Relays
   */
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
}

void loop()
{
  while (Serial.available() > 0) {
    int pulley_destination = Serial.parseInt();
    Serial.println(pulley_destination);
    if (!in_motion) {
      seek_position(pulley_destination);
    }
  }
}

// TODO: running average
  /*
    distance(bool biasCorrection, char lidarliteAddress)

    Take a distance measurement and read the result.

    Parameters
    ----------------------------------------------------------------------------
    biasCorrection: Default true. Take aquisition with receiver bias
      correction. If set to false measurements will be faster. Receiver bias
      correction must be performed periodically. (e.g. 1 out of every 100
      readings).
    lidarliteAddress: Default 0x62. Fill in new address here if changed. See
      operating manual for instructions.
  */
void seek_position(int to_position) {
  in_motion = true;
  //error check input
  if (to_position < min_position || to_position > max_position) {
    in_motion = false;
    return;
  }
  boolean complete = false;
  boolean left_on = false;
  boolean right_on = false;
  while (!complete) {
    DISTANCE = myLidarLite.distance();
    Serial.println(DISTANCE);
    // Take 99 measurements without receiver bias correction
    for(int i = 0; i < 99; i++)
    {
      DISTANCE = myLidarLite.distance(false);
      // kill if passed distance limits
      if (DISTANCE < min_position || DISTANCE > max_position){
        digitalWrite(7, HIGH);
        digitalWrite(6, HIGH);
        in_motion = false;
        return;
      }

      // check if we should move left, and turn on correct relay for left movement
      if (to_position > DISTANCE && !right_on) {
        left_on = true;
        digitalWrite(7, LOW);
      }
      // if position being traveled to has been passed than stop.
      if (to_position < DISTANCE && left_on){
        digitalWrite(7, HIGH);
        in_motion = false;
        return;
      }

      // check if we should move right, and turn on correct relay for right movement
      if (to_position < DISTANCE && !left_on) {
        right_on = true;
        digitalWrite(6, LOW);
      }
      // if position being traveled to has been passed than stop.
      if (to_position > DISTANCE && right_on){
        digitalWrite(6, HIGH);
        in_motion = false;
        return;
      }

    }
  }
}
