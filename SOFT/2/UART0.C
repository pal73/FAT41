#include "uart0.h"
#include <stm32f10x_lib.h> 
#include "main.h"
#include "cmd.c"
#include "eeprom_map.h"

//-----------------------------------------------
//УАРТ
char bRXIN2;
char UIB2[100];
char flag2;
char rx_buffer2[RX_BUFFER_SIZE2];
char tx_buffer2[TX_BUFFER_SIZE2];
unsigned short rx_wr_index2,rx_rd_index2,rx_counter2;
unsigned short tx_wr_index2,tx_rd_index2,tx_counter2;
char rx_buffer_overflow2;
char tx2_restart;
char memo_out[50];
char uart_plazma[5];

const char Table87[]={
0x00, 0x0E, 0x1C, 0x12, 0x38, 0x36, 0x24, 0x2A, 0x70, 0x7E, 0x6C, 0x62, 0x48, 0x46, 0x54, 0x5A,
0xE0, 0xEE, 0xFC, 0xF2, 0xD8, 0xD6, 0xC4, 0xCA, 0x90, 0x9E, 0x8C, 0x82, 0xA8, 0xA6, 0xB4, 0xBA,
0xCE, 0xC0, 0xD2, 0xDC, 0xF6, 0xF8, 0xEA, 0xE4, 0xBE, 0xB0, 0xA2, 0xAC, 0x86, 0x88, 0x9A, 0x94,
0x2E, 0x20, 0x32, 0x3C, 0x16, 0x18, 0x0A, 0x04, 0x5E, 0x50, 0x42, 0x4C, 0x66, 0x68, 0x7A, 0x74,
0x92, 0x9C, 0x8E, 0x80, 0xAA, 0xA4, 0xB6, 0xB8, 0xE2, 0xEC, 0xFE, 0xF0, 0xDA, 0xD4, 0xC6, 0xC8,
0x72, 0x7C, 0x6E, 0x60, 0x4A, 0x44, 0x56, 0x58, 0x02, 0x0C, 0x1E, 0x10, 0x3A, 0x34, 0x26, 0x28,
0x5C, 0x52, 0x40, 0x4E, 0x64, 0x6A, 0x78, 0x76, 0x2C, 0x22, 0x30, 0x3E, 0x14, 0x1A, 0x08, 0x06,
0xBC, 0xB2, 0xA0, 0xAE, 0x84, 0x8A, 0x98, 0x96, 0xCC, 0xC2, 0xD0, 0xDE, 0xF4, 0xFA, 0xE8, 0xE6,
0x2A, 0x24, 0x36, 0x38, 0x12, 0x1C, 0x0E, 0x00, 0x5A, 0x54, 0x46, 0x48, 0x62, 0x6C, 0x7E, 0x70,
0xCA, 0xC4, 0xD6, 0xD8, 0xF2, 0xFC, 0xEE, 0xE0, 0xBA, 0xB4, 0xA6, 0xA8, 0x82, 0x8C, 0x9E, 0x90,
0xE4, 0xEA, 0xF8, 0xF6, 0xDC, 0xD2, 0xC0, 0xCE, 0x94, 0x9A, 0x88, 0x86, 0xAC, 0xA2, 0xB0, 0xBE,
0x04, 0x0A, 0x18, 0x16, 0x3C, 0x32, 0x20, 0x2E, 0x74, 0x7A, 0x68, 0x66, 0x4C, 0x42, 0x50, 0x5E,
0xB8, 0xB6, 0xA4, 0xAA, 0x80, 0x8E, 0x9C, 0x92, 0xC8, 0xC6, 0xD4, 0xDA, 0xF0, 0xFE, 0xEC, 0xE2,
0x58, 0x56, 0x44, 0x4A, 0x60, 0x6E, 0x7C, 0x72, 0x28, 0x26, 0x34, 0x3A, 0x10, 0x1E, 0x0C, 0x02,
0x76, 0x78, 0x6A, 0x64, 0x4E, 0x40, 0x52, 0x5C, 0x06, 0x08, 0x1A, 0x14, 0x3E, 0x30, 0x22, 0x2C,
0x96, 0x98, 0x8A, 0x84, 0xAE, 0xA0, 0xB2, 0xBC, 0xE6, 0xE8, 0xFA, 0xF4, 0xDE, 0xD0, 0xC2, 0xCC};



