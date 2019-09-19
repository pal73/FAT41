#include "main.h"
#include <stm32f10x_lib.h>
#include <RTL.h>                      /* RTL kernel functions & defines      */
//#include <stdio.h>                    /* standard I/O .h-file                */
//#include <ctype.h>                    /* character functions                 */
#include <string.h>                   /* string and memory functions         */
#include "STM32_Init.h"               /* stm32 initialisation                */
#include "common_func.h"
#include "24c01.h"
#include "i2c.h"
#include "SPI_STM32f103.h"
#include "gran.h"
#include "eeprom_map.h"
#include "at45.h"
#include "ibatton.h"
#include "uart0.h"

//-----------------------------------------------
//Таймер
char b1000Hz,b100Hz,b50Hz,b10Hz,b5Hz,b2Hz,b1Hz,b33Hz;
short t0cnt,t0cnt0,t0cnt1,t0cnt2,t0cnt3,t0cnt4,t0cnt5,t0cnt6,t0_cnt7,t0_cnt8;
char bFL5,bFL2,bFL,bFL_;
char strob_cnt;
volatile unsigned long SysTickCnt;

//-----------------------------------------------
//Динамическая индикация
char ind_cnt;
const char IND_CONST[20]={	BIN8(11000000),
						BIN8(11111001),
						BIN8(10100100),
						BIN8(10110000),
						BIN8(10011001),
						BIN8(10010010),
						BIN8(10000010),
						BIN8(11111000),
						BIN8(10000000),
						BIN8(10010000),
						BIN8(10001000),
						BIN8(10000011),
						BIN8(11000110),
						BIN8(10100001),
						BIN8(10000110),
						BIN8(10001110),
						BIN8(11111111),
						BIN8(11111111)};
char ind_out[10]={BIN8(11111111),BIN8(11111111),BIN8(11111111),BIN8(11111111),BIN8(0)}; 
char ind_out_[5];
char dig[5];
char zero_on;
char ind_cnt_;
const short STROB[5]={0xefff,0xdfff,0xbfff,0x7fff,0xffff};


///-----------------------------------------------
//Индикация
stuct_ind a,b[10];
char dig[5];
char zero_on;
const char ABCDEF[]={"0123456789ABCDEF"};
signed char ptr_ind=0;
char mnemo_cnt;
char fl_simv_num,fl_simv_len;
char default_temp;
signed short av_j_si_max,av_j_si_max_;
char simax;
char phase;
const char sm_[]		={"                    "}; //
const char sm_exit[]	={" Выход              "}; //
const char smon[]	={"ВКЛ."}; 
const char smoff[]	={"ВЫК."};
const char sm_mont[13][4]={"упс","янв","фев","мар","апр","май","июн","июл","авг","сен","окт","ноя","дек"}; //
char content[63]; 
signed char parol[5];
short ret_cnt;
char pvlk;

//-----------------------------------------------
//Управление процессом
signed short time_wrk;
signed short demo_per;
signed short demo_len;
signed short current_sound;
signed short wrk_cnt;
signed short max_file;
signed short current_file;
signed short current_demo_file;
signed short main_demo_cnt;

//-----------------------------------------------
//Запоминание таблеток
unsigned int i_batton[10];
short i_batton_num;

//-----------------------------------------------
//Измерение напряжения
signed short Ku;
signed short U;
signed short umin_cnt;

//-----------------------------------------------
//Управление воспроизведением
signed short file_len[10];
char play;
long file_lengt;
unsigned short file_lengt_in_pages,current_page_in_file,current_page_in_memory,last_page,current_byte_in_buffer;
char bSTART,bBUFF_LOAD,bBUFF_READ_L,bBUFF_READ_H,bBUFF_LOAD_L,bBUFF_LOAD_H;
short sample;
signed short sample_cnt;
short sound_buffer[256];
signed short file_num;
signed short sound_imit;

//-----------------------------------------------
//Фискальный счетчик
signed short fisk_cnt;

//-----------------------------------------------
//Управление лампой
char lamp_cnt;
#define LAMP_PERIOD 7 

//**********************************************
//Работа с кнопками 
char but;                            
unsigned long but_n,but_s;
char but0_cnt;
char but1_cnt;
char but_onL_temp;
char speed,l_but,n_but;
char in_drv_cnt,in_stop_drv_cnt;
char bIN,bIN_STOP;

//-----------------------------------------------
//Низкое напряжение
char u_min_cnt=0;
#define U_MIN 106
char bUMIN=0;
char u_min_hndl_cnt;

//**********************************************
//Коэффициенты, отображаемые из ЕЕПРОМ
signed short MAIN_TIME;
signed short DEMO_TIME;
signed short DEMO_PERIOD;
signed short NUM_OF_FILES;
signed short MAIN_VOLUME;
signed short MAIN_CNT;

