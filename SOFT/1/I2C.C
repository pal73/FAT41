#include "i2c.h"
#include <stm32f10x_lib.h>
#include "main.h"

//-----------------------------------------------
void i2c_init(void)
{

GPIOB->CRL&=~0x000000ff;
GPIOB->CRL|= 0x00000055;

RCC->APB1ENR|=(1<<3);
GPIOB->ODR|=(1<<0)|(1<<1);

}


//-----------------------------------------------
void i2c_Start(void)
{
SDA_LOW()
SCL_HIGH()
delay_us(5);
SCL_LOW()
delay_us(5);
return;
}

//-----------------------------------------------
void i2c_Restart(void)
{
SCL_LOW()                      
SDA_HIGH()                     
delay_us(5);
SCL_HIGH()
delay_us(5);
SDA_LOW()                      
delay_us(5);
return;
}    

//-----------------------------------------------
signed char i2c_ReadAcknowledge(void)
{
unsigned char ack;
SDA_HIGH()
SCL_LOW()
delay_us(1);
SCL_HIGH()                              
delay_us(1);		
if(SDA_PIN) ack = 1;
else ack = 0;
SCL_LOW()		
delay_us(1);
return	ack;				
}

//-----------------------------------------------
char i2c_ReadByte(void)
{
unsigned char i;
unsigned char byte = 0;
SDA_HIGH();
for(i=0; i<8; i++)
	{
	SCL_LOW()
	SDA_HIGH();		
	delay_us(1);
	SCL_HIGH();
	delay_us(1);
	byte = byte << 1;		 
	if(SDA_PIN) byte|=1;
	else byte&=0xfe;
	delay_us(1);
	SCL_LOW()
	}
return byte;
}

//-----------------------------------------------
void i2c_SendAcknowledge(unsigned char status)
{
SCL_LOW()
delay_us(3);
if(status&0x01)
	{
     SDA_LOW()                          
     }
else
	{
     SDA_HIGH()
	}
delay_us(1);
SCL_HIGH()			
delay_us(1);
SCL_LOW()
delay_us(1);
return;
}

//-----------------------------------------------
unsigned char i2c_SendByte(unsigned char byte)
{
signed char i;
SCL_LOW()
delay_us(1);


for(i=8; i>=1; i--)
	{
     if(byte&0x80)
         	{
         	SDA_HIGH()
         	}
     else
         	{
         	SDA_LOW()
         	}
     delay_us(1);
     byte<<=1;
     SCL_HIGH()
	delay_us(1);
	SCL_LOW()
	delay_us(1);
 	}

SDA_HIGH()
return 0;
}

//-----------------------------------------------
void i2c_Stop(void)
{
SDA_LOW()                      
SCL_HIGH()
delay_us(1);
SDA_HIGH()                     
delay_us(1);	
SDA_HIGH()
return;
}    

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