const char Table95[]={
0x00, 0x2A, 0x54, 0x7E, 0xA8, 0x82, 0xFC, 0xD6, 0x7A, 0x50, 0x2E, 0x04, 0xD2, 0xF8, 0x86, 0xAC,
0xF4, 0xDE, 0xA0, 0x8A, 0x5C, 0x76, 0x08, 0x22, 0x8E, 0xA4, 0xDA, 0xF0, 0x26, 0x0C, 0x72, 0x58,
0xC2, 0xE8, 0x96, 0xBC, 0x6A, 0x40, 0x3E, 0x14, 0xB8, 0x92, 0xEC, 0xC6, 0x10, 0x3A, 0x44, 0x6E,
0x36, 0x1C, 0x62, 0x48, 0x9E, 0xB4, 0xCA, 0xE0, 0x4C, 0x66, 0x18, 0x32, 0xE4, 0xCE, 0xB0, 0x9A,
0xAE, 0x84, 0xFA, 0xD0, 0x06, 0x2C, 0x52, 0x78, 0xD4, 0xFE, 0x80, 0xAA, 0x7C, 0x56, 0x28, 0x02,
0x5A, 0x70, 0x0E, 0x24, 0xF2, 0xD8, 0xA6, 0x8C, 0x20, 0x0A, 0x74, 0x5E, 0x88, 0xA2, 0xDC, 0xF6,
0x6C, 0x46, 0x38, 0x12, 0xC4, 0xEE, 0x90, 0xBA, 0x16, 0x3C, 0x42, 0x68, 0xBE, 0x94, 0xEA, 0xC0,
0x98, 0xB2, 0xCC, 0xE6, 0x30, 0x1A, 0x64, 0x4E, 0xE2, 0xC8, 0xB6, 0x9C, 0x4A, 0x60, 0x1E, 0x34,
0x76, 0x5C, 0x22, 0x08, 0xDE, 0xF4, 0x8A, 0xA0, 0x0C, 0x26, 0x58, 0x72, 0xA4, 0x8E, 0xF0, 0xDA,
0x82, 0xA8, 0xD6, 0xFC, 0x2A, 0x00, 0x7E, 0x54, 0xF8, 0xD2, 0xAC, 0x86, 0x50, 0x7A, 0x04, 0x2E,
0xB4, 0x9E, 0xE0, 0xCA, 0x1C, 0x36, 0x48, 0x62, 0xCE, 0xE4, 0x9A, 0xB0, 0x66, 0x4C, 0x32, 0x18,
0x40, 0x6A, 0x14, 0x3E, 0xE8, 0xC2, 0xBC, 0x96, 0x3A, 0x10, 0x6E, 0x44, 0x92, 0xB8, 0xC6, 0xEC, 
0xD8, 0xF2, 0x8C, 0xA6, 0x70, 0x5A, 0x24, 0x0E, 0xA2, 0x88, 0xF6, 0xDC, 0x0A, 0x20, 0x5E, 0x74, 
0x2C, 0x06, 0x78, 0x52, 0x84, 0xAE, 0xD0, 0xFA, 0x56, 0x7C, 0x02, 0x28, 0xFE, 0xD4, 0xAA, 0x80, 
0x1A, 0x30, 0x4E, 0x64, 0xB2, 0x98, 0xE6, 0xCC, 0x60, 0x4A, 0x34, 0x1E, 0xC8, 0xE2, 0x9C, 0xB6, 
0xEE, 0xC4, 0xBA, 0x90, 0x46, 0x6C, 0x12, 0x38, 0x94, 0xBE, 0xC0, 0xEA, 0x3C, 0x16, 0x68, 0x42}; 


