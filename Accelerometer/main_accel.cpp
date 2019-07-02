#include "mbed.h"
#include "accel_mag.h"
#include "string.h"

// Pin connections
DigitalOut led1(LED_GREEN); // RGB LED
Serial pc(USBTX, USBRX); // Serial interface
FXOS8700 accel(PTC11, PTC10);

// Variables
float accelData[3]; // Storage for the data from the sensor
float accel_x, accel_y, accel_z; // Integer value from the sensor to be displayed
char text1[20]; // Text Buffer for dynamic value displayed
char text2[20]; // Text Buffer for dynamic value displayed
char text3[20]; // Text Buffer for dynamic value displayed

int main() {
        
    // Configure Accelerometer
    accel.configureAccelerometer();
    
    while (true) 
    {
    
      accel.getAccelData(accelData);
      printf("Accelerometer: \tX-Axis %4.2f \tY-Axis %4.2f \tZ-Axis %4.2f\n\r",accelData[0],accelData[1],accelData[2]);
      
      wait(0.01);
      accel_x = accelData[0];
      accel_y = accelData[1];
      accel_z = accelData[2];             

      led1 = !led1;
      Thread::wait(250);
    }
}

