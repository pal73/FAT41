extern char res[8];
extern short ibatton_repeat_cnt,ibatton_repeat_cnt_;
extern int ibatton_new,ibatton_old,ibatton;
extern char ibatton_n,ibatton_n_;
extern short num_out;
extern char ib_pl[5];
extern char ibatton_an_plazma,ibatton_an_plazma_;
 
 void ibatton_drv(void);
 char ibatton_polling(void);
 char ibatton_read_rom(void);
 char ibatton_w1ts(void);
 char ibatton_w0ts(void);
 char ibatton_rts(void);
 void ibatton_send_byte(char in);
 char ibatton_read_byte(void);

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