//-----------------------------------------------
char crc_87(char* ptr,char num)
{
char r,j/*,lb*/;
r=*ptr;

for(j=1;j<num;j++)
	{
     ptr++;
	r=((*ptr)^Table87[r]);
	}

return r;	
} 

//-----------------------------------------------
char crc_95(char* ptr,char num)
{
char r,j/*,lb*/;
r=*ptr;

for(j=1;j<num;j++)
	{
     ptr++;
	r=((*ptr)^Table95[r]);
	}

return r;	
}


//-----------------------------------------------
void putchar2(char c)
{
while (tx_counter2 == TX_BUFFER_SIZE2);

tx_buffer2[tx_wr_index2]=c;
   
if (++tx_wr_index2 >= TX_BUFFER_SIZE2) tx_wr_index2=0;

//++tx_counter2;

if (tx2_restart) 
	{                               // If transmit interrupt is disabled, enable it
    	tx2_restart = 0;
	USART2->CR1 |= USART_FLAG_TXE;		          // enable TX interrupt
  	}
}

//-----------------------------------------------
void uart_out2 (char num,char data0,char data1,char data2,char data3,char data4,char data5)
{
char i,t=0;
//char *ptr=&data1;
char UOB2[16]; 
UOB2[0]=data0;
UOB2[1]=data1;
UOB2[2]=data2;
UOB2[3]=data3;
UOB2[4]=data4;
UOB2[5]=data5;

for (i=0;i<num;i++)
	{
	t^=UOB2[i];
	}    
UOB2[num]=num;
t^=UOB2[num];
UOB2[num+1]=t;
UOB2[num+2]=END;

for (i=0;i<num+3;i++)
	{
	putchar2(UOB2[i]);
	}   	
}

//-----------------------------------------------
void uart_out_adr0 (char *ptr, char len)
{
char UOB[50]/*={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}*/;
char i,t=0;

for(i=0;i<len;i++)
	{
	UOB[i]=ptr[i];
	t^=UOB[i];
	}
//if(!t)t=0xff;
UOB[len]=len;
t^=len;	
UOB[len+1]=t;	
UOB[len+2]=END;
//UOB[0]=i+1;
//UOB[i]=t^UOB[0];
//UOB[i+1]=END;
	
//puts(UOB); 
for (i=0;i<len+3;i++)
	{
	putchar2(UOB[i]);
	}   
}


//-----------------------------------------------
void uart_out_adr_block (unsigned long adress,char *ptr, char len)
{
//char UOB[100]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char /*i,*/temp11,t=0;
unsigned i11;

t=0;
temp11=CMND;
t^=temp11;
putchar2(temp11);

temp11=10;
t^=temp11;
putchar2(temp11);

temp11=(*((char*)&adress));
t^=temp11;
putchar2(temp11);

temp11=(*(((char*)&adress)+1));
t^=temp11;
putchar2(temp11);

temp11=(*(((char*)&adress)+2));
t^=temp11;
putchar2(temp11);

temp11=(*(((char*)&adress)+3));
t^=temp11;
putchar2(temp11);


for(i11=0;i11<len;i11++)
	{
	temp11=ptr[i11];
	t^=temp11;
	putchar2(temp11);
	}
	
temp11=(len+6);
t^=temp11;
putchar2(temp11);

putchar2(t);

putchar2(0x0a);
	
}

/*
//-----------------------------------------------
void uart0_init(void)
{


SET_REG(PINSEL0,1,0*2,2);
SET_REG(PINSEL0,1,1*2,2);



SET_REG(U0LCR,1,7,1);//U0LCR_bit.DLAB=1;
U0DLL=70000000UL/(BAUD_RATE0*16);
U0DLM=70000000UL/(BAUD_RATE0*16*256);
SET_REG(U0LCR,0,7,1);//U0LCR_bit.DLAB=0;
U0LCR=0x03;
U0FCR=0;

VICProtection = 0;
VICIntEnClr |= (1 << VIC_UART0); 
VICIntSelect &= ~(1 << VIC_UART0);
VICVectAddr9=(unsigned)uart0_interrupt;
VICVectCntl9 = 0x20 | VIC_UART0;
VICIntEnable |= (1  << VIC_UART0);

U0IER=0x03;

}
*/

