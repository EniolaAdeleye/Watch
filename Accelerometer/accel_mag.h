#ifndef accel_mag_h
#define accel_mag_h
#include "mbed.h"

#define FXOS8700_I2C_ADDRESS (0x1E<<1) //SA0pin and SA1pin =0

#define FXOS8700_STATUS             0x00
#define FXOS8700_MDR_STATUS         0x32
#define FXOS8700_WHOAMI             0x0D
#define FXOS8700_CTRL_REG1          0x2A
#define FXOS8700_M_CTRL_REG1        0x5B
#define FXOS8700_WHOAMI_VALUE       0xC7

class FXOS8700
{
/*Call functions for configuration and data acquisition*/
    public:
    
    FXOS8700(PinName sda, PinName scl);

    void configureAccelerometer(void); 
    
    void configureMagnetometer(void);  
        
    void getAccelData(float * da); 
    
    void getMagData(float * dm);
    
    private:
   I2C FXOS8700_i2c;   
    
};

#endif

