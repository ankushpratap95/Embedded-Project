#include <mbed.h>
# define read_spi 0x80              //enable spi_read
# define write_spi 0x00             // enable spi_write
# define mosi PF_9                  //mosi
# define miso PF_8                  //miso
# define sclk PF_7                  //sclk
# define CS PC_1                    //CS
# define who_am_i_reg 0x0f          // address of who_am_i register
# define ctrl_reg_1 0x20            // different registers adderesses
# define ctrl_reg_2 0x21            // different registers adderesses
# define ctrl_reg_3 0x22            // different registers adderesses
# define ctrl_reg_4 0x23            // different registers adderesses
# define ctrl_reg_5 0x24            // different registers adderesses
# define ENABLE_GYRO_REG 0x0F       // different registers adderesses
# define DUMMY 0x00
# define radius_length 0.3 //taken by default fixed
 



int get_who_am_i(SPI *spi); //get_who_am_i funct
void write_SPI(SPI *spi, int address, int write_bit); //write_spi funct
void gyro_initialize(SPI *spi, DigitalOut *cs); //gyro_init
float cal_linear_distance(short xAxis[], short yAxis[], short zAxis[]); //calculate_linear_dist funct



