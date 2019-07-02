#include "accel_mag.h"
#include "mbed.h"

FXOS8700::FXOS8700(PinName sda, PinName scl) : FXOS8700_i2c(sda,scl)
 {
       
 }
    
 /*Configure the accelerometer*/
void FXOS8700::configureAccelerometer(void)
 {
   char d[2]; 
//Put accelerometer in standby mode
   d[0] = FXOS8700_CTRL_REG1;                     
   d[1] = 0x00; 
   FXOS8700_i2c.write(FXOS8700_I2C_ADDRESS, d,2);   
          
//Put accelerometer in active mode
   d[0] = FXOS8700_CTRL_REG1;                     
   d[1] = 0x01;
   FXOS8700_i2c.write(FXOS8700_I2C_ADDRESS, d, 2);   
      
 }
 /*Configure the magnetometer*/
  void FXOS8700:: configureMagnetometer(void)
 {
   char d[2];
   d[0] = FXOS8700_CTRL_REG1;                     //Standby mode (Magnetometer)
   d[1] = 0x00;
   FXOS8700_i2c.write(FXOS8700_I2C_ADDRESS, d,2);   
          
   
   d[0] = FXOS8700_M_CTRL_REG1;                   //hybrid mode (both accelerometer and magnetometer are active)
   d[1] = 0x03;
   FXOS8700_i2c.write(FXOS8700_I2C_ADDRESS, d, 2);   
   

   d[0] = FXOS8700_CTRL_REG1;                     //Active mode (Magnetometer)
   d[1] = 0x01;
   FXOS8700_i2c.write(FXOS8700_I2C_ADDRESS, d,2);  
   
 }
 
 void FXOS8700::getAccelData(float * accelData)
 {
  
   char data_bytes[7];
   char d[1];
   d[0]=FXOS8700_STATUS;
   FXOS8700_i2c.write(FXOS8700_I2C_ADDRESS,d,1,true);
//Read data from X Y Z
   FXOS8700_i2c.read(FXOS8700_I2C_ADDRESS,data_bytes,7);
   
/*Calculation derived from the datasheet*/
/*256 = 2^8  or >>8 can be used*/
   accelData[0] =  ((float)((int16_t)(((data_bytes[1]<<8) + (data_bytes[2])))>> 2) * 0.000244);
   accelData[1] =  ((float)((int16_t)(((data_bytes[3]<<8) + (data_bytes[4])))>> 2) * 0.000244);
   accelData[2] =  ((float)((int16_t)(((data_bytes[5]<<8) + (data_bytes[6])))>> 2) * 0.000244);
   
 }

 void FXOS8700::getMagData(float * magData)
 {
  
   char data_bytes[7];
   char d[1];
   d[0]=FXOS8700_MDR_STATUS;
   FXOS8700_i2c.write(FXOS8700_I2C_ADDRESS,d,1,true);  // Read data from X Y Z
   FXOS8700_i2c.read(FXOS8700_I2C_ADDRESS,data_bytes,7);
   
   magData[0] =  (float)((int16_t)((data_bytes[1]<<8) + (data_bytes[2]))) * 0.1;
   magData[1] =  (float)((int16_t)((data_bytes[3]<<8) + (data_bytes[4]))) * 0.1;
   magData[2] =  (float)((int16_t)((data_bytes[5]<<8) + (data_bytes[6]))) * 0.1;
   
 }
     
     