//-----------------------------------------------
char getchar0(void)
{
char data;
while (rx_counter2==0);
data=rx_buffer2[rx_rd_index2];
if (++rx_rd_index2 == RX_BUFFER_SIZE2) rx_rd_index2=0;
--rx_counter2;
return data;
}



//-----------------------------------------------
void uart_in_an2(void)
{
char temp_char,r1;
int iii;



if(UIB2[0]==CMND)
	{
	
	if(UIB2[1]==1)
		{
		long temp_L; 
		uart_plazma[0]++;
		temp_L=DF_mf_dev_read();
		uart_out2 (6,CMND,1,*((char*)&temp_L),*(((char*)&temp_L)+1),*(((char*)&temp_L)+2),*(((char*)&temp_L)+3));
		}
	else if(UIB2[1]==2)
		{
		char temp;
		
		temp=DF_status_read();
		uart_out2 (3,CMND,2,temp,0,0,0);    
		} 
		
	else if(UIB2[1]==3)
		{
		char temp;
		DF_memo_to_256();
		uart_out2 (2,CMND,3,temp,0,0,0);    
		}				
		
	else if(UIB2[1]==4)
		{
		char temp;
		DF_memo_to_256();
		uart_out2 (2,CMND,3,temp,0,0,0);    
		}				

	else if(UIB2[1]==10)
		{
		char temp;
//		DF_page_to_buffer(2,0);
		
				

		if(UIB2[2]==1)DF_buffer_read(1,0,256, buff);
		else if(UIB2[2]==2)DF_buffer_read(2,0,256, buff);
		
		uart_out_adr_block (0,buff,64);
		for(iii=0;iii<200000;iii++) __nop(); 
		uart_out_adr_block (64,&buff[64],64);
		for(iii=0;iii<200000;iii++) __nop();     
		uart_out_adr_block (128,&buff[128],64);
		for(iii=0;iii<200000;iii++) __nop();    
		uart_out_adr_block (192,&buff[192],64);
		for(iii=0;iii<200000;iii++) __nop();  
		}				

	else if(UIB2[1]==11)
		{
		char temp;
		unsigned i;
//		DF_page_to_buffer(2,0);
		
		for(i=0;i<256;i++)buff[i]=0;

		if(UIB2[2]==1)DF_buffer_write(1,0,256, buff);
		else if(UIB2[2]==2)DF_buffer_write(2,0,256, buff);
		}	
		
	else if(UIB2[1]==12)
		{
		char temp;
		unsigned i;

		
		for(i=0;i<256;i++)buff[i]=0;
		
		if(UIB2[3]==1)
			{
			buff[0]=0x00;
			buff[1]=0x11;
			buff[2]=0x22;
			buff[3]=0x33;
			buff[4]=0x44;
			buff[5]=0x55;
			buff[6]=0x66;
			buff[7]=0x77;
			buff[8]=0x88;
			buff[9]=0x99;
			buff[10]=0;
			buff[11]=0;
			}

		else if(UIB2[3]==2)
			{
			buff[0]=0x00;
			buff[1]=0x10;
			buff[2]=0x20;
			buff[3]=0x30;
			buff[4]=0x40;
			buff[5]=0x50;
			buff[6]=0x60;
			buff[7]=0x70;
			buff[8]=0x80;
			buff[9]=0x90;
			buff[10]=0;
			buff[11]=0;
			}

		else if(UIB2[3]==3)
			{
			buff[0]=0x98;
			buff[1]=0x87;
			buff[2]=0x76;
			buff[3]=0x65;
			buff[4]=0x54;
			buff[5]=0x43;
			buff[6]=0x32;
			buff[7]=0x21;
			buff[8]=0x10;
			buff[9]=0x00;
			buff[10]=0;
			buff[11]=0;
			}
		if(UIB2[2]==1)DF_buffer_write(1,0,256, buff);
		else if(UIB2[2]==2)DF_buffer_write(2,0,256, buff);
		}
		
	else if(UIB2[1]==13)
		{
		char temp;
		unsigned i;
          		
		DF_page_to_buffer(UIB2[2],UIB2[3]);
			
		}					


	else if(UIB2[1]==14)
		{
		char temp;
		unsigned i;
          		
		DF_buffer_to_page_er(UIB2[2],UIB2[3]);
			
		}

	else if(UIB2[1]==20)
		{
		char temp;
		unsigned i;

		file_num=_24c01_read_2byte(EE_FILE_NUM);
		uart_out2 (4,CMND,51,*((char*)&file_num),*(((char*)&file_num)+1),0,0);

		file_lengt=*((long*)&UIB2[2]);
		file_lengt_in_pages=(unsigned)(file_lengt/256);

		

		if(file_num==0)
			{
			current_page_in_memory=0;
			}
		else if(file_num>0)
			{
			current_page_in_memory= _24c01_read_2byte(ADR_EE_FILE[file_num-1])+_24c01_read_2byte(ADR_EE_LEN_FILE[file_num-1]);
			}
		current_byte_in_buffer=0;

		
		_24c01_write_2byte(ADR_EE_FILE[file_num],current_page_in_memory);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE[file_num],file_lengt_in_pages);
		Delay(10);
		_24c01_write_2byte(EE_FILE_NUM,file_num+1);
		Delay(10);
		file_num=_24c01_read_2byte(EE_FILE_NUM);

		

		uart_out2 (4,CMND,52,*((char*)&file_num),*(((char*)&file_num)+1),0,0);

		

		current_page_in_file=0;
		uart_out2 (6,CMND,21,*((char*)&current_page_in_file),*(((char*)&current_page_in_file)+1),*((char*)&current_page_in_memory),*(((char*)&current_page_in_memory)+1));
			
		}
	else if(UIB2[1]==21)
		{
		char temp;
		unsigned i;
          
		//plazma1++;

          for(i=0;i<64;i++)
          	{
          	buff[current_byte_in_buffer+i]=UIB2[2+i];
          	}                                       
          current_byte_in_buffer+=64;
          if(current_byte_in_buffer>=256)
          	{
          	
      /*    	for(i=0;i<256;i++)
          	{
          	buff[i]=(char)i;
          	}  */ 
          	
          	DF_buffer_write(/*//current_buffer*/1,0,256,buff);
          	DF_buffer_to_page_er(/*///current_buffer*/1,current_page_in_memory);
			current_page_in_memory++;
			current_page_in_file++;
			if(current_page_in_file<file_lengt_in_pages)
				{ 
				Delay(10);
				uart_out2 (6,CMND,21,*((char*)&current_page_in_file),*(((char*)&current_page_in_file)+1),*((char*)&current_page_in_memory),*(((char*)&current_page_in_memory)+1));
				current_byte_in_buffer=0;
			    ///	if(current_buffer==1)current_buffer=2;
			    ///	else current_buffer=1;
				}
			else 
				{
				//_24c01_write_2byte(FILE_ADR_0,0);
				Delay(10);
				//_24c01_write_2byte(FILE_LEN_0,current_page);
				Delay(10);
				}
          	}	
          		

			
		}
		
	else if(UIB2[1]==30)
		{
		char temp;
		unsigned i;


		start_play(UIB2[2]);

          
     //     for(i=0;i<256;i++) buff[i]=20/*(char)i*/; 
/*          
          current_page=0;
          last_page=EE_PAGE_LEN-5;
          
          current_buffer_H=2;
          current_buffer_L=2;
          
          DF_page_to_buffer(current_buffer_H,current_page);
          delay_ms(10);
 		DF_buffer_read(current_buffer_L,0,128,buff);
 		delay_ms(10);
		DF_buffer_read(current_buffer_L,128,128,&buff[128]);         
          
          //for(i=0;i<100;i++) buff[i]=240; 
          
          play=1; */ 
          //bSTART=1;
          		
/*		file_lengt=*((long*)&UIB[2]);
		file_lengt_in_pages=(unsigned)(file_lengt/256);
		
		current_byte_in_buffer=0;
		current_buffer=1;
		
		uart_out0 (4,CMND,21,*((char*)&current_page),*(((char*)&current_page)+1),0,0); */
			
		}								
	
	else if(UIB2[1]==40)
		{
		char temp;
		unsigned i;
          
/*      
          DF_page_to_buffer(1,*((unsigned*)&UIB[2]));
          delay_ms(10);
 		DF_buffer_read(1,0,128,buff);
 		delay_ms(10);
		DF_buffer_read(1,128,128,&buff[128]);         
          
		usart_out_adr_block (0,buff,64);
		delay_ms(100);    
		usart_out_adr_block (64,&buff[64],64);
		delay_ms(100);    
		usart_out_adr_block (128,&buff[128],64);
		delay_ms(100);    
		usart_out_adr_block (192,&buff[192],64);
		delay_ms(100);    
          
          play=1;*/
/////		bSTART=1;	
		}
		
	else if(UIB2[1]==51)
		{
		char temp;
		unsigned i;
         
		 		
		_24c01_write_2byte(EE_FILE_NUM,0);
		Delay(10);
		file_num=_24c01_read_2byte(EE_FILE_NUM);

		Delay(10);

		_24c01_write_2byte(ADR_EE_FILE0,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE0,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_FILE1,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE1,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_FILE2,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE2,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_FILE3,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE3,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_FILE4,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE4,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_FILE5,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE5,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_FILE6,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE6,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_FILE7,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE7,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_FILE8,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE8,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_FILE9,0);
		Delay(10);
		_24c01_write_2byte(ADR_EE_LEN_FILE9,0);
		
		Delay(10);

		uart_out2 (4,CMND,51,*((char*)&file_num),*(((char*)&file_num)+1),0,0);
			
		}
		
	else if(UIB2[1]==55) //Просмотр списка ячеек с адресами и длинами (в страницах) файлов
		{
		char temp;
		unsigned i,ii;
         
		
		file_num=_24c01_read_2byte(EE_FILE_NUM);
		uart_out2 (4,CMND,51,*((char*)&file_num),*(((char*)&file_num)+1),0,0);
		Delay(100);
		
		i=_24c01_read_2byte(ADR_EE_FILE0);
		ii=_24c01_read_2byte(ADR_EE_LEN_FILE0);
		uart_out2 (6,CMND,55,*((char*)&i),*(((char*)&i)+1),*((char*)&ii),*(((char*)&ii)+1));
		Delay(100);

		i=_24c01_read_2byte(ADR_EE_FILE1);
		ii=_24c01_read_2byte(ADR_EE_LEN_FILE1);
		uart_out2 (6,CMND,55,*((char*)&i),*(((char*)&i)+1),*((char*)&ii),*(((char*)&ii)+1));
		Delay(100);

		i=_24c01_read_2byte(ADR_EE_FILE2);
		ii=_24c01_read_2byte(ADR_EE_LEN_FILE2);
		uart_out2 (6,CMND,55,*((char*)&i),*(((char*)&i)+1),*((char*)&ii),*(((char*)&ii)+1));
		Delay(100);

		i=_24c01_read_2byte(ADR_EE_FILE3);
		ii=_24c01_read_2byte(ADR_EE_LEN_FILE3);
		uart_out2 (6,CMND,55,*((char*)&i),*(((char*)&i)+1),*((char*)&ii),*(((char*)&ii)+1));
		Delay(100);

		i=_24c01_read_2byte(ADR_EE_FILE4);
		ii=_24c01_read_2byte(ADR_EE_LEN_FILE4);
		uart_out2 (6,CMND,55,*((char*)&i),*(((char*)&i)+1),*((char*)&ii),*(((char*)&ii)+1));
		Delay(100);

		i=_24c01_read_2byte(ADR_EE_FILE5);
		ii=_24c01_read_2byte(ADR_EE_LEN_FILE5);
		uart_out2 (6,CMND,55,*((char*)&i),*(((char*)&i)+1),*((char*)&ii),*(((char*)&ii)+1));
		Delay(100);

		i=_24c01_read_2byte(ADR_EE_FILE6);
		ii=_24c01_read_2byte(ADR_EE_LEN_FILE6);
		uart_out2 (6,CMND,55,*((char*)&i),*(((char*)&i)+1),*((char*)&ii),*(((char*)&ii)+1));
		Delay(100);

		i=_24c01_read_2byte(ADR_EE_FILE7);
		ii=_24c01_read_2byte(ADR_EE_LEN_FILE7);
		uart_out2 (6,CMND,55,*((char*)&i),*(((char*)&i)+1),*((char*)&ii),*(((char*)&ii)+1));
		Delay(100);

		i=_24c01_read_2byte(ADR_EE_FILE8);
		ii=_24c01_read_2byte(ADR_EE_LEN_FILE8);
		uart_out2 (6,CMND,55,*((char*)&i),*(((char*)&i)+1),*((char*)&ii),*(((char*)&ii)+1));
		Delay(100);

		i=_24c01_read_2byte(ADR_EE_FILE9);
		ii=_24c01_read_2byte(ADR_EE_LEN_FILE9);
		uart_out2 (6,CMND,55,*((char*)&i),*(((char*)&i)+1),*((char*)&ii),*(((char*)&ii)+1));
		Delay(100);
			
		}													
	}	

	  




