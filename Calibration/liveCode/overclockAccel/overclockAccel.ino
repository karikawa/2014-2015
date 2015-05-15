/*
*  Accelerometer skeleton code
*/
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <URP_LSM303.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <String.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_10DOF.h>

/* Assign a unique ID to the sensors */
Adafruit_10DOF                dof   = Adafruit_10DOF();
URP_LSM303_Accel accel = URP_LSM303_Accel(30301);
const char* FILENAME = "tlaunch.txt";
sensors_event_t accel_event;
sensors_vec_t   orientation;
//data buffer for the accel and orientation readings
char dataString[60];

void initAccel()
{
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    while(1);
  }
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void setup(void)
{
  //this is needed for the duemilanove and uno without ethernet shield
  const int sdCardPin = 10;
  delay(2000);
  pinMode(10,OUTPUT);
  if (!SD.begin(sdCardPin)) {
    // don't do anything more:
    return;
  }
  //format
  sdWrite("AccelX, AccelY, AccelZ, Roll, Pitch, Heading");
  sdWriteNewline();
  /* Initialise the sensors */
  initAccel();
  accel.setDataRate((byte)LSM303_ACCEL_DATA_RATE_400HZ);
  accel.setDataRate((byte)LSM303_MAG_DATA_RATE_220HZ);

}

/**************************************************************************/
/*!
    @brief  Constantly check the roll/pitch/heading/altitude/temperature
*/
/**************************************************************************/
void loop(void)
{
  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    sdWrite((String) accel_event.acceleration.x);
    sdWrite((String) accel_event.acceleration.y);
    sdWrite((String) accel_event.acceleration.z);
    sdWrite((String) accel_event.orientation.roll);
    sdWrite((String) millis());
    sdWriteNewline();
  }
}
bool sdWriteNewline() {
  File dataFile = SD.open(FILENAME, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("");
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    dataFile.close();
  }
}

void sdWrite(String data) {
  File dataFile = SD.open(FILENAME, FILE_WRITE);
  // if the file is available, write to it:
  dataFile.print(data + ", ");
  dataFile.close();
}
