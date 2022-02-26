//importing libraries
#include "helper.h"
#include "mbed.h"

int get_who_am_i(SPI *spi)
{  

    int address = (read_spi | who_am_i_reg);
    spi->write(address);                        // Send 0x8f, the command to read the WHOAMI register  
    int who = spi->write(0x00);        // Send a dummy byte to receive the contents of the WHOAMI register  
 
    return who;

}

void write_SPI(SPI *spi, int address, int write_bit)
{

    spi->write(address);
    spi->write(write_bit); // write SPI 
  
}

void gyro_initialize(SPI *spi, DigitalOut *cs)
{
  int Address;
  int write_bit;
  cs->write(1);            // Chip must be deselected
                          // Setup the spi for 8 bit data, high steady state clock,
                          // second edge capture, with a 1MHz clock rate
  spi->format(8, 3);       // 8 bit data format and 3 is spi mode
  spi->frequency(1000000); // 1MHz
  cs->write(0);                 // here we want to start communicating with the chip;
  int who = get_who_am_i(spi);
  printf("who_am_i = 0x%X\n", who);
  cs->write(1);                 // Chip must be deselected

  cs->write(0);  
  Address = write_spi | ctrl_reg_1;
  write_bit = ENABLE_GYRO_REG;
  write_SPI(spi, Address, write_bit); //enable the gyroscope in normal mode
  cs->write(1); 

  cs->write(0);  
  Address = write_spi | ctrl_reg_2;
  write_bit = DUMMY;
  write_SPI(spi, Address, write_bit);
  cs->write(1); 

  cs->write(0);  
  Address = write_spi | ctrl_reg_3;
  write_bit = DUMMY;
  write_SPI(spi, Address, write_bit);
  cs->write(1); 

  cs->write(0);  
  Address = write_spi | ctrl_reg_4;
  write_bit = DUMMY;
  write_SPI(spi, Address, write_bit);
  cs->write(1); 

  cs->write(0);  
  Address = write_spi | ctrl_reg_5;
  write_bit = DUMMY;
  write_SPI(spi, Address, write_bit);
  cs->write(1); 

}

// Calulate linear dist from different angular vel
float cal_linear_distance(short angular_vel_x[], short angular_vel_y[], short angular_vel_z[])
{
  float dt = 0.5;
  float dx = 0.0;
  float dy = 0.0;
  float dz = 0.0;
  float velocity_x[40];
  float velocity_y[40];
  float velocity_z[40];
  float angular_velocity_x[40];
  float angular_velocity_y[40];
  float angular_velocity_z[40];

  angular_velocity_x[0] = float(angular_vel_x[0]) * 250.0/pow(2,15); // normalizing values
  angular_velocity_y[0] = float(angular_vel_y[0]) * 250.0/pow(2,15);
  angular_velocity_z[0] = float(angular_vel_z[0]) * 250.0/pow(2,15);
  

  for (int i=1; i<40; i++)
  {
    angular_velocity_x[i] = float(angular_vel_x[i]) * 250.0/pow(2,15); // normalizing values
    angular_velocity_y[i] = float(angular_vel_y[i]) * 250.0/pow(2,15);
    angular_velocity_z[i] = float(angular_vel_z[i]) * 250.0/pow(2,15);

    printf("Angular Vel X axis: %0.4f  Angular Vel Y axis: %0.4f  Angular Vel Z: %0.4f\n", angular_velocity_x[i],angular_velocity_y[i],angular_velocity_z[i]);
    
    velocity_x[i] = radius_length*abs(angular_velocity_x[i-1] + angular_velocity_x[i])/2.0f; // calculating linear vel in different axis
    velocity_y[i] = radius_length*abs(angular_velocity_y[i-1] + angular_velocity_y[i])/2.0f;
    velocity_z[i] = radius_length*abs(angular_velocity_z[i-1] + angular_velocity_z[i])/2.0f;

    printf(" Linear Vel X axis: %0.4f  Linear Vel Y axis: %0.4f  Linear Vel Z: %0.4f\n", velocity_x[i],velocity_y[i],velocity_z[i]);

    dx = dx + (velocity_x[i] * dt); // calculating change in distance along different axis
    dy = dy + (velocity_y[i] * dt);
    dz = dz + (velocity_z[i] * dt);
  }

  float linear_dist = sqrt(dx*dx + dy*dy + dz*dz); //taking sqrt
  return (linear_dist);

}
