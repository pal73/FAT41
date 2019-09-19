#include <stm32f10x_lib.h>
#include "ibatton.h"
#include "main.h"
#include "eeprom_map.h"
#include "24c01.h"

char ibatton_n,ibatton_n_;
char res[8];
short	ibatton_repeat_cnt,ibatton_repeat_cnt_;
int ibatton_new,ibatton_old,ibatton;
char ib_pl[5];
char ibatton_an_plazma,ibatton_an_plazma_;

//-----------------------------------------------
void ibatton_an(void)
{
unsigned int ib;
short ib_n;
short wt;

if(ind==iMn)
	{
	if(ibatton_n)
		{
		char i_flag,i;

		ibatton_an_plazma++;

		ib_n=_24c01_read_2byte(EE_IBATTON_NUM);
		Delay(20);
		wt=_24c01_read_2byte(EE_TIME_WRK);
		Delay(20);
		i_flag=0;

		for(i=0;i<ib_n;i++)
			{
			ib=i_batton[i];//_24c01_read_4byte(ADR_EE_IBATTON[i]);
			if(ibatton==ib)i_flag=1;
			}
		if(i_flag)
			{
			if((!wrk_cnt)&&(!bUMIN))
				{
				wrk_cnt=wt;
				start_play(_ONE_BEEP_);
				}
			else if (((wt-wrk_cnt)<30)&&((wt-wrk_cnt)>2))
				{
				wrk_cnt=0;
				start_play(_THREE_BEEP_);
				}
			}
		}
	if(ibatton_n_)
		{
		char i_flag,i;

		ib_n=_24c01_read_2byte(EE_IBATTON_NUM);
		Delay(20);
		i_flag=0;

		for(i=0;i<ib_n;i++)
			{
			ib=_24c01_read_4byte(ADR_EE_IBATTON[i]);
			if(ibatton==ib)i_flag=1;
			}
		if(i_flag)
			{
			ind=iFisk;
			sub_ind=0;
			ret_cnt=30;
			}


		}
	}
else if(ind==iIbatton)
	{
	if(sub_ind==0)
		{
		if(ibatton_n_)
			{
			char i_flag,i;
			i_flag=0;
			
			if(i_batton_num)
				{
				for(i=0;(i<i_batton_num)&&(i<10);i++)
					{
					if(ibatton==i_batton[i])i_flag=1;
					}
				}
			
			if(!pvlk)sub_ind=1;
			
			if((i_flag==0)&&(i_batton_num<9))
				{
				/*_24c01_write_2byte(ADR_EE_IBATTON[i_batton_num],(short)ibatton);
				Delay(5000);
				_24c01_write_2byte(ADR_EE_IBATTON[i_batton_num]+2,(short)(ibatton>>16));
				Delay(5000);*/
				_24c01_write_4byte(ADR_EE_IBATTON[i_batton_num],ibatton);
				Delay(10);
				_24c01_write_2byte(EE_IBATTON_NUM,_24c01_read_2byte(EE_IBATTON_NUM)+1);
				Delay(10);
				start_play(_ONE_BEEP_);
				}
			}
		}
	}
else if(ind==iFisk)
	{
	if(ibatton_n)
		{
		char i_flag,i;
		i_flag=0;

		for(i=0;i<i_batton_num;i++)
			{
			if(ibatton==i_batton[i])i_flag=1;
			}

		if(i_flag)
			{
			if(sub_ind<4)
				{
				sub_ind++;
				ret_cnt=100;
				start_play(_ONE_BEEP_);
				}
			else
				{
				ind=iMn;
				}
			}


		}
	}


ibatton_n=0;
ibatton_n_=0;
}

//-----------------------------------------------
void ibatton_drv(void)
{
//ibatton_repeat_cnt=34;//ibatton_polling();
if(ibatton_polling())
	{
	//ibatton_repeat_cnt++;
	ibatton_read_rom();
	ibatton_new=*((int*)&res[1]);
/*	if((ibatton_new!=0xffffffff)&&(ibatton_new!=0x00000000)&&(!ibatton_repeat_cnt))
		{
		ibatton=ibatton_new;
		ibatton_n=1;
		}*/
	if((ibatton_new!=0xffffffff)&&(ibatton_new!=0x00000000))
		{
		
		if(ibatton_new==ibatton_old)
			{
			ibatton_repeat_cnt++; 
			if(ibatton_repeat_cnt>=80)
				{
				ibatton=ibatton_new;
				ibatton_n_=1;
				ibatton_repeat_cnt=0;
				ibatton_an_plazma_++;
				}
			}
		}
	ibatton_old=ibatton_new;
	}
else
	{
	if((ibatton_repeat_cnt)&&(ibatton_repeat_cnt<100)&&(ibatton_new!=0xffffffff)&&(ibatton_new!=0x00000000))
		{
		ibatton=ibatton_new;
		ibatton_n=1;
		}
	
	//ibatton_repeat_cnt=0;
	ibatton_new=0;
	}
}