if((UIB2[0]==CMND)&&(UIB2[1]==999))
	{
	unsigned short i;
     unsigned long adress;
     
//	plazma++;

     *((char*)&adress)=UIB2[2];
     *(((char*)&adress)+1)=UIB2[3];
	*(((char*)&adress)+2)=UIB2[4];
	*(((char*)&adress)+3)=UIB2[5];


	///SD_SendCommand(MMC_SET_BLOCKLEN,512);

	///SD_Read512Bytes(adress,memo_out);

	/*for(i=0;i<512;i++)
		{
		memo_out[i]=(char)(i/2);
		}*/


     uart_out_adr_block(adress,memo_out,64);
     
	while (tx_counter2);
	for(i=0;i<10000;i++)  __nop();
     uart_out_adr_block(0x0040UL+adress,&memo_out[64],64);
	while (tx_counter2);  
     for(i=0;i<10000;i++)  __nop();	 
 //    for(i=0;i<5000000;i++)  __nop();
     uart_out_adr_block(0x0080UL+adress,&memo_out[128],64);
	while (tx_counter2);  
     for(i=0;i<10000;i++)  __nop();
     uart_out_adr_block(0x00c0UL+adress,&memo_out[192],64); 
	while (tx_counter2);
     for(i=0;i<10000;i++)  __nop();
     
	uart_out_adr_block(0x0100UL+adress,&memo_out[256],64);
     while (tx_counter2);
	for(i=0;i<10000;i++)  __nop();

     uart_out_adr_block(0x0140UL+adress,&memo_out[320],64); 
     while (tx_counter2);
	for(i=0;i<10000;i++)  __nop();

     uart_out_adr_block(0x0180UL+adress,&memo_out[384],64);  
     while (tx_counter2);
	for(i=0;i<10000;i++)  __nop();

     uart_out_adr_block(0x01c0UL+adress,&memo_out[448],64);
     while (tx_counter2);
	for(i=0;i<10000;i++)  __nop();            
			
	}    
}


