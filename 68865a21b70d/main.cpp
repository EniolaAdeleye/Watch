#include "mbed.h"
#include "FXOS8700.h"
#include "Hexi_OLED_SSD1351.h"
#include "images.h"
#include "string.h"

// Pin connections
DigitalOut led1(LED_GREEN); // RGB LED
Serial pc(USBTX, USBRX); // Serial interface
FXOS8700 accel(PTC11, PTC10);
SSD1351 oled(PTB22,PTB21,PTC13,PTB20,PTE6, PTD15); // SSD1351 OLED Driver (MOSI,SCLK,POWER,CS,RST,DC)

// Variables
float accel_data[3]; // Storage for the data from the sensor
float accel_rms=0.0; // RMS value from the sensor
float ax, ay, az; // Integer value from the sensor to be displayed
const uint8_t *image1; // Pointer for the image1 to be displayed
char text1[20]; // Text Buffer for dynamic value displayed
char text2[20]; // Text Buffer for dynamic value displayed
char text3[20]; // Text Buffer for dynamic value displayed

int main() {
        
    // Configure Accelerometer FXOS8700, Magnetometer FXOS8700
    accel.accel_config();
    
    // Setting pointer location of the 96 by 96 pixel bitmap
    image1  = Accelero;

    // Dimm Down OLED backlight
//    oled.DimScreenON();
    
    // Fill 96px by 96px Screen with 96px by 96px Image starting at x=0,y=0
    oled.DrawImage(image1,0,0);  

    
    while (true) 
    {
    
      accel.acquire_accel_data_g(accel_data);
      accel_rms = sqrt(((accel_data[0]*accel_data[0])+(accel_data[1]*accel_data[1])+(accel_data[2]*accel_data[2]))/3);
      printf("Accelerometer \tX-Axis %4.2f \tY-Axis %4.2f \tZ-Axis %4.2f \tRMS %4.2f\n\r",accel_data[0],accel_data[1],accel_data[2],accel_rms);
      wait(0.01);
      ax = accel_data[0];
      ay = accel_data[1];
      az = accel_data[2];             

      /* Get OLED Class Default Text Properties */
      oled_text_properties_t textProperties = {0};
      oled.GetTextProperties(&textProperties); 
      
      /* Set text properties to white and right aligned for the dynamic text */
      textProperties.fontColor = COLOR_BLUE;
      textProperties.alignParam = OLED_TEXT_ALIGN_LEFT;
      oled.SetTextProperties(&textProperties);  
      
      /* Display Legends */
      strcpy((char *) text1,"X-Axis (g):");
      oled.Label((uint8_t *)text1,3,45);      
      
      /* Format the value */
      sprintf(text1,"%4.2f",ax);
      /* Display time reading in 35px by 15px textbox at(x=55, y=40) */
      oled.TextBox((uint8_t *)text1,70,45,20,15); //Increase textbox for more digits

      /* Set text properties to white and right aligned for the dynamic text */ 
      textProperties.fontColor = COLOR_GREEN;
      textProperties.alignParam = OLED_TEXT_ALIGN_LEFT;
      oled.SetTextProperties(&textProperties);  

      /* Display Legends */
      strcpy((char *) text2,"Y-Axis (g):");
      oled.Label((uint8_t *)text2,3,62); 
      
      /* Format the value */
      sprintf(text2,"%4.2f",ay);
      /* Display time reading in 35px by 15px textbox at(x=55, y=40) */
      oled.TextBox((uint8_t *)text2,70,62,20,15); //Increase textbox for more digits
      
      /* Set text properties to white and right aligned for the dynamic text */ 
      textProperties.fontColor = COLOR_RED;
      textProperties.alignParam = OLED_TEXT_ALIGN_LEFT;
      oled.SetTextProperties(&textProperties);  
      
      /* Display Legends */
      strcpy((char *) text3,"Z-Axis (g):");
      oled.Label((uint8_t *)text3,3,79);       
      
      /* Format the value */
      sprintf(text3,"%4.2f",az);
      /* Display time reading in 35px by 15px textbox at(x=55, y=40) */
      oled.TextBox((uint8_t *)text3,70,79,20,15); //Increase textbox for more digits

      led1 = !led1;
      Thread::wait(250);
    }
}