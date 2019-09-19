#define PAROL_FISK	753 

#include <RTL.h>



#define BIN__N(x) (x) | x>>3 | x>>6 | x>>9
#define BIN__B(x) (x) & 0xf | (x)>>12 & 0xf0
#define BIN8(v) (BIN__B(BIN__N(0x##v)))

//***********************************************
//Установки для софтового I2C
#define PORT_I2C_SDA	GPIOB
#define PORT_I2C_SCL	GPIOB
#define PIN_SDA		1
#define PIN_SCL		0

#define PAROL_ALL_IBATTON_ERASE	1111
#define PAROL_IBATTON_WRITE		2222
#define PAROL_IBATTON_VIEW		3333
#define PAROL_IBATTON_PVLK		1410

#define _END_OF_PLAY_	0
#define _LOW_BATTERY_	1
#define _GONG_			2
#define _ONE_BEEP_		3
#define _TWO_BEEP_		4
#define _THREE_BEEP_	5

//-----------------------------------------------
//Таймер
extern char b1000Hz,b100Hz,b50Hz,b10Hz,b5Hz,b2Hz,b1Hz,b33Hz;
extern short t0cnt,t0cnt0,t0cnt1,t0cnt2,t0cnt3,t0cnt4,t0cnt5,t0cnt6,t0_cnt7,t0_cnt8;
extern char bFL5,bFL2,bFL,bFL_;
extern volatile unsigned long SysTickCnt;

//-----------------------------------------------
//Динамическая индикация
extern char ind_cnt;
extern char ind_out[10];
extern char ind_out_[5];
extern const char IND_CONST[20];
extern char dig[5];
extern char ind_cnt_;

//-----------------------------------------------
//Индикация
typedef struct  
{
enum {iMn,iSrv_sl,iNet,iSet,iSet_sel,iSet_time,iIbatton_prl,iSet_prl,iK_prl,iIbatton,iK,
	iK_u,iSpc,k,Crash_0,Crash_1,iKednd,
	iLoad,iSpc_prl_vz,iSpc_prl_ke,iKe,iVz,iAVAR,iStr,iVrs,iPrltst,iApv,
	iK_src,iK_bat,iK_load,iK_net,iTst,iTst_klbr,iTst_BPS1,iTst_BPS2,
	iTst_BPS12,iDeb,iDef,iSet_st_prl,iK_pdp,iSet_T,iDemo_per,iJ_bat,
	iPrl_bat_in_out,iSetT,iVol,iPlay,iJAv_net,iJAv_src1,
	iJAv_src2,iJAv_bat,iJAv_bat_sel,iSetFile,iAusw_prl,iAusw_set,iK_t_out,
	iJ_bat_ke_sel,iJ_bat_ke,iJ_bat_vz_sel,iJ_bat_vz,iJ_bat_wrk_sel,iJ_bat_wrk,
	iExt_set,iExt_dt,iExt_sk,iFisk_prl,iFisk,iDemo_len,iDemo_vol,iTest}i;

signed char s_i;
signed char s_i1;
signed char s_i2;
signed char i_s;
} stuct_ind;

extern stuct_ind a,b[10];

#define ind     a.i
#define sub_ind     a.s_i
#define sub_ind1     a.s_i1
#define sub_ind2     a.s_i2
#define index_set     a.i_s

extern char dig[5];
extern char zero_on;
extern const char ABCDEF[];
extern signed char ptr_ind;
extern char mnemo_cnt;
extern char fl_simv_num,fl_simv_len;
extern char default_temp;
extern signed short av_j_si_max,av_j_si_max_;
extern char simax;
extern char phase;
extern const char sm_[]; //
extern const char sm_exit[]; //
extern const char smon[]; 
extern const char smoff[];
extern const char sm_mont[13][4]; //
extern char content[63];
extern signed char parol[5];
extern short ret_cnt; 
extern char pvlk;


//-----------------------------------------------
//Управление процессом
extern signed short time_wrk;
extern signed short demo_per;
extern signed short demo_len;
extern signed short current_sound;
extern signed short wrk_cnt;
extern signed short max_file;
extern signed short current_file;
extern signed short current_demo_file;
extern signed short main_demo_cnt;

//-----------------------------------------------
//Запоминание таблеток
extern unsigned int i_batton[10];
extern short i_batton_num;

//-----------------------------------------------
//Измерение напряжения
extern signed short Ku;
extern signed short U;
extern signed short umin_cnt;

//-----------------------------------------------
//Управление воспроизведением
extern signed short file_len[10];
extern char play;
extern long file_lengt;
extern unsigned short file_lengt_in_pages,current_page_in_file,current_page_in_memory,last_page,current_byte_in_buffer;
extern char bSTART,bBUFF_LOAD,bBUFF_READ_L,bBUFF_READ_H,bBUFF_LOAD_L,bBUFF_LOAD_H;
extern short sample;
extern signed short sample_cnt;
extern short sound_buffer[256];
extern signed short file_num;

//-----------------------------------------------
//Фискальный счетчик
extern signed short fisk_cnt;


//-----------------------------------------------
//Низкое напряжение
extern char u_min_cnt;
extern char bUMIN;
extern char u_min_hndl_cnt;

//**********************************************
//Коэффициенты, отображаемые из ЕЕПРОМ
/*extern signed short MAIN_TIME;
extern signed short DEMO_TIME;
extern signed short DEMO_PERIOD;
extern signed short MAIN_VOLUME;
extern signed short NUM_OF_FILES;*/

//-----------------------------------------------
//АЦП
extern signed short adc_buff[16],adc_buff_;
extern char adc_cnt;

//-----------------------------------------------
//Управление воспроизведением музыки
extern signed short main_music_cnt;
extern signed short max_main_music_cnt;
extern signed short fade_out_main_music_cnt;
//-----------------------------------------------
//Управление громкостью
extern signed short volume;
extern signed char volume_vector;
extern signed short volume_koef;
extern signed short volume_max;
extern signed short volume_period_cnt;
extern signed short volume_period_cnt_max;

//-----------------------------------------------
//Управление процессом
//extern signed short time_wrk;
//extern signed short demo_per;
//extern signed short demo_len;
extern signed short current_sound;
extern signed short wrk_cnt;
extern signed short max_file;
extern signed short current_file;
extern signed short current_demo_file;
extern signed short main_demo_cnt;

extern char buff[1024];

//**********************************************
//Карта памяти ЕЕПРОМ
#define EE_MAIN_TIME	10
#define EE_DEMO_TIME	12
#define EE_DEMO_PERIOD 	14
#define EE_NUM_OF_FILES	16
#define EE_MAIN_VOLUME	18
#define EE_MAIN_CNT		20

//-----------------------------------------------
void delay_us(long del);
//-----------------------------------------------
void ret(char duty);
//-----------------------------------------------
void ret_hndl(void);
//-----------------------------------------------
void start_play(char in);

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