//-----------------------------------------------
//АЦП
signed short adc_buff[16],adc_buff_;
char adc_cnt;



char plazma_cnt;
char plazma_rx;


//-----------------------------------------------
//Управление воспроизведением музыки
signed short main_music_cnt;
signed short max_main_music_cnt;
signed short fade_out_main_music_cnt;


//-----------------------------------------------
//Управление процессом
//signed short time_wrk;
//signed short demo_per;
//signed short demo_len;
signed short current_sound;
signed short wrk_cnt;
signed short max_file;
signed short current_file;
signed short current_demo_file;
signed short main_demo_cnt;
char strob_cnt51;
//-----------------------------------------------
//Фискальный счетчик
signed short main_cnt;

char plazma,plazma1,plazma2,plazma3=6;
long plazma_long;

unsigned short read_adress;
signed short dac_cnt;


char plazma_cnt;
char buff[1024];


//-----------------------------------------------
void delay_us(long del)
{
long temp;
temp=5*del;

while (--temp);
return;
}

//-----------------------------------------------
void Delay (unsigned long tick) 
{       
unsigned long systickcnt;

systickcnt = SysTickCnt;
while ((SysTickCnt - systickcnt) < tick);
}

//-----------------------------------------------
void dac_init(void)
{
RCC->APB1ENR|=(1<<29);
DAC->CR=0x00000001;
}
/*
//-----------------------------------------------
void dac_drv(void)
{
dac_cnt++;
if(dac_cnt>4000)dac_cnt=0;
DAC->DHR12R1=dac_cnt;
DAC->DHR12R2=(4000-dac_cnt);
}*/


//-----------------------------------------------
void short2indh_sf(unsigned short in, char start, char fl) {
	char i;

	ind_out_[0]=IND_CONST[in%16];
	in/=16;
	ind_out_[1]=IND_CONST[in%16];
	in/=16;
	ind_out_[2]=IND_CONST[in%16];
	in/=16;
	ind_out_[3]=IND_CONST[in%16];

	
	if(((fl==1)&&(bFL)) ||
	   ((fl==2)&&(bFL2)) ||
	   ((fl==5)&&(bFL5))) {
		ind_out_[0]=IND_CONST[17];
		ind_out_[1]=IND_CONST[17];
		ind_out_[2]=IND_CONST[17];
		ind_out_[3]=IND_CONST[17];

	}

	
	ind_out[start]=ind_out_[0];
	ind_out[start+1]=ind_out_[1];
	ind_out[start+2]=ind_out_[2];
	ind_out[start+3]=ind_out_[3];


}


//-----------------------------------------------
void char2indh_sf(unsigned char in, char start, char fl) {
	char i;

	ind_out_[0]=IND_CONST[in%16];
	ind_out_[1]=IND_CONST[in/16];
	
	if(((fl==1)&&(bFL)) ||
	   ((fl==2)&&(bFL2)) ||
	   ((fl==5)&&(bFL5))) {
		ind_out_[0]=IND_CONST[17];
		ind_out_[1]=IND_CONST[17];
	}

	
	ind_out[start]=ind_out_[0];
	ind_out[start+1]=ind_out_[1];



}


//-----------------------------------------------
void wrk_drv(void)
{
GPIOA->CRL&=~0x0000f000;
GPIOA->CRL|=0x00002000;
if(wrk_cnt)
	{
	wrk_cnt--;
	GPIOA->ODR|=1<<3;
	main_demo_cnt=0;
	}
else
	{
	GPIOA->ODR&=~(1<<3);
	}


if(wrk_cnt==((MAIN_TIME-5)*10))
	{
	MAIN_CNT++;
	gran_ring(&MAIN_CNT,0,9999);
	_24c01_write_2byte(EE_MAIN_CNT,MAIN_CNT);
	}
}

//-----------------------------------------------
void adc_init(void)
{
//int temp_S;
//char i;
 
RCC->APB2ENR |= (1<<9);                      /* enable periperal clock for ADC1      */

ADC1->SQR1  = 0x00000000;                    /* 1 conversions                    */
ADC1->SQR3  = (10<<0);     				/* chn10  */
ADC1->SMPR1 = 5;     					/* set sample time (55,5 cycles)        */ 

ADC1->CR1   =  0x00000100;                   /* use independant mode, SCAN mode      */
ADC1->CR2   =  0x000E0003;                   /* data align right, cont. conversion   */
                                               /* EXTSEL = SWSTART                     */ 
                                               /* enable ADC, DMA mode                 */
ADC1->CR2  |=  0x00500000;                   /* start SW conversion                  */

}

//-----------------------------------------------
void rele_hndl(void)
{

if(wrk_cnt)
	{
	GPIOB->ODR|=(1<<7);
	}
else
	{
	GPIOB->ODR&=~(1<<7);
	}

 
}

