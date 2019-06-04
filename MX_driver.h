// Heartrate driver for Smartwatch
// MAXIM 30101

#define MX_REG_INT_STATUS1		    0x00    //Interrupt status 1
#define MX_REG_INT_STATUS2		    0x01    //Interrupt status 2
#define MX_REG_INT_ENABLE1		    0x02    //Interrupt Enable 1
#define MX_REG_INT_ENABLE2		    0x03    //Interrupt Enable 2
#define MX_REG_FIFO_WR_PTR		    0x04    //FIFO Write Pointer
#define MX_REG_FIFO_OVF_COUNTER		0x05    //Overflow Counter
#define MX_REG_FIFO_RD_PTR		    0x06    //FIFO Read Pointer
#define MX_REG_FIFO_DATA		      0x07    //FIFO Data Register
#define MX_REG_FIFO_CONFIG		    0x08    //FIFO Configuration
#define MX_REG_MODE_CONFIG		    0x09    //Mode Configuration
#define MX_REG_SPO2_CONFIG		    0x0a    //SpO2 Configuration
#define MX_REG_LED1_PA		        0x0c    //LED Pulse Amplitude 1
#define MX_REG_LED2_PA		        0x0d    //LED Pulse Amplitude 2
#define MX_REG_LED3_PA		        0x0e    //LED Pulse Amplitude 3
#define MX_REG_PILOT_PA		        0x10
#define MX_REG_MULTI_LED		      0x11    //Multi-LED Mode Control Register
#define MX_REG_TEMP_INT		        0x1f    //Temp Value_Integer
#define MX_REG_TEMP_FRAC		      0x20    //Temp Value_Fraction
#define MX_REG_TEMP_CONFIG        0x21    //Die Temperature Config
#define MX_REG_PROX_INT		        0x30
#define MX_REG_REV_ID		          0xfe    //Revision ID
#define MX_REG_PART_ID		        0xff    //Part ID







#define MX_I2C_ADDRESS		        0x57
