#ifndef lum_H
#define lum_H

#include "mbed.h"

// Luminosity sensor, TSL2561
// Address b7=0,b6=1,b5=1,b4=1,b3=0,b2=0,b1=1, b0=R/W
#define TSL2561_ADDRESS_GND         (0x29 << 1)
#define TSL2561_ADDRESS_FLOAT       (0x39 << 1)
#define TSL2561_ADDRESS_VDD         (0x49 << 1)


// Registers
#define TSL2561_CONTROL             0x00 //Control of basic functions
#define TSL2561_TIMING              0x01 //Integration time/gain control
#define TSL2561_THRESHLOWLOW        0x02 //Low byte of low interrupt threshold
#define TSL2561_THRESHHIGHLOW       0x04 //High byte of low interrupt threshold
#define TSL2561_INTERRUPT           0x06 //Interrupt control
#define TSL2561_CRC                 0x08 //Factory test — not a user register
#define TSL2561_ID                  0x0A //Part number/ Rev ID
#define TSL2561_DATA0LOW            0x0C //Low byte of ADC channel 0
#define TSL2561_DATA0HIGH           0x0D //High byte of ADC channel 0
#define TSL2561_DATA1LOW            0x0E //Low byte of ADC channel 1
#define TSL2561_DATA1HIGH           0x0F //High byte of ADC channel 1


#define TIMING_GAIN_1               (0UL << 4)
#define TIMING_GAIN_16              (1UL << 4)
#define TIMING_TIME_13R7            (0x0)
#define TIMING_TIME_101             (0x1)
#define TIMING_TIME_402             (0x2)
#define TIMING_TIME_MANU            (0x3)
#define TIMING_DEFAULT              (TIMING_GAIN_1 + TIMING_TIME_402)

// ID 
#define I_AM_TSL2561                0x50
#define REG_NO_MASK                 0x0F

// COMMAND 
#define CMD_CMDMODE                 (1UL << 7)
#define CMD_CLEAR                   (1UL << 6)
#define CMD_WORD                    (1UL << 5)
#define CMD_BLOCK                   (1UL << 4)
#define CMD_SINGLE                  (CMD_CMDMODE)
#define CMD_MULTI                   (CMD_CMDMODE + CMD_WORD)

class TSL2561
{
public:
    TSL2561(PinName sda, PinName scl);      //set pinname
    TSL2561(PinName sda, PinName scl, uint8_t addr);

    TSL2561(I2C& TSL2561_i2c);
    TSL2561(I2C& TSL2561_i2c, uint8_t addr);

    float lux(void);
    uint8_t set_timing_reg(uint8_t parameter);    //Set timing register
    uint8_t read_timing_reg(void);    //Read timing register
    void frequency(int hz);           //Set I2C clock frequency
    uint8_t get_device_ID(void);           //check Device ID number
    uint16_t get_ID(void);           //Read ID and Revision Number
    void power_up(void);
    void power_down(void);

protected:
    I2C *_i2c_p;
    I2C &_i2c;

    void init(void);   //initialize

private:
    uint8_t  TSL2561_addr;
    uint8_t  dt[4];
    uint32_t ch0;
    uint32_t ch1;
    int8_t   gain;
    uint16_t  id_number;
    double   integ_time;
};

#endif    