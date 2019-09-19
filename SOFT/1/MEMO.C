#include "24c01.h"
#include "memo.h"
#include "eeprom_map.h"
#include "main.h"

char memo_read_cnt;
//-----------------------------------------------
void memo_read (void)
{
memo_read_cnt++;
if(memo_read_cnt>=20)memo_read_cnt=0;

if(memo_read_cnt==0)Ku=_24c01_read_2byte(EE_KU);
if(memo_read_cnt==1)fisk_cnt=_24c01_read_2byte(EE_FISK_CNT);
if(memo_read_cnt==2)i_batton_num=_24c01_read_2byte(EE_IBATTON_NUM);
if(memo_read_cnt==3)time_wrk=_24c01_read_2byte(EE_TIME_WRK);
if(memo_read_cnt==4)file_num=_24c01_read_2byte(EE_FILE_NUM);



if(memo_read_cnt==5)i_batton[0]=_24c01_read_4byte(ADR_EE_IBATTON[0]);
if(memo_read_cnt==6)i_batton[1]=_24c01_read_4byte(ADR_EE_IBATTON[1]);
if(memo_read_cnt==7)i_batton[2]=_24c01_read_4byte(ADR_EE_IBATTON[2]);
if(memo_read_cnt==8)i_batton[3]=_24c01_read_4byte(ADR_EE_IBATTON[3]);
if(memo_read_cnt==9)i_batton[4]=_24c01_read_4byte(ADR_EE_IBATTON[4]);
if(memo_read_cnt==10)i_batton[5]=_24c01_read_4byte(ADR_EE_IBATTON[5]);
if(memo_read_cnt==11)i_batton[6]=_24c01_read_4byte(ADR_EE_IBATTON[6]);
if(memo_read_cnt==12)i_batton[7]=_24c01_read_4byte(ADR_EE_IBATTON[7]);
if(memo_read_cnt==13)i_batton[8]=_24c01_read_4byte(ADR_EE_IBATTON[8]);
if(memo_read_cnt==14)i_batton[9]=_24c01_read_4byte(ADR_EE_IBATTON[9]);

}


