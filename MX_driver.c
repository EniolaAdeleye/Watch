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

	fifo_chan = 0;
	for (i = 0; i < num_bytes; i += 3) {
		/* Each channel is 18-bits */
		fifo_data = (buffer[i] << 16) | (buffer[i + 1] << 8) |
			    (buffer[i + 2]);
		fifo_data &= MX_FIFO_DATA_MASK;

		/* Save the raw data */
		data->raw[fifo_chan++] = fifo_data;
	}

	return 0;
}

static int MX_channel_get(struct device *dev, enum sensor_channel chan,
				struct sensor_value *val)
{
	struct MX_data *data = dev->driver_data;
	enum MX_led_channel led_chan;
	int fifo_chan;

	switch (chan) {
	case SENSOR_CHAN_RED:
		led_chan = MX_LED_CHANNEL_RED;
		break;

	case SENSOR_CHAN_IR:
		led_chan = MX_LED_CHANNEL_IR;
		break;

	case SENSOR_CHAN_GREEN:
		led_chan = MX_LED_CHANNEL_GREEN;
		break;

	default:
		LOG_ERR("Unsupported sensor channel");
		return -ENOTSUP;
	}

	/* Check if the led channel is active by looking up the associated fifo
	 * channel. If the fifo channel isn't valid, then the led channel
	 * isn't active.
	 */
	fifo_chan = data->map[led_chan];
	if (fifo_chan >= MX_MAX_NUM_CHANNELS) {
		LOG_ERR("Inactive sensor channel");
		return -ENOTSUP;
	}

	/* TODO: Scale the raw data to standard units */
	val->val1 = data->raw[fifo_chan];
	val->val2 = 0;

	return 0;
}

static const struct sensor_driver_api MX_driver_api = {
	.sample_fetch = MX_sample_fetch,
	.channel_get = MX_channel_get,
};

static int MX_init(struct device *dev)
{
	const struct MX_config *config = dev->config->config_info;
	struct MX_data *data = dev->driver_data;
	u8_t part_id;
	u8_t mode_cfg;
	u32_t led_chan;
	int fifo_chan;

	/* Get the I2C device */
	data->i2c = device_get_binding(DT_MAX_MX_0_BUS_NAME);
	if (!data->i2c) {
		LOG_ERR("Could not find I2C device");
		return -EINVAL;
	}

	/* Check the part id to make sure this is MX */
	if (i2c_reg_read_byte(data->i2c, MX_I2C_ADDRESS,
			      MX_REG_PART_ID, &part_id)) {
		LOG_ERR("Could not get Part ID");
		return -EIO;
	}
	if (part_id != MX_PART_ID) {
		LOG_ERR("Got Part ID 0x%02x, expected 0x%02x",
			    part_id, MX_PART_ID);
		return -EIO;
	}

	/* Reset the sensor */
	if (i2c_reg_write_byte(data->i2c, MX_I2C_ADDRESS,
			       MX_REG_MODE_CFG,
			       MX_MODE_CFG_RESET_MASK)) {
		return -EIO;
	}

	/* Wait for reset to be cleared */
	do {
		if (i2c_reg_read_byte(data->i2c, MX_I2C_ADDRESS,
				      MX_REG_MODE_CFG, &mode_cfg)) {
			LOG_ERR("Could read mode cfg after reset");
			return -EIO;
		}
	} while (mode_cfg & MX_MODE_CFG_RESET_MASK);

	/* Write the FIFO configuration register */
	if (i2c_reg_write_byte(data->i2c, MX_I2C_ADDRESS,
			       MX_REG_FIFO_CFG, config->fifo)) {
		return -EIO;
	}

	/* Write the mode configuration register */
	if (i2c_reg_write_byte(data->i2c, MX_I2C_ADDRESS,
			       MX_REG_MODE_CFG, config->mode)) {
		return -EIO;
	}

	/* Write the SpO2 configuration register */
	if (i2c_reg_write_byte(data->i2c, MX_I2C_ADDRESS,
			       MX_REG_SPO2_CFG, config->spo2)) {
		return -EIO;
	}

	/* Write the LED pulse amplitude registers */
	if (i2c_reg_write_byte(data->i2c, MX_I2C_ADDRESS,
			       MX_REG_LED1_PA, config->led_pa[0])) {
		return -EIO;
	}
	if (i2c_reg_write_byte(data->i2c, MX_I2C_ADDRESS,
			       MX_REG_LED2_PA, config->led_pa[1])) {
		return -EIO;
	}
	if (i2c_reg_write_byte(data->i2c, MX_I2C_ADDRESS,
			       MX_REG_LED3_PA, config->led_pa[2])) {
		return -EIO;
	}

#ifdef CONFIG_MX_MULTI_LED_MODE
	u8_t multi_led[2];

	/* Write the multi-LED mode control registers */
	multi_led[0] = (config->slot[1] << 4) | (config->slot[0]);
	multi_led[1] = (config->slot[3] << 4) | (config->slot[2]);

	if (i2c_reg_write_byte(data->i2c, MX_I2C_ADDRESS,
			       MX_REG_MULTI_LED, multi_led[0])) {
		return -EIO;
	}
	if (i2c_reg_write_byte(data->i2c, MX_I2C_ADDRESS,
			       MX_REG_MULTI_LED + 1, multi_led[1])) {
		return -EIO;
	}
#endif

	/* Initialize the channel map and active channel count */
	data->num_channels = 0U;
	for (led_chan = 0U; led_chan < MX_MAX_NUM_CHANNELS; led_chan++) {
		data->map[led_chan] = MX_MAX_NUM_CHANNELS;
	}

	/* Count the number of active channels and build a map that translates
	 * the LED channel number (red/ir/green) to the fifo channel number.
	 */
	for (fifo_chan = 0; fifo_chan < MX_MAX_NUM_CHANNELS;
	     fifo_chan++) {
		led_chan = (config->slot[fifo_chan] & MX_SLOT_LED_MASK)-1;
		if (led_chan < MX_MAX_NUM_CHANNELS) {
			data->map[led_chan] = fifo_chan;
			data->num_channels++;
		}
	}

	return 0;
}

