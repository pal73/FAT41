#define RX_BUFFER_SIZE2 512
#define TX_BUFFER_SIZE2 512

/*
#define RX_BUFFER_SIZE0 1024
#define TX_BUFFER_SIZE0 1024

#define BUFSIZE    		1024

#define IER_RBR		0x01
#define IER_THRE		0x02
#define IER_RLS		0x04

#define PT	(UIB0[1]&0x80)
#define C_D	(UIB0[1]&0x40)
#define FR	(UIB0[1]&0x20)

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define VIC_UART0	6*/
/*
extern char bRXIN0;
extern char UIB0[100];
extern char flag0;
extern char rx_buffer0[RX_BUFFER_SIZE0];
extern char tx_buffer0[TX_BUFFER_SIZE0];
extern unsigned short rx_wr_index0,rx_rd_index0,rx_counter0;
extern unsigned short tx_wr_index0,tx_rd_index0,tx_counter0;
extern char rx_buffer_overflow0;
extern char plazma_uart0;
extern char memo_out[50];
extern char data_rs[50];
extern char data_rs0[50];*/
//-----------------------------------------------
//сюпр
extern char bRXIN2;
extern char UIB2[100];
extern char flag2;
extern char rx_buffer2[RX_BUFFER_SIZE2];
extern char tx_buffer2[TX_BUFFER_SIZE2];
extern unsigned short rx_wr_index2,rx_rd_index2,rx_counter2;
extern unsigned short tx_wr_index2,tx_rd_index2,tx_counter2;
extern char rx_buffer_overflow2;
extern char tx2_restart;
extern char memo_out[50];
extern char uart_plazma[5];
extern const char Table87[];
extern const char Table95[]; 

char crc_87(char* ptr,char num);
char crc_95(char* ptr,char num);

/*void putchar0(char c);
void uart_out0 (char num,char data0,char data1,char data2,char data3,char data4,char data5);
void uart_out_adr0 (char *ptr, char len);
void uart0_init(void);
char getchar0(void);
__irq void uart0_interrupt(void);
void uart_in_an0(void);
signed short index_offset0 (signed short index,signed short offset);
char control_check0(signed short index);

void uart_out_adr_block (unsigned long adress,char *ptr, char len);  */
void uart_in2(void);
