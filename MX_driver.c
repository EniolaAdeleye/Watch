//Heartrate MX_driver.c

#include <MX_driver.h>

#include <log.h>

#define LOG_LEVEL CONFIG_SENSOR_LEVEL
LOG_MODULE_REGISTER(MX);

static int MX_GET_SAMPLES(struct device *devMem, enum sensor_channel chan){           /*the *devMem will will take the address of the variable and get the driver data passed into the MX_DATA *data address pointer*/

    struct mx_data *data = (*devMem).driver_data;
    u8_t buffer[MX_MAX_BYTES_PER_SAMPLE];
    int fifo_chan;
    u32_t fifo_data;
    int byteNumber;
    int j;
    /*read through all the channel for sample*/
    byteNumber = (*data).num_channel * MX_MAX_BYTES_PER_SAMPLE;                  //total number of bytes through all chan
    if (i2c_burst_read((*data).i2c, MX_I2C_ADDRESS,MX_REG_FIFO_DATA,buffer, byteNumber)){       //reading multiple byte from internal address
        LOG_ERR("Could Not Fetch Data");
        return -EIO;                                                            //return the general input and output error
    }

    fifo_chan = 0;
    for (j=0; j < byteNumber; j+=3){                                            //each channel is 18bits
        fifo_data = (buffer[j] << 16) | (buffer[j + 1] << 8) | (buffer[j + 2]);
        fifo_data &= MX_FIFO_DATA_MASK;

        data->raw[fifo_chan++] = fifo_data;                     //saving raw data
    }
    return 0;
}