//-----------------------------------------------
char ibatton_polling(void)
{
short i,ii,num_out;
long inp;
ib_pl[0]++;
GPIOB->CRH=(GPIOB->CRH&~0x00000f00)|0x00000700;
//SET_REG(LPC_PINCON->PINSEL2,0,10*2,2);
//SET_REG(LPC_GPIO1->FIOPIN,0,10,1);

//SET_REG(LPC_GPIO1->FIODIR,1,10,1);

GPIOB->ODR&=~(1<<10);

//импульс сброса 600мкс
for(i=0;i<2000;i++)
	{
	__nop();
	}
//SET_REG(LPC_GPIO1->FIODIR,0,10,1);
GPIOB->ODR|=(1<<10);

//выдержка 15мкс
for(i=0;i<50;i++)
	{
	__nop();
	}

//еще 45мкс ждем сигнала от таблетки
for(i=0;i<50;i++)
	{
	__nop();
	__nop();
	__nop();
	inp=GPIOB->IDR;
	if(!(inp&(1<<10)))goto ibatton_polling_lbl_000;
	}
goto ibatton_polling_lbl_zero_exit;

ibatton_polling_lbl_000:

//измеряем длительность ответного импульса не дольше 300мкс
ib_pl[1]++;
for(i=0;i<200;i++)
	{
	inp=GPIOB->IDR;
	if(!(inp&(1<<10)))
		{
		__nop();
		__nop();
		num_out=10;
		goto ibatton_polling_lbl_001;	//continue;
		}
	}
num_out=5;
goto ibatton_polling_lbl_zero_exit;

ibatton_polling_lbl_001:
//выдержка 15мкс
for(i=0;i<1000;i++)
	{
	__nop();
	}
ibatton_polling_lbl_success_exit:
/*SET_REG(IO0DIR,0,19,1);
for(i=0;i<800;i++)
	{
	__nop();
	}
SET_REG(IO0DIR,1,19,1);
for(i=0;i<800;i++)
	{
	__nop();
	}
SET_REG(IO0DIR,0,19,1);*/
return 1;
ibatton_polling_lbl_zero_exit:
/*SET_REG(IO0DIR,0,19,1);
for(i=0;i<800;i++)
	{
	__nop();
	}
SET_REG(IO0DIR,1,19,1);
for(i=0;i<160;i++)
	{
	__nop();
	}
SET_REG(IO0DIR,0,19,1); */
return 0;


/*ibatton_polling_lbl_cnt_exit:
SET_REG(LPC_GPIO1->FIODIR,0,10,1);
for(i=0;i<1600;i++)
	{
	__nop();
	}
for(ii=0;ii<num_out;ii++)
	{
	SET_REG(LPC_GPIO1->FIODIR,1,10,1);
	for(i=0;i<160;i++)
		{
		__nop();
		}
	SET_REG(LPC_GPIO1->FIODIR,0,10,1);
	for(i=0;i<160;i++)
		{
		__nop();
		}
	}
return 0;*/

}

//-----------------------------------------------
char ibatton_read_rom(void)
{
ibatton_send_byte(0x0f);

res[0]=ibatton_read_byte();
res[1]=ibatton_read_byte();
res[2]=ibatton_read_byte();
res[3]=ibatton_read_byte();
res[4]=ibatton_read_byte();
res[5]=ibatton_read_byte();
res[6]=ibatton_read_byte();
res[7]=ibatton_read_byte();
}


//-----------------------------------------------
char ibatton_w1ts(void)
{
short i,ii,num_out;
GPIOB->CRH=(GPIOB->CRH&~0x00000f00)|0x00000700;
GPIOB->ODR&=~(1<<10);

//импульс 10мкс
for(i=0;i<50;i++)
	{
	__nop();
	}
GPIOB->ODR|=(1<<10);

//выдержка 90мкс
for(i=0;i<280;i++)
	{
	__nop();
	} 
}

//-----------------------------------------------
char ibatton_w0ts(void)
{
short i,ii,num_out;
//SET_REG(LPC_GPIO1->FIOCLR,1,10,1);
//SET_REG(LPC_GPIO1->FIODIR,1,10,1);

GPIOB->CRH=(GPIOB->CRH&~0x00000f00)|0x00000700;
GPIOB->ODR&=~(1<<10);

//импульс 90мкс
for(i=0;i<280;i++)
	{
	__nop();
	}
GPIOB->ODR|=(1<<10);

//выдержка 10мкс
for(i=0;i<50;i++)
	{
	__nop();
	}
}

//-----------------------------------------------
void ibatton_send_byte(char in)
{
char i,ii;
ii=in;

for(i=0;i<8;i++)
	{
	if(ii&0x01)ibatton_w1ts();
	else ibatton_w0ts();
	ii>>=1;
	}
}

//-----------------------------------------------
char ibatton_read_byte(void)
{
char i,ii;
ii=0;

for(i=0;i<8;i++)
	{
	ii>>=1;
	if(ibatton_rts())ii|=0x80;
	else ii&=0x7f;
	}
return ii;
}

//-----------------------------------------------
char ibatton_rts(void)
{
short i,ii,num_out;

GPIOB->CRH=(GPIOB->CRH&~0x00000f00)|0x00000700;
GPIOB->ODR&=~(1<<10);

//импульс 10мкс
for(i=0;i<30;i++)
	{
	__nop();
	}
GPIOB->ODR|=(1<<10);

//импульс 20мкс
for(i=0;i<30;i++)
	{
	__nop();
	}
if((GPIOB->IDR)&(1<<10))	ii=1;
else ii=0;

//выдержка 30мкс
for(i=0;i<100;i++)
	{
	__nop();
	}
return ii;
}