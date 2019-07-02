#include "mbed.h"
#include "lum.h"
Serial      pc(USBTX, USBRX); // Serial interface
DigitalOut  sensorPowerEn(PTB12);
DigitalOut  led1(LED1);

TSL2561     lum(PTB1,PTB0);    // TSL2561 SDA, SCL

int main() {
    
    sensorPowerEn = 0;
    pc.baud(9600);
    printf("who am i : %d \r\n", lum.get_device_ID());

    while (true) {
        led1 = !led1;
        printf("Illuminance: %+7.2f [Lux]\r\n", lum.lux());
        wait(0.5);
        
    }
}


