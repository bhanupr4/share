#include<Wire.h>
#define accm (0x53)
//address 0x53
/*0x2D resistor
  _______________________________________________
  D7| D6 |  D5 |    D4    |  D3   | D2  |D1|D0 |
  0 |  0 |LINK |AUTO_SLEEP|MEASURE|SLEEP|WAKEUP|
  0 |  0 | 0   |     0    |   1   |  0  |0 |0  |=8//SET TO MEASURE AND DISABLE AUTO SLEEP

  Wire.beginTransmission(accm);
  Wire.write(0x32);
  Wire.endTransmision();
  trigger value to get data
  0x32    50      DATAX0      R      00000000        X-Axis Data 0.
  0x33    51      DATAX1      R      00000000        X-Axis Data 1.
  0x34    52      DATAY0      R      00000000        Y-Axis Data 0.
  0x35    53      DATAY1      R      00000000        Y-Axis Data 1.
  0x36    54      DATAZ0      R      00000000        Z-Axis Data 0.
  0x37    55      DATAZ1      R      00000000        Z-Axis Data 1.

  0x1D    29      THRESH_TAP  R/W    00000000        Tap threshold.
  0x1E    30      OFSX        R/W    00000000        X-axis offset.
  0x1F    31      OFSY        R/W    00000000        Y-axis offset.
  0x20    32      OFSZ        R/W    00000000        Z-axis offset.
*/
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  //to disable sleep mode
  Wire.beginTransmission(accm);
  Wire.write(0x2D);//registor to set flag value
  Wire.write(8);//disable auto sleep
  Wire.endTransmission();
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(accm);
  Wire.write(0x32);
  Wire.endTransmission();
  Wire.requestFrom(accm, 1);
  Serial.println(Wire.read());
  delay(100);
}
