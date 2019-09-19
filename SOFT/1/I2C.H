
#define SCL_HIGH() 	PORT_I2C_SCL->ODR|=(1<<PIN_SCL);
#define SCL_LOW()  	PORT_I2C_SCL->ODR&=~(1<<PIN_SCL);
#define SCL_PIN 	(PORT_I2C_SCL->IDR)&(1<<PIN_SCL)

#define SDA_HIGH() 	PORT_I2C_SDA->ODR|=(1<<PIN_SDA);
#define SDA_LOW()  	PORT_I2C_SDA->ODR&=~(1<<PIN_SDA);
#define SDA_PIN 	(PORT_I2C_SDA->IDR)&(1<<PIN_SDA) 

//-----------------------------------------------
void i2c_init(void);
//-----------------------------------------------
void i2c_Start(void);
//-----------------------------------------------
void i2c_Restart(void);
//-----------------------------------------------
signed char i2c_ReadAcknowledge(void);
//-----------------------------------------------
char i2c_ReadByte(void);
//-----------------------------------------------
void i2c_SendAcknowledge(unsigned char status);
//-----------------------------------------------
unsigned char i2c_SendByte(unsigned char byte);
//-----------------------------------------------
void i2c_Stop(void);



/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/







