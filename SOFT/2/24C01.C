#include "24c01.h"
#include "i2c.h"

//-----------------------------------------------
char _24c01_read_byte(short adr)
{
char temp;

return temp;
}


//-----------------------------------------------
void _24c01_write_2byte(short adr,short data)
{
i2c_Start();
i2c_SendByte(0xa0);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_write_2byte;
i2c_SendByte((char)adr);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_write_2byte;
i2c_SendByte(data);
i2c_ReadAcknowledge();
i2c_SendByte(data>>8);
i2c_ReadAcknowledge();
Stop_label_24c01_write_2byte: 
i2c_Stop();
}

//-----------------------------------------------
void _24c01_write_4byte(short adr,long data)
{
i2c_Start();
i2c_SendByte(0xa0);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_write_4byte;
i2c_SendByte((char)adr);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_write_4byte;
i2c_SendByte(data);
i2c_ReadAcknowledge();
i2c_SendByte(data>>8);
i2c_ReadAcknowledge();
i2c_SendByte(data>>16);
i2c_ReadAcknowledge();
i2c_SendByte(data>>24);
i2c_ReadAcknowledge();
Stop_label_24c01_write_4byte: 
i2c_Stop();
}

//-----------------------------------------------
short _24c01_read_2byte(short adr)
{
short temp;
char temp1;
i2c_Start();
i2c_SendByte(0xa0);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_read_2byte;
i2c_SendByte((char)adr);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_read_2byte;
i2c_Restart();
i2c_SendByte(0xa1);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_read_2byte;
temp1=i2c_ReadByte();
i2c_SendAcknowledge(1);
temp=i2c_ReadByte();
i2c_SendAcknowledge(0);
Stop_label_24c01_read_2byte: 
i2c_Stop();
temp<<=8;
temp+=temp1;
return temp;
}

//-----------------------------------------------
long _24c01_read_4byte(short adr)
{
long temp;
char temp1;
long temp_[4];

i2c_Start();
i2c_SendByte(0xa0);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_read_4byte;
i2c_SendByte((char)adr);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_read_4byte;
i2c_Restart();
i2c_SendByte(0xa1);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_read_4byte;
temp_[0]=i2c_ReadByte();
i2c_SendAcknowledge(1);
temp_[1]=i2c_ReadByte();
i2c_SendAcknowledge(1);
temp_[2]=i2c_ReadByte();
i2c_SendAcknowledge(1);
temp_[3]=i2c_ReadByte();
i2c_SendAcknowledge(0);
Stop_label_24c01_read_4byte: 
i2c_Stop();
temp_[0]&=0x000000ff;
temp_[1]<<=8;
temp_[1]&=0x0000ff00;
temp_[2]<<=16;
temp_[2]&=0x00ff0000;
temp_[3]<<=24;
temp_[3]&=0xff000000;
temp=temp_[0]+temp_[1]+temp_[2]+temp_[3];
return temp;
}
//-----------------------------------------------
void _24c01_read_nbyte(short adr, char* buff, char n)
{
char i;
i2c_Start();
i2c_SendByte(0xa0);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_read_nbyte;
i2c_SendByte((char)adr);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_read_nbyte;
i2c_Restart();
i2c_SendByte(0xa1);
if(i2c_ReadAcknowledge()) goto Stop_label_24c01_read_nbyte;
for(i=0;i<(n-1);i++)
	{
	buff[i]=i2c_ReadByte();
	i2c_SendAcknowledge(1);
	}
buff[i]=i2c_ReadByte();
i2c_SendAcknowledge(0);
Stop_label_24c01_read_nbyte: 
i2c_Stop();

}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