static struct MX_config MX_config = {
	.fifo = (CONFIG_MX_SMP_AVE << MX_FIFO_CFG_SMP_AVE_SHIFT) |
#ifdef CONFIG_MX_FIFO_ROLLOVER_EN
		MX_FIFO_CFG_ROLLOVER_EN_MASK |
#endif
		(CONFIG_MX_FIFO_A_FULL <<
		 MX_FIFO_CFG_FIFO_FULL_SHIFT),

#if defined(CONFIG_MX_HEART_RATE_MODE)
	.mode = MX_MODE_HEART_RATE,
	.slot[0] = MX_SLOT_RED_LED1_PA,
	.slot[1] = MX_SLOT_DISABLED,
	.slot[2] = MX_SLOT_DISABLED,
	.slot[3] = MX_SLOT_DISABLED,
#elif defined(CONFIG_MX_SPO2_MODE)
	.mode = MX_MODE_SPO2,
	.slot[0] = MX_SLOT_RED_LED1_PA,
	.slot[1] = MX_SLOT_IR_LED2_PA,
	.slot[2] = MX_SLOT_DISABLED,
	.slot[3] = MX_SLOT_DISABLED,
#else
	.mode = MX_MODE_MULTI_LED,
	.slot[0] = CONFIG_MX_SLOT1,
	.slot[1] = CONFIG_MX_SLOT2,
	.slot[2] = CONFIG_MX_SLOT3,
	.slot[3] = CONFIG_MX_SLOT4,
#endif

	.spo2 = (CONFIG_MX_ADC_RGE << MX_SPO2_ADC_RGE_SHIFT) |
		(CONFIG_MX_SR << MX_SPO2_SR_SHIFT) |
		(MX_PW_18BITS << MX_SPO2_PW_SHIFT),

	.led_pa[0] = CONFIG_MX_LED1_PA,
	.led_pa[1] = CONFIG_MX_LED2_PA,
	.led_pa[2] = CONFIG_MX_LED3_PA,
};

static struct MX_data MX_data;

DEVICE_AND_API_INIT(MX, DT_MAX_MX_0_LABEL, MX_init,
		    &MX_data, &MX_config,
		    POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,
		    &MX_driver_api);
