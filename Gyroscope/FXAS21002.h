#ifndef FXAS21002_H
#define FXAS21002_H
#include "mbed.h"

#define FXAS21002_I2C_ADDRESS 0x40

#define FXAS21002_STATUS 0x00
#define FXAS21002_WHO_AM_I 0x0C
#define FXAS21002_CTRL_REG0 0x0D
#define FXAS21002_CTRL_REG1 0x13
#define FXAS21002_WHO_AM_I_VALUE 0xD1

class FXAS21002
{
    public:
    
    FXAS21002(PinName sda, PinName scl);
      
    void gyro_config(void);
        
    void acquire_gyro_data_dps(float * du);
    
    private:
    I2C gyroi2c;
    
};

#endif