//-----------------------------------------------
void adc_drv(void)
{
int temp_S;
char i;

adc_buff[adc_cnt]=(signed short)(ADC1->DR);
temp_S=0;
for(i=0;i<16;i++) temp_S+=adc_buff[i];
adc_buff_=temp_S>>4;
adc_cnt++;
if(adc_cnt>=16)adc_cnt=0;


temp_S=(int)adc_buff_;
temp_S*=(int)_24c01_read_2byte(EE_KU);
temp_S/=3000;

U=(signed short)temp_S;

}



//-----------------------------------------------
void in_drv(void)
{
GPIOB->CRH&=~0x000ff000;
GPIOB->CRH|=0x00088000;
GPIOB->ODR|=(1<<11)|(1<<12);

if(!(GPIOB->IDR&(1<<11)))
	{
	if(in_drv_cnt<10)
		{
		in_drv_cnt++;
		if(in_drv_cnt>=10)
			{
			bIN=1;
			}
		}
	}
else 
	{
	in_drv_cnt=0;
	}

if(!(GPIOB->IDR&(1<<12)))
	{
	if(in_stop_drv_cnt<10)
		{
		in_stop_drv_cnt++;
		if(in_stop_drv_cnt>=10)
			{
			bIN_STOP=1;
			}
		}
	}
else 
	{
	in_stop_drv_cnt=0;
	}

}

//-----------------------------------------------
void u_min_hndl(void)
{
if(!wrk_cnt)
	{
	u_min_cnt=0;
	}
else if(U<U_MIN)
	{
	u_min_cnt++;
	if(u_min_cnt>=5)
		{
		u_min_cnt=5;
		bUMIN=1;
		}
	}

if((wrk_cnt)||(!bUMIN))
	{
	u_min_hndl_cnt=0;
	}
else
	{
	u_min_hndl_cnt++;
	if(u_min_hndl_cnt>=30)
		{
		u_min_hndl_cnt=0;
		start_play(_LOW_BATTERY_);
		}
	}
}

//-----------------------------------------------
void in_an(void)
{
if(bIN)
	{
	bIN=0;

	wrk_cnt= MAIN_TIME*10;
	current_file++;
	gran_ring(&current_file,1,NUM_OF_FILES);
	//music_start(current_file,MAIN_TIME+10,1,5,MAIN_VOLUME,1,10);
	//ind=iMn;
	

	}

if(bIN_STOP)
	{
	bIN_STOP=0;

	wrk_cnt=0;
//	music_stop();
	}
}


//-----------------------------------------------
void bcd2ind_zero()
{
char i;
zero_on=1;
for (i=4;i>0;i--)
	{
	if(zero_on&&(!dig[i-1])&&(i-1))
		{
		ind_out_[i-1]=IND_CONST[17];
		}
	else
		{
		ind_out_[i-1]=IND_CONST[dig[i-1]];
		zero_on=0;
		}
	}
}

//-----------------------------------------------
void bcd2ind()
{
char i;

for (i=4;i>0;i--)
	{
	ind_out_[i-1]=IND_CONST[dig[i-1]];
	}
}

//-----------------------------------------------
void int2ind_slkuf(unsigned short in, char start, char len, char komma, char unzero, char fl)
{
char i;
//char n;
bin2bcd_int(in);
if(unzero)bcd2ind_zero();
else bcd2ind();
if(komma)ind_out_[komma]&=BIN8(1111111); 
if(fl&&bFL5)
	{
	for(i=0;i<len;i++)
		{
		ind_out_[i]=IND_CONST[17];
		}
	}

for(i=0;i<len;i++)
		{
		ind_out[start+i]=ind_out_[i];
		}
if(len==0)
	{
	ind_out[0]=0xff;
	ind_out[1]=0xff;
	ind_out[2]=0xff;
	ind_out[3]=0xff;
	}

}


