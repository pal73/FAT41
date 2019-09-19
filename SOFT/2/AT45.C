#include "at45.h"
#include <stm32f10x_lib.h>
#include "main.h"
#include "SPI_STM32f103.h"

//-----------------------------------------------
void df_port_cs_en ( void )
{
GPIOB->ODR&=~(1<<8);
}

//-----------------------------------------------
void df_port_cs_dis ( void )
{
GPIOB->ODR|=(1<<8);
}

//-----------------------------------------------
char df_spi (char outb) 
{
while (!(SPI1->SR & TXE));
SPI1->DR = outb;

while (!(SPI1->SR & RXNE));
return (SPI1->DR);

}


//-----------------------------------------------
void df_spi_init ( void )
{
/* Enable clock for SPI, GPIOA, GPIOC and AFIO. */
RCC->APB2ENR |= 0x00001015;

/* Reset SPI remap (use PA4..PA7). */
AFIO->MAPR   &= 0xFFFFFFFE;

/* SPI1_NSS is GPIO, output set to high. */
GPIOA->BSRR = 0x00000010;

/* SPI1_SCK, SPI1_MISO, SPI1_MOSI are SPI pins. */
GPIOA->CRL &= ~0xFFFF0000;
GPIOA->CRL |=  0xB8B30000;

GPIOB->CRL &= ~0xf0000000;
GPIOB->CRL |=  0x30000000;

SPI1->CR1  = 0x0344+3;
SPI1->CR2  = 0x0000;


}

//-----------------------------------------------
char DF_status_read ( void )
{
char d0;
df_spi_init();

df_port_cs_en();
df_spi(0xd7);
d0=df_spi(0xff);
df_port_cs_dis(); 
return d0;
}

//-----------------------------------------------
long DF_mf_dev_read(void)
{
char d0,d1,d2,d3;
df_spi_init();

df_port_cs_en();
df_spi(0xaf);
d0=df_spi(0xff);
d1=df_spi(0xff);
d2=df_spi(0xff);
d3=df_spi(0xff);  
df_port_cs_dis(); 
return  *((long*)&d0);
}

//-----------------------------------------------
void DF_page_to_buffer(char buff,unsigned page_addr)
{
char d0;
df_spi_init();

if(buff==1)d0=0x53; 
else d0=0x55;

df_port_cs_en();
df_spi(d0);
df_spi(*(((char*)&page_addr)+1));
df_spi(*((char*)&page_addr));
df_spi(0xff);
df_port_cs_dis(); 
}

//-----------------------------------------------
void DF_buffer_to_page_er(char buff,unsigned page_addr)
{
char d0;
df_spi_init();

if(buff==1)d0=0x83; 
else d0=0x86;


df_port_cs_en();
df_spi(d0);
df_spi(*(((char*)&page_addr)+1));
df_spi(*((char*)&page_addr));
df_spi(0xff);
df_port_cs_dis(); 
}

//-----------------------------------------------
void DF_buffer_to_page(char buff,unsigned page_addr)
{
char d0;
df_spi_init();

if(buff==1)d0=0x88; 
else d0=0x89;

df_port_cs_en();
df_spi(d0);
df_spi(*(((char*)&page_addr)+1));
df_spi(*((char*)&page_addr));
df_spi(0xff);
df_port_cs_dis(); 
}

//-----------------------------------------------
void DF_buffer_read(char buff,unsigned buff_addr,unsigned len, char* adr)
{
unsigned i;
char d0;
df_spi_init();

if(buff==1)d0=0x54; 
else d0=0x56;

df_port_cs_en();
df_spi(d0);
df_spi(0xff);
df_spi(*(((char*)&buff_addr)+1));
df_spi(*((char*)&buff_addr));
df_spi(0xff);
for(i=0;i<len;i++)
	{
	adr[i]=df_spi(0xff);
	}
df_port_cs_dis(); 
}

//-----------------------------------------------
void DF_buffer_write(char buff,unsigned buff_addr,unsigned len, char* adr)
{
unsigned i;
char d0;
df_spi_init();

if(buff==1)d0=0x84; 
else d0=0x87;

df_port_cs_en();
df_spi(d0);
df_spi(0xff);
df_spi(*(((char*)&buff_addr)+1));
df_spi(*((char*)&buff_addr));

for(i=0;i<len;i++)
	{
	df_spi(adr[i]);
	}
df_port_cs_dis(); 
}


//-----------------------------------------------
void DF_memo_to_256(void)
{
df_spi_init();
df_port_cs_en();
df_spi(0x3d);
df_spi(0x2a); 
df_spi(0x80);
df_spi(0xa6);
df_port_cs_dis(); 
}  
