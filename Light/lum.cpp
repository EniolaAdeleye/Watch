#include "lum.h"

TSL2561::TSL2561 (PinName sda, PinName scl)
 : _i2c_p(new I2C(sda, scl)), _i2c(*_i2c_p)
{
    TSL2561_addr = TSL2561_ADDRESS_GND;
    init();
}

TSL2561::TSL2561 (PinName sda, PinName scl, uint8_t addr)
 : _i2c_p(new I2C(sda, scl)), _i2c(*_i2c_p)
{
    TSL2561_addr = addr;
    init();
}

TSL2561::TSL2561 (I2C& TSL2561_i2c)
 : _i2c(TSL2561_i2c)
{
    TSL2561_addr = TSL2561_ADDRESS_GND;
    init();
}

TSL2561::TSL2561 (I2C& TSL2561_i2c, uint8_t addr)
 : _i2c(TSL2561_i2c)
{
    TSL2561_addr = addr;
    init();
}


void TSL2561::init()            //Initialize sensor
{
    _i2c.frequency(100000);
    power_up();
    set_timing_reg(TIMING_DEFAULT);
}


float TSL2561::lux()            // Read Lux from sensor
{
    double lux0, lux1;
    double ratio;
    double dlux;

    dt[0] = CMD_MULTI + TSL2561_DATA0LOW;
    _i2c.write((int)TSL2561_addr, (char *)dt, 1, true);
    _i2c.read(TSL2561_addr, (char *)dt, 2, false);
    ch0 = dt[1] << 8 | dt[0];
    dt[0] = CMD_MULTI + TSL2561_DATA1LOW;
    _i2c.write((int)TSL2561_addr, (char *)dt, 1, true);
    _i2c.read(TSL2561_addr, (char *)dt, 2, false);
    ch1 = dt[1] << 8 | dt[0];
    if (ch0 == 0xFFFF) {
        return 2500.0;
    }
    lux0 = (double)ch0;
    lux1 = (double)ch1;
    ratio = lux1 / lux0;
    read_timing_reg();
    lux0 *= (402.0/integ_time);
    lux1 *= (402.0/integ_time);
    lux0 /= gain;
    lux1 /= gain;
    if (ratio <= 0.5) {
        dlux = 0.03040 * lux0 - 0.06200 * lux0 * pow(ratio,1.4);
    } else if (ratio <= 0.61) {
        dlux = 0.02240 * lux0 - 0.03100 * lux1;
    } else if (ratio <= 0.80) {
        dlux = 0.01280 * lux0 - 0.01530 * lux1;
    } else if (ratio <= 1.30) {
        dlux = 0.00146 * lux0 - 0.00112 * lux1;
    } else {
        dlux = 0;
    }
    return (float)dlux;
}


uint8_t TSL2561::set_timing_reg(uint8_t parameter)
{
    dt[0] = CMD_SINGLE + TSL2561_TIMING;
    dt[1] = parameter;
    _i2c.write((int)TSL2561_addr, (char *)dt, 2, false);
    dt[0] = CMD_SINGLE + TSL2561_TIMING;
    _i2c.write((int)TSL2561_addr, (char *)dt, 1, true);
    _i2c.read(TSL2561_addr, (char *)dt, 1, false);
    return dt[0];
}

uint8_t TSL2561::read_timing_reg(void)
{
    uint8_t i;

    dt[0] = CMD_SINGLE + TSL2561_TIMING;
    _i2c.write((int)TSL2561_addr, (char *)dt, 1, true);
    _i2c.read(TSL2561_addr, (char *)dt, 1, false);
    if (dt[0] & TIMING_GAIN_16){
        gain = 16;
    } else {
        gain = 1;
    }
    i = dt[0] & 0x3;
    switch (i) {
        case 0:
            integ_time = 13.7;
            break;
        case 1:
            integ_time = 101.0;
            break;
        case 2:
            integ_time = 402.0;
            break;
        default:
            integ_time = 0;
            break;
    }
    return dt[0];
}

uint16_t TSL2561::get_ID()
{
    dt[0] = CMD_SINGLE + TSL2561_ID;
    _i2c.write((int)TSL2561_addr, (char *)dt, 1, true);
    _i2c.read(TSL2561_addr, (char *)dt, 2, false);
    id_number = dt[0] << 8 | dt[1];
    return id_number;
}

uint8_t TSL2561::get_device_ID()
{
    get_ID();
    if ((id_number >> 8) == I_AM_TSL2561) {
        return 1;
    } else {
        return 0;
    }
}

void TSL2561::power_up()
{
    dt[0] = CMD_SINGLE + TSL2561_CONTROL;
    dt[1] = 3;
    _i2c.write((int)TSL2561_addr, (char *)dt, 2, false);
}

void TSL2561::power_down()
{
    dt[0] = CMD_SINGLE + TSL2561_CONTROL;
    dt[1] = 0;
    _i2c.write((int)TSL2561_addr, (char *)dt, 2, false);
}

void TSL2561::frequency(int hz)
{
    _i2c.frequency(hz);
}