//-----------------------------------------------
void ind_hndl(void)
{
if(ind_cnt_)ind_cnt_--;
if(ind==iMn)
	{
	if(wrk_cnt)int2ind_slkuf(wrk_cnt,0,4,0,1,0);
	else 
		{
		ind_out[0]=0x7f;
		ind_out[1]=0xff;
		ind_out[2]=0xff;
		ind_out[3]=0xff;
		}

	if(bFL)
		{
		//char2indh_sf(_24c01_read_2byte(202),2,0);
		//char2indh_sf(_24c01_read_2byte(200),0,0);
		//short2indh_sf((short)i_batton[1],0,0);
		}
	else
		{
		//char2indh_sf(_24c01_read_2byte(204),2,0);
		//char2indh_sf(_24c01_read_2byte(206),0,0);
		//short2indh_sf((short)(i_batton[1]>>16),0,5);
		}
		
	//char2indh_sf(res[4],2,0);
	//char2indh_sf(res[3],0,0);

	//ibatton_polling();
	//int2ind_slkuf(i_batton_num,0,4,0,0,0);
	//int2ind_slkuf(_24c01_read_2byte(ADR_EE_IBATTON[0]+2),0,4,0,0,0);
	//int2ind_slkuf(adc_buff_,0,4,0,0,0);
	//int2ind_slkuf(ibatton_n_,1,1,0,0,5);
	//int2ind_slkuf(ibatton_n,0,1,0,0,0);
	//int2ind_slkuf(/*rx_wr_index2*/uart_plazma[0],0,4,0,0,0);
	
	//int2ind_slkuf(ibatton_repeat_cnt,0,3,0,0,0);
	//int2ind_slkuf(ibatton_n,3,1,0,0,0);
	//int2ind_slkuf(/*_24c01_read_2byte(ADR_EE_LEN_FILE0)*/current_page_in_memory,0,4,0,0,0);
	} 
else if(ind==iSet_sel)
	{
	int2ind_slkuf(sub_ind,0,4,0,1,1);
	}

else if(ind==iIbatton_prl)
	{
	int2ind_slkuf(parol[0],3,1,0,1,(sub_ind==0));
	int2ind_slkuf(parol[1],2,1,0,1,(sub_ind==1));
	int2ind_slkuf(parol[2],1,1,0,1,(sub_ind==2));
	int2ind_slkuf(parol[3],0,1,0,1,(sub_ind==3));
	}

else if(ind==iSet_time)
	{
	int2ind_slkuf(1,3,1,0,1,1);
	int2ind_slkuf(time_wrk,0,3,0,1,0);
	}

else if(ind==iIbatton)
	{
	int2ind_slkuf(3,3,1,0,1,1);
	if(sub_ind==0)
		{
		int2ind_slkuf(_24c01_read_2byte(EE_IBATTON_NUM),0,3,0,1,1);

		}
	else if(sub_ind==1)
		{
		int2ind_slkuf(_24c01_read_2byte(EE_IBATTON_NUM),0,3,0,1,0);

		}
	}

else if(ind==iK_u)
	{
	int2ind_slkuf(4,3,1,0,1,1);
	int2ind_slkuf(U,0,3,1,1,0);
	}


else if(ind==iSetFile)
	{
	int2ind_slkuf(max_file,0,3,0,1,0);
	}

else if(ind==iFisk_prl)
	{
	int2ind_slkuf(file_len[0],0,4,0,1,0);
	/*int2ind_slkuf(parol[0],3,1,0,1,(sub_ind==0));
	int2ind_slkuf(parol[1],2,1,0,1,(sub_ind==1));
	int2ind_slkuf(parol[2],1,1,0,1,(sub_ind==2));
	int2ind_slkuf(parol[3],0,1,0,1,(sub_ind==3));*/
	}

else if(ind==iFisk)
	{
	if(sub_ind==0)
		{
		ind_out[0]=0xbf;
		ind_out[1]=0xbf;
		ind_out[2]=0xbf;
		ind_out[3]=0xbf;
		}
	else if(sub_ind==1)
		{
		ind_out[0]=0xff;
		ind_out[1]=0xbf;
		ind_out[2]=0xbf;
		ind_out[3]=0xbf;
		}
	else if(sub_ind==2)
		{
		ind_out[0]=0xff;
		ind_out[1]=0xff;
		ind_out[2]=0xbf;
		ind_out[3]=0xbf;
		}
	else if(sub_ind==3)
		{
		ind_out[0]=0xff;
		ind_out[1]=0xff;
		ind_out[2]=0xff;
		ind_out[3]=0xbf;
		}
	else if(sub_ind==4)
		{
		int2ind_slkuf(fisk_cnt,0,4,0,0,0);
		}
	

	}

else if(ind==iDemo_per)
	{
	int2ind_slkuf(demo_per,0,3,0,1,0);
	}

else if(ind==iDemo_len)
	{
	int2ind_slkuf(demo_len,0,3,0,1,0);
	}

else if(ind==iTest)
	{
	int2ind_slkuf(sub_ind,0,3,0,1,0);
	}

else if(ind==iDeb)
	{
	int2ind_slkuf(sub_ind,3,1,0,1,1);
	//i_batton[0]=0x12345678;
	//i_batton[1]=0x43218765;
	//i_batton[2]=0x87654321;
	if(sub_ind==0)int2ind_slkuf(_24c01_read_2byte(EE_IBATTON_NUM),0,3,0,1,0);
	else if(sub_ind==1)int2ind_slkuf((char)((i_batton[0])&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==2)int2ind_slkuf((char)((i_batton[0]>>8)&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==3)int2ind_slkuf((char)((i_batton[0]>>16)&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==4)int2ind_slkuf((char)((i_batton[0]>>24)&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==5)int2ind_slkuf((char)((i_batton[1])&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==6)int2ind_slkuf((char)((i_batton[1]>>8)&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==7)int2ind_slkuf((char)((i_batton[1]>>16)&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==8)int2ind_slkuf((char)((i_batton[1]>>24)&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==9)int2ind_slkuf((char)((i_batton[2])&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==10)int2ind_slkuf((char)((i_batton[2]>>8)&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==11)int2ind_slkuf((char)((i_batton[2]>>16)&0x000000ffUL),0,3,0,1,0);
	else if(sub_ind==12)int2ind_slkuf((char)((i_batton[2]>>24)&0x000000ffUL),0,3,0,1,0);

	}	
//int2ind_slkuf(ibatton_repeat_cnt,0,2,0,1,0);
//int2ind_slkuf(ibatton_an_plazma,3,3,0,1,0);
//int2ind_slkuf(ibatton_an_plazma_,2,1,0,1,0);
}


#define BUT0	16
#define BUT1	17
#define BUT2	18
#define BUT3	19
#define BUT4	20   
#define BUT_MASK (1UL<<BUT0)|(1UL<<BUT1)|(1UL<<BUT2)|(1UL<<BUT3)|(1UL<<BUT4)

#define BUT_ON 10
#define BUT_ONL 30 

#define butLR   126
#define butLR_  118
#define butL   127
#define butL_  119
#define butR   254
#define butR_  246
#define butE   223
#define butE_  215






//-----------------------------------------------
void but_drv(void)
{
char i;

GPIOC->CRL &= 	0xFF0FFFFF;
GPIOC->CRL |=  0x00800000;
GPIOC->CRH &= 	0xFFF0F0FF;
GPIOC->CRH |=  0x00080800;
GPIOC->ODR |=  (1<<5)|(1<<10)|(1<<12);

for(i=0;i<100;i++)
{
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
}

but_n=((GPIOC->IDR)>>5)|0xffffff5e; 	

if((but_n==0xffffffffUL)||(but_n!=but_s))
 	{
 	speed=0;
 
   	if (((but0_cnt>=BUT_ON)||(but1_cnt!=0))&&(!l_but))
  		{
   	     n_but=1;
          but=(char)but_s;

          }
   	if (but1_cnt>=but_onL_temp)
  		{
   	     n_but=1;
 
          but=((char)but_s)&0xf7;
          }
    	l_but=0;
   	but_onL_temp=BUT_ONL;
    	but0_cnt=0;
  	but1_cnt=0;          
     goto but_drv_out;
  	}
if(but_n==but_s)
 	{
  	but0_cnt++;
  	if(but0_cnt>=BUT_ON)
  		{
   		but0_cnt=0;
   		but1_cnt++;
   		if(but1_cnt>=but_onL_temp)
   			{              
    			but=(char)(but_s&0xf7);
    			but1_cnt=0;
    			n_but=1;
    			     
    			l_but=1;
			if(speed)
				{
    				but_onL_temp=but_onL_temp>>1;
        			if(but_onL_temp<=2) but_onL_temp=2;
				}    
   			}
  		}
 	}
but_drv_out: 
but_s=but_n;
	   
}


//-----------------------------------------------
void start_play(char in)
{
current_page_in_memory=_24c01_read_2byte(ADR_EE_FILE[in])+2;
last_page=current_page_in_memory+_24c01_read_2byte(ADR_EE_LEN_FILE[in])-4;
          
DF_page_to_buffer(1,current_page_in_memory);
Delay(10);
DF_buffer_read(1,0,256,(char*)sound_buffer);
Delay(10);
			
DF_page_to_buffer(1,current_page_in_memory+1);
Delay(10);
DF_buffer_read(1,0,256,(char*)&sound_buffer[128]);         
          
play=1;

}

//-----------------------------------------------
void lamp_drv(void)
{
if(++lamp_cnt>=LAMP_PERIOD)	lamp_cnt=0;
if(wrk_cnt>30)lamp_cnt=0;

if((lamp_cnt>=LAMP_PERIOD/2)&&(wrk_cnt)&&(wrk_cnt<=30)) 
	{
	GPIOB->ODR|=(1<<9);
	}
else
	{
	GPIOB->ODR&=~(1<<9);
	}

}

//-----------------------------------------------
void process_hndl(void)
{
if(wrk_cnt)wrk_cnt--;

if(wrk_cnt==(time_wrk-30))
	{
	fisk_cnt++;
	gran_ring(&fisk_cnt,0,1000);
	_24c01_write_2byte(EE_FISK_CNT,fisk_cnt);
	}

if((wrk_cnt)&&(wrk_cnt<=20)&&(((wrk_cnt-3)%8)==0)) start_play(0);
}

//-----------------------------------------------
void but_an(void)
{
//signed short temp_SS; 
//char r1;



//watchdog_reset();

if(!n_but)goto but_an_end;
if(ind==iMn)
	{
	if(but==butE_)
		{
		ind=iSet_sel;
		sub_ind=1;
		}
	if(but==butE)
		{
		//_24c01_write_2byte(200,0x90ab);
		//_24c01_write_4byte(ADR_EE_IBATTON[0]+2,0x1235);
		}
	/*if(but==butE_)
		{
		_24c01_write_2byte(202,13);
		//_24c01_write_4byte(ADR_EE_IBATTON[0]+2,0x1235);
		}*/
	if(but==butL)
		{
		//_24c01_write_4byte(200,0xabcd12ef);
		//_24c01_write_4byte(ADR_EE_IBATTON[0]+2,0x1235);
		}
	if(but==butLR)
		{
		ind=iDeb;
		sub_ind=0;
		}

  	}
else if(ind==iDeb)
	{
	if(but==butLR)
		{
		ind=iMn;
		sub_ind=0;
		}
	if(but==butR)
		{
		sub_ind++;
		gran_char(&sub_ind,0,12);
		}
	if(but==butL)
		{
		sub_ind--;
		gran_char(&sub_ind,0,12);
		}
  	}
else if(ind==iSet_sel)
	{
	if(but==butR)
		{
		sub_ind++;
		gran_char(&sub_ind,1,6);
		}
	else if(but==butL)
		{
		sub_ind--;
		gran_char(&sub_ind,1,6);
		}
	else if(sub_ind==1)
		{
		if(but==butE_)
			{
			ind=iSet_time;
			}
		}
	else if(sub_ind==3)
		{
		if(but==butE_)
			{
			ind=iIbatton_prl;
			parol[0]=0;
			parol[1]=0;
			parol[2]=0;
			parol[3]=0;
			}
		}
	else if(sub_ind==4)
		{
		if(but==butE_)
			{
			ind=iK_u;
			ret_cnt=300;
			}
		}
	else if(sub_ind==6)
		{
		if(but==butE_)
			{
			ind=iMn;
			}
		}
	}

else if(ind==iSet_time)
	{
	ret_cnt=1000;
	if(but==butR)
		{
		time_wrk=((time_wrk/10)+1)*10;
		gran(&time_wrk,30,600);
		_24c01_write_2byte(EE_TIME_WRK,time_wrk);
		}
	else if(but==butR_)
		{
		time_wrk=((time_wrk/10)+1)*10;
		gran(&time_wrk,30,600);
		_24c01_write_2byte(EE_TIME_WRK,time_wrk);
		speed=1;
		}
	else if(but==butL)
		{
		time_wrk=((time_wrk/10)-1)*10;
		gran(&time_wrk,30,600);
		_24c01_write_2byte(EE_TIME_WRK,time_wrk);
		speed=1;
		}
	else if(but==butL_)
		{
		time_wrk=((time_wrk/10)-1)*10;
		gran(&time_wrk,30,600);
		_24c01_write_2byte(EE_TIME_WRK,time_wrk);
		speed=1;
		}
	else if(but==butE_)
		{
		ind=iSet_sel;
		sub_ind=1;
		}
	}

else if(ind==iIbatton_prl)
	{
	ret_cnt=1000;
	if(but==butR)
		{
		sub_ind++;
		gran_ring_char(&sub_ind,0,3);
		}
	else if(but==butL)
		{
		sub_ind--;
		gran_ring_char(&sub_ind,0,3);
		}
	else if(but==butE)
		{
		parol[sub_ind]++;
		gran_ring_char(&parol[sub_ind],0,9);
		}
	else if(but==butE_)
		{
		signed short temp_SS;

		temp_SS=0;

		temp_SS+=(signed short)(parol[3]);
		temp_SS+=(signed short)(parol[2]*10);
		temp_SS+=(signed short)(parol[1]*100);
		temp_SS+=(signed short)(parol[0]*1000);

		if(temp_SS==PAROL_ALL_IBATTON_ERASE)
		  	{
			ind=iIbatton;
			sub_ind=1;
			ret_cnt=30;
			_24c01_write_2byte(EE_IBATTON_NUM,0);
			_24c01_write_4byte(ADR_EE_IBATTON[0],0);
			_24c01_write_4byte(ADR_EE_IBATTON[1],0);
			_24c01_write_4byte(ADR_EE_IBATTON[2],0);
			_24c01_write_4byte(ADR_EE_IBATTON[3],0);
			_24c01_write_4byte(ADR_EE_IBATTON[4],0);
			_24c01_write_4byte(ADR_EE_IBATTON[5],0);
			_24c01_write_4byte(ADR_EE_IBATTON[6],0);
			_24c01_write_4byte(ADR_EE_IBATTON[7],0);
			_24c01_write_4byte(ADR_EE_IBATTON[8],0);
			_24c01_write_4byte(ADR_EE_IBATTON[9],0);
			//start_play(_ONE_BEEP_);
			}

		else if(temp_SS==PAROL_IBATTON_WRITE)
		  	{
			ind=iIbatton;
			sub_ind=0;
			ret_cnt=100;
			}

		else if(temp_SS==PAROL_IBATTON_VIEW)
		  	{
			ind=iIbatton;
			sub_ind=1;
			ret_cnt=100;
			}

		else if(temp_SS==PAROL_IBATTON_PVLK)
		  	{					
			ind=iIbatton;
			sub_ind=0;
			ret_cnt=500;
			pvlk=1;
			_24c01_write_2byte(EE_IBATTON_NUM,0);
			_24c01_write_4byte(ADR_EE_IBATTON[0],0);
			_24c01_write_4byte(ADR_EE_IBATTON[1],0);
			_24c01_write_4byte(ADR_EE_IBATTON[2],0);
			_24c01_write_4byte(ADR_EE_IBATTON[3],0);
			_24c01_write_4byte(ADR_EE_IBATTON[4],0);
			_24c01_write_4byte(ADR_EE_IBATTON[5],0);
			_24c01_write_4byte(ADR_EE_IBATTON[6],0);
			_24c01_write_4byte(ADR_EE_IBATTON[7],0);
			_24c01_write_4byte(ADR_EE_IBATTON[8],0);
			_24c01_write_4byte(ADR_EE_IBATTON[9],0);
		//start_play(_ONE_BEEP_);
			}
		}
	}


else if(ind==iIbatton)
	{
	if(but==butE_)
		{
		ind=iSet_sel;
		sub_ind=3;
		ret_cnt=100;
		}
	}
else if(ind==iK_u)
	{
	ret_cnt=300;
	if(but==butR)
		{
		Ku++;
		gran(&Ku,100,200);
		_24c01_write_2byte(EE_KU,Ku);
		}
	else if(but==butR_)
		{
		Ku+=2;
		gran(&Ku,100,200);
		_24c01_write_2byte(EE_KU,Ku);
		speed=1;
		}
	else if(but==butL)
		{
		Ku--;
		gran(&Ku,100,200);
		_24c01_write_2byte(EE_KU,Ku);
		}
	else if(but==butL_)
		{
		Ku-=2;
		gran(&Ku,100,200);
		_24c01_write_2byte(EE_KU,Ku);
		speed=1;
		}
	else if(but==butE_)
		{
		ind=iSet_sel;
		sub_ind=4;
		}
	}			
but_an_end:
n_but=0;
}




//***********************************************
//***********************************************
//***********************************************
//***********************************************
//***********************************************
void SysTick_Handler (void) 
{

sound_imit+=200;
if(sound_imit>1000)sound_imit=-1000;
if(play)
	{
	DAC->DHR12L1=sample;
	//GPIOB->ODR^=(1<<11);
	sample_cnt++;
	if(sample_cnt>=256)
		{
		sample_cnt=0;
		//GPIOB->ODR^=(1<<11);
		}
	
	sample=((sound_buffer[sample_cnt])<<4)+0x8000-(500<<4);
	
	if(sample_cnt==5) 
		{
		bBUFF_LOAD_H=1;
		//GPIOB->ODR^=(1<<11);
		}
	if(sample_cnt==30) bBUFF_READ_H=1;	
   	if(sample_cnt==133) bBUFF_LOAD_L=1;
    	if(sample_cnt==158) bBUFF_READ_L=1; 



   	}
/*!!!
else if(bZV_ERROR)
	{
	audio_out_cnt++;
	if(audio_out_cnt>=256)
		{
		audio_out_cnt=0;
		}
	     
	sound_buffer_=sound_buffer[audio_out_cnt]+0x200;
	
	//if((audio_out_cnt/16)&0x01) PWMMR6=900;
	//else PWMMR6=100;
	PWMMR6=sound_buffer_;
	PWMMR4=sound_buffer_ ;
	    
     if(audio_out_cnt==1)
     	{
         	b1ERR=1;

     	}
	}*/	
else
	{
	DAC->DHR12L1=0x8000;//+(sound_imit<<4);
	}



if(++t0cnt4>=22)
     {
	SysTickCnt++;
	
	
     t0cnt4=0;
     b1000Hz=1;
	
	if(++t0cnt5>=20)
     	{
     	t0cnt5=0;
     	b50Hz=1;
     	}
     
	if(++t0cnt>=10)
     	{
     	t0cnt=0;
     	b100Hz=1;

     	if(++t0cnt0>=10)
	     	{
	     	t0cnt0=0;
	     	b10Hz=1;
		     }

     	if(t0cnt0==5)
	     	{
		    	}

     	if(++t0cnt1>=20)
	     	{
	     	t0cnt1=0;
	     	b5Hz=1;
			if(bFL5)bFL5=0;
			else bFL5=1;     
	     	}

     	if(++t0cnt2>=50)
	     	{
	     	t0cnt2=0;
	     	b2Hz=1;
			if(bFL2)bFL2=0;
			else bFL2=1;
	     	}         

     	if(++t0cnt3>=100)
	     	{
	     	t0cnt3=0;
	     	b1Hz=1;
			if(bFL)bFL=0;
			else bFL=1;

		     }
     	}
	strob_cnt++;
	if(strob_cnt>=6)
		{
		strob_cnt=0;
		}
	//SET_REG(LPC_GPIO2->FIOPIN,0xff,2,8);
	GPIOB->CRH=((GPIOB->CRH)&0x0000ffff)|0x77770000;
	GPIOB->ODR|=(1<<12)|(1<<13)|(1<<14)|(1<<15);
	//SET_REG(LPC_GPIO2->FIODIR,0xff,2,8);
	//SET_REG(LPC_GPIO1->FIOPIN,0x0f,22,4);
	//SET_REG(LPC_GPIO1->FIODIR,0x0f,22,4);
	GPIOC->CRL=((GPIOC->CRL)&0x000fffff)|0x33300000;
	GPIOC->CRH=((GPIOC->CRH)&0xfff00000)|0x00033333;
	GPIOC->ODR|=(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)|(1<<10)|(1<<11)|(1<<12);
	

	if(strob_cnt<=3)
		{
		//SET_REG(LPC_GPIO1->FIOPIN,~(1<<strob_cnt),22,4);
		GPIOB->ODR&=STROB[strob_cnt];///*~((1<<12)|(1<<13)|(1<<14)|(1<<15));//*/~(1<<(12+strob_cnt));
		//SET_REG(LPC_GPIO2->FIOPIN,ind_out[strob_cnt],2,8);
		GPIOC->ODR&=((ind_out[strob_cnt])<<5)|0xe01f;
		}
	else	if(strob_cnt==4)
		{
		but_drv();
		}
	else	if(strob_cnt==5)
		{
		if(!strob_cnt51)
			{
			strob_cnt51=1;
			if(!play)memo_read ();
			}
		else
			{
			strob_cnt51=0;
			if(!play)ibatton_drv();
			}

		}
	}
} 




/*----------------------------------------------------------------------------
 *        Main: 
 *---------------------------------------------------------------------------*/
int main (void) {

stm32_Init();
SysTick->LOAD  = 1088;  
delay_us(100000);                             
dac_init();
//init_display ();
//plazma1=SD_Reset();
///ind=iMn;
///i2c_init();
///_24c01_read_nbyte(EE_MAIN_TIME,(char*)&MAIN_TIME,12);
adc_init();

			//	_24c01_write_4byte(ADR_EE_IBATTON0,0x11223344UL);
			//	Delay(10);

while (1) 
	{
	if(bBUFF_LOAD_L)
		{
		bBUFF_LOAD_L=0;
		//GPIOB->ODR^=(1<<11);
		if(current_page_in_memory<last_page)
			{
			current_page_in_memory++;
			}
		else 
			{
			current_page_in_memory=0;
		    	play=0;
			}	
		DF_page_to_buffer(1,current_page_in_memory);
		}

	if(bBUFF_LOAD_H)
		{
		bBUFF_LOAD_H=0;
		
		if(current_page_in_memory<last_page)
			{
			current_page_in_memory++;
			}
		else 
			{
			current_page_in_memory=0;
		    	play=0;
			}	
		DF_page_to_buffer(1,current_page_in_memory);
		}
	
	if(bBUFF_READ_L)
		{
		bBUFF_READ_L=0;
		DF_buffer_read(1,0,256,(char*)sound_buffer);
          }	

	if(bBUFF_READ_H)
		{
		bBUFF_READ_H=0;
		DF_buffer_read(1,0,256,(char*)&sound_buffer[128]);
		}		

 	if(bRXIN2)
     	{
		bRXIN2=0;
		uart_in2();
				
		}
		
		                                   
	if (b1000Hz) 
		{
		b1000Hz=0;
		rele_hndl();
		}
	if (b100Hz) 
		{
		b100Hz=0;
		but_an();
		
		ibatton_an();
		
		}
	if (b10Hz) 
		{
		b10Hz=0; 

		ind_hndl();
		
		
		if(ret_cnt)
			{
			ret_cnt--;
			if(ret_cnt==0)
				{
				ind=iMn;
				ind_cnt_=30;
				}
			}
		lamp_drv();
		
		}   
	if (b5Hz) 
		{
		b5Hz=0; 
		adc_drv();
		
		//uart_out2(6,0x55,0x55,0x55,0x55,0x55,0x55);
		}
	if (b1Hz) 
		{
		b1Hz=0;

		process_hndl();
		u_min_hndl();
		ibatton_n=0;
		if((ibatton_repeat_cnt)&&(!ibatton_repeat_cnt))ibatton_repeat_cnt_=ibatton_repeat_cnt;
		else if((ibatton_repeat_cnt==ibatton_repeat_cnt))
			{
			ibatton_repeat_cnt_=0;
			ibatton_repeat_cnt=0;
			}
		}
	}
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