/*
//-----------------------------------------------
void uart_in_an0(void)
{
//motor_ind[0]^=0b0001000;

if((UIB0[0]==0x55)&&(UIB0[1]==0x66))
	{
	plazma_uart0++;
	uart_out0(2,0x57,0x66,0,0,0,0);
	}

else if((UIB0[0]==CMND)&&(UIB0[1]==1))
	{
//	adc_buff_out_[0]=UIB1[2]+(UIB1[3]*256);
//	adc_buff_out_[1]=UIB1[4]+(UIB1[5]*256);
	}

}   	
*/

//-----------------------------------------------
signed short index_offset2 (signed short index,signed short offset)
{
index=index+offset;
if(index>=RX_BUFFER_SIZE2) index-=RX_BUFFER_SIZE2; 
if(index<0) index+=RX_BUFFER_SIZE2;
return index;
}

//-----------------------------------------------
char control_check2(signed short index)
{
char i=0,ii=0,iii;

if(rx_buffer2[index]!=END) goto error_cc;

ii=rx_buffer2[index_offset2(index,-2)];
iii=0;
for(i=0;i<=ii;i++)
	{
	iii^=rx_buffer2[index_offset2(index,-2-ii+i)];
	}
if (iii!=rx_buffer2[index_offset2(index,-1)]) goto error_cc;	


success_cc:
return 1;
error_cc:
return 0;

}

