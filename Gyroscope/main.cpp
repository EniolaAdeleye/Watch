#include "mbed.h"
#include "FXAS21002.h"
//#include "Hexi_OLED_SSD1351.h"
//#include "images.h"
#include "string.h"

// Pin connections
DigitalOut led1(LED_GREEN); // RGB LED
Serial pc(USBTX, USBRX); // Serial interface
FXAS21002 gyro(PTC11,PTC10); // Gyroscope
//SSD1351 oled(PTB22,PTB21,PTC13,PTB20,PTE6, PTD15); // SSD1351 OLED Driver (MOSI,SCLK,POWER,CS,RST,DC)

// Variables
float gyro_data[3];  // Storage for the data from the sensor
//float gyro_rms=0.0; // RMS value from the sensor
float gx, gy, gz; // Integer value from the sensor to be displayed
//const uint8_t *image1; // Pointer for the image1 to be displayed
char text1[20]; // Text Buffer for dynamic value displayed
char text2[20]; // Text Buffer for dynamic value displayed
char text3[20]; // Text Buffer for dynamic value displayed

int main() {
    
    // Configure Gyroscope FXAS21002    
    gyro.gyro_config();
    
    /* Setting pointer location of the 96 by 96 pixel bitmap */
    //image1  = Gyro;

    // Dimm Down OLED backlight
//    oled.DimScreenON();
    
    /* Fill 96px by 96px Screen with 96px by 96px NXP Image starting at x=0,y=0 */
    //oled.DrawImage(image1,0,0);  

    
    while (true) 
    {
    
      gyro.acquire_gyro_data_dps(gyro_data);
      //gyro_rms = sqrt(((gyro_data[0]*gyro_data[0])+(gyro_data[1]*gyro_data[1])+(gyro_data[2]*gyro_data[2]))/3);
      pc.printf("Gyroscope \tRoll (G) %4.2f \tPitch (G) %4.2f \tYaw (G) %4.2f \n\r",gyro_data[0],gyro_data[1],gyro_data[2]);
      wait(0.01);
      gx = gyro_data[0];
      gy = gyro_data[1];
      gz = gyro_data[2];  

      
      led1 = !led1;
      Thread::wait(1000);
    }
}
