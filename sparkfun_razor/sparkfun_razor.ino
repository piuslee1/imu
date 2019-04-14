/************************************************************
MPU9250_Basic
 Basic example sketch for MPU-9250 DMP Arduino Library 
Jim Lindblom @ SparkFun Electronics
original creation date: November 23, 2016
https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library
This example sketch demonstrates how to initialize the 
MPU-9250, and stream its sensor outputs to a serial monitor.
Development environment specifics:
Arduino IDE 1.6.12
SparkFun 9DoF Razor IMU M0
Supported Platforms:
- ATSAMD21 (Arduino Zero, SparkFun SAMD21 Breakouts)
*************************************************************/
#include <SparkFunMPU9250-DMP.h>
#include <stdio.h>
#define SerialPort SerialUSB
#define vals_len 9
#define indiv_val_len 7

MPU9250_DMP imu;
float vals[vals_len];
char buffer[vals_len*indiv_val_len];
char * val_formats[] = {
  "%1.4f,", "%1.4f,", "%1.4f,", 
  "%1.4f,", "%1.4f,", "%1.4f,", 
  "%4.1f,", "%4.1f,", "%4.1f,"
};

void setup() 
{
  SerialPort.begin(115200);
  Serial1.begin(115200);
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      SerialPort.println("Unable to communicate with MPU-9250");
      SerialPort.println("Check connections, and try again.");
      SerialPort.println();
      delay(5000);
    }
  }
  // Use setSensors to turn on or off MPU-9250 sensors.
  // Any of the following defines can be combined:
  // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
  // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  // Enable all sensors:
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
  // Use setGyroFSR() and setAccelFSR() to configure the
  // gyroscope and accelerometer full scale ranges.
  // Gyro options are +/- 250, 500, 1000, or 2000 dps
  imu.setGyroFSR(2000); // Set gyro to 2000 dps
  // Accel options are +/- 2, 4, 8, or 16 g
  imu.setAccelFSR(2); // Set accel to +/-2g
  // Note: the MPU-9250's magnetometer FSR is set at 
  // +/- 4912 uT (micro-tesla's)
  // setLPF() can be used to set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // Can be any of the following: 188, 98, 42, 20, 10, 5
  // (values are in Hz).
  imu.setLPF(188); // Set LPF corner frequency to 5Hz
  // The sample rate of the accel/gyro can be set using
  // setSampleRate. Acceptable values range from 4Hz to 1kHz
  imu.setSampleRate(1000); // Set sample rate to 10Hz
  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  imu.setCompassSampleRate(100); // Set mag rate to 10Hz
}
void loop() 
{
  // dataReady() checks to see if new accel/gyro data
  // is available. It will return a boolean true or false
  // (New magnetometer data cannot be checked, as the library
  //  runs that sensor in single-conversion mode.)
  if ( imu.dataReady() )
  {
    // Call update() to update the imu objects sensor data.
    // You can specify which sensors to update by combining
    // UPDATE_ACCEL, UPDATE_GYRO, UPDATE_COMPASS, and/or
    // UPDATE_TEMPERATURE.
    // (The update function defaults to accel, gyro, compass,
    //  so you don't have to specify these values.)
    imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    printIMUData();
  }
  /*if (Serial1.available()) {*/
    /*SerialPort.println(Serial1.readStringUntil('\n'));*/
  /*}*/
}
void printIMUData(void)
{  
  // After calling update() the ax, ay, az, gx, gy, gz, mx,
  // my, mz, time, and/or temerature class variables are all
  // updated. Access them by placing the object. in front:
  // Use the calcAccel, calcGyro, and calcMag functions to
  // convert the raw sensor readings (signed 16-bit values)
  // to their respective units.
  vals[0] = imu.calcAccel(imu.ax);
  vals[1] = imu.calcAccel(imu.ay);
  vals[2] = imu.calcAccel(imu.az);
  vals[3] = imu.calcGyro(imu.gx);
  vals[4] = imu.calcGyro(imu.gy);
  vals[5] = imu.calcGyro(imu.gz);
  vals[6] = imu.calcMag(imu.mx);
  vals[7] = imu.calcMag(imu.my);
  vals[8] = imu.calcMag(imu.mz);
  int start = 0;
  for (int i=0; i<vals_len; i++) {
    // 2+4 digits + period and comma
    start += sprintf(&buffer[start], val_formats[i], vals[i]);
  }
  /*buffer[(vals_len-1)*indiv_val_len+1] = 0;*/
  SerialPort.println(buffer);
  
  /*SerialPort.println("Accel: " + String(accelX) + ", " +*/
              /*String(accelY) + ", " + String(accelZ) + " g");*/
  /*SerialPort.println("Gyro: " + String(gyroX) + ", " +*/
              /*String(gyroY) + ", " + String(gyroZ) + " dps");*/
  /*SerialPort.println("Mag: " + String(magX) + ", " +*/
              /*String(magY) + ", " + String(magZ) + " uT");*/
  /*SerialPort.println("Time: " + String(imu.time) + " ms");*/
  /*SerialPort.println();*/
}
