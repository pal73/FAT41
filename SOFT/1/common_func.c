#include <RTL.h>  
#include "common_func.h"
#include "main.h"



//-----------------------------------------------
signed short abs(signed short in)
{
if(in<0)in=-in;
return in;
}





//-----------------------------------------------
void bin2bcd_int(unsigned int in)
{

char i=5;
for(i=0;i<5;i++)
	{
	dig[i]=in%10;
	in/=10;
	}   
}
//-----------------------------------------------
void bcd2lcd_zero(char sig)
{
char i;
zero_on=1;
for (i=5;i>0;i--)
	{
	if(zero_on&&(!dig[i-1])&&(i>sig))
		{
		dig[i-1]=0x20;
		}
	else
		{
		dig[i-1]=dig[i-1]+0x30;
		zero_on=0;
		}	
	}
}             








//-----------------------------------------------
char ptr_carry(signed int in,unsigned char modul,signed int carry)
{
signed int tempSI;
tempSI=in;                                                             
tempSI+=carry;
if(tempSI<0)tempSI+=modul;
else if(tempSI>=modul)tempSI-=modul;

return (char)tempSI;
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

