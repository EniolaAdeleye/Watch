// Heartrate driver for Smartwatch

#define MX_I2C_ADDRESS		0x57

#define MX30_REG_INTERRUPT_STATUS1		0x00            // interrupt status 1
#define MX30_REG_INTERRUPT_STATUS2		0x01            // interrupt status 2
#define MX30_REG_INTERRUPT_ENABLE1		0x02            //interrupt enable 1
#define MX30_REG_INTERRUPT_ENABLE2      0x03            // interrupt enable 2
#define MX30_REG_FIFO_WRITE             0x04            //FIFO write pointer
#define MX30_REG_FIFO_OVERFLOW          0x05            //FIFO overflow counter
#define MX30_REG_FIFO_READ              0x06            //FIFO read pointer
#define MX30_REG_FIFO_DATA              0x07            //FIFO data register
#define MX30_REG_FIFO_CONFIGURATION     0x08            //FIFO configuration
#define MX30_REG_MODE_CONFIGURATION     0x09            //mode configuration
#define MX30_REG_SPO2_CONFIGURATION     0x0A            //SpO2 configuration
#define MX30_REG_LED1_PA                0x0C            //led pulse amplitude led 1
#define MX30_REG_LED2_PA                0x0D            //led pulse amplitude led 2
#define MX30_REG_LED3_PA                0x0E            //led pulse amplitude led 3
#define MX30_REG_LED4_PA                0x0F            //led pulse amplitude led 4
#define MX30_REG_MULTI_LED              0x11            //multi_led mode control register
#define MX30_REG_TINT                   0x1F            //die temp integer TINT
#define MX30_REG_TFRAC                  0x20            //die temp fraction  TRAC
#define MX30_REG_TEMP_CONFIGURATION     0x21            //die temp configuration
#define MX30_REG_PROX_INT               0x30
#define MX30_REG_REV_ID                 0xFE
#define MX30_REG_PART_ID                0xFE


