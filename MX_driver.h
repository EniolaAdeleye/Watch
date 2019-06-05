// Heartrate driver for Smartwatch
// MAXIM 30101

#define MX_REG_INT_STATUS1            		0x00    //Interrupt status 1
#define MX_REG_INT_STATUS2            		0x01    //Interrupt status 2
#define MX_REG_INT_ENABLE1            		0x02    //Interrupt Enable 1
#define MX_REG_INT_ENABLE2            		0x03    //Interrupt Enable 2
#define MX_REG_FIFO_WR_PTR            		0x04    //FIFO Write Pointer
#define MX_REG_FIFO_OVF_COUNTER       		0x05    //Overflow Counter
#define MX_REG_FIFO_RD_PTR            		0x06    //FIFO Read Pointer
#define MX_REG_FIFO_DATA              		0x07    //FIFO Data Register
#define MX_REG_FIFO_CONFIG            		0x08    //FIFO Configuration
#define MX_REG_MODE_CONFIG            		0x09    //Mode Configuration
#define MX_REG_SPO2_CONFIG            		0x0a    //SpO2 Configuration
#define MX_REG_LED1_PA                		0x0c    //LED Pulse Amplitude 1
#define MX_REG_LED2_PA                		0x0d    //LED Pulse Amplitude 2
#define MX_REG_LED3_PA                		0x0e    //LED Pulse Amplitude 3
#define MX_REG_PILOT_PA               		0x10
#define MX_REG_MULTI_LED              		0x11    //Multi-LED Mode Control Register
#define MX_REG_TEMP_INT               		0x1f    //Temp Value_Integer
#define MX_REG_TEMP_FRAC              		0x20    //Temp Value_Fraction
#define MX_REG_TEMP_CONFIG            		0x21    //Die Temperature Config
#define MX_REG_PROX_INT               		0x30
#define MX_REG_REV_ID                 		0xfe    //Revision ID
#define MX_REG_PART_ID                		0xff    //Part ID

#define MX_I2C_ADDRESS		            	0x57
/*

#define MX_INT_PPG_MASK				(1 << 6)

#define MX_FIFO_CFG_SMP_AVE_SHIFT		5
#define MX_FIFO_CFG_FIFO_FULL_SHIFT		0
#define MX_FIFO_CFG_ROLLOVER_EN_MASK		(1 << 4)

#define MX_MODE_CFG_SHDN_MASK			(1 << 7)
#define MX_MODE_CFG_RESET_MASK			(1 << 6)

#define MX_SPO2_ADC_RGE_SHIFT			5
#define MX_SPO2_SR_SHIFT			2
#define MX_SPO2_PW_SHIFT			0

#define MX_PART_ID		                0x15

#define MX_BYTES_PER_CHANNEL			3
#define MX_MAX_NUM_CHANNELS			3
#define MX_MAX_BYTES_PER_SAMPLE			(MX_MAX_NUM_CHANNELS * \
						MX_BYTES_PER_CHANNEL)

#define MX_SLOT_LED_MASK			0x03

#define MX_FIFO_DATA_BITS			18
#define MX_FIFO_DATA_MASK			((1 << 18) - 1)


enum mx_mode {
	MX_MODE_HEART_RATE			= 2,
	MX_MODE_SPO2				= 3,
	MX_MODE_MULTI_LED			= 7,
};

enum mx_slot {
	MX_SLOT_DISABLED			= 0,
	MX_SLOT_RED_LED1_PA,
	MX_SLOT_IR_LED2_PA,
	MX_SLOT_GREEN_LED3_PA,
	MX_SLOT_RED_PILOT_PA,
	MX_SLOT_IR_PILOT_PA,
	MX_SLOT_GREEN_PILOT_PA,
};

enum mx_led_channel {
	MX_LED_CHANNEL_RED			= 0,
	MX_LED_CHANNEL_IR,
	MX_LED_CHANNEL_GREEN,
};

enum mx_pw {
	MX_PW_15BITS				= 0,
	MX_PW_16BITS,
	MX_PW_17BITS,
	MX_PW_18BITS,
};

struct mx_config {
	u8_t fifo;
	u8_t spo2;
	u8_t led_pa[MX_MAX_NUM_CHANNELS];
	enum mx_mode mode;
	enum mx_slot slot[4];
};

struct mx_data {
	struct device *i2c;
	u32_t raw[MX_MAX_NUM_CHANNELS];
	u8_t map[MX_MAX_NUM_CHANNELS];
	u8_t num_channels;
};
*/



