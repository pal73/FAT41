/* SPI_SR - bit definitions. */
#define RXNE    0x01
#define TXE     0x02
#define BSY     0x80

//-----------------------------------------------
void spi_init (void); 
//-----------------------------------------------
void spi_hi_speed (BOOL on);
//-----------------------------------------------
void spi_ss (U32 ss);
//-----------------------------------------------
U8 spi_send (U8 outb);
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