//-----------------------------------------------
void uart_in2(void)
{
char temp,i/*,count*/;

//plazma++;

//__disable_irq();

if(rx_buffer_overflow2)
	{
	rx_wr_index2=0;
	rx_rd_index2=0;
	rx_counter2=0;
	rx_buffer_overflow2=0;
	}    
	
if(rx_counter2&&(rx_buffer2[index_offset2(rx_wr_index2,-1)])==END)
	{
	
     temp=rx_buffer2[index_offset2(rx_wr_index2,-3)];
    	if(temp<100) 
    		{
    		if(control_check2(index_offset2(rx_wr_index2,-1)))
    			{
    		
    			rx_rd_index2=index_offset2(rx_wr_index2,-3-temp);
    			for(i=0;i<temp;i++)
				{
				UIB2[i]=rx_buffer2[index_offset2(rx_rd_index2,i)];
				} 
			rx_rd_index2=rx_wr_index2;
			rx_counter2=0;
			
			uart_in_an2();
    			}
 	
    		} 
    	}	


//__enable_irq();     
}  

/*----------------------------------------------------------------------------
  USART1_IRQHandler
  Handles USART1 global interrupt request.
 *----------------------------------------------------------------------------*/
void USART2_IRQHandler (void) 
{
volatile unsigned int IIR;
struct buf_st *p;
char data;

IIR = USART2->SR;		
if (IIR & USART_FLAG_RXNE) 
	{                  // read interrupt
	//plazma_rx++;	
	
     USART2->SR &= ~USART_FLAG_RXNE;	          // clear interrupt

	data=USART2->DR & 0x00ff;

	rx_buffer2[rx_wr_index2]=data;
   	bRXIN2=1;
   	if (++rx_wr_index2 == RX_BUFFER_SIZE2) rx_wr_index2=0;
   	if (++rx_counter2 == RX_BUFFER_SIZE2)
      	{
      	rx_counter2=0;
      	rx_buffer_overflow2=1;
		//plazma_rx++;
      	}
    }

if (IIR & USART_FLAG_TXE) 
	{
     USART2->SR &= ~USART_FLAG_TXE;	          // clear interrupt
	
	if (tx_rd_index2 != tx_wr_index2/*tx_counter2*/)
   		{
   		//--tx_counter2;
   		USART2->DR = tx_buffer2[tx_rd_index2];
		
		if (++tx_rd_index2 >= TX_BUFFER_SIZE2) tx_rd_index2=0;
   		}
      else 
	 	{
        	tx2_restart = 1;
		USART2->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send
		}

	//USART2->CR1 &= ~USART_FLAG_TXE;
    	}
}
