//importing libraries
#include "mbed.h"
#include "helper.h"
#include <time.h>

SPI spi(PF_9, PF_8, PF_7); // mosi, miso, sclk
DigitalOut cs(PC_1); //chip select (CS)

bool sample=false; // set according to 0.5 sec interval
Ticker tick;
void sample_collect_ticker() //Ticker Funct
{
  
  sample = true;

}

int main() {
  //gyro_init
  gyro_initialize(&spi,&cs);

  //ticket attached
  tick.attach(&sample_collect_ticker, 0.5f);

  //storing angular_velocities
  short angular_vel_x[50];
  short angular_vel_y[50];
  short angular_vel_z[50];

  // to maintain index
  int count = 0;

  //initial value 0
  float linear_dist = 0.0;

  while(1)
  {
    if (sample == true) // to check if sampling needs to be done
    {
    // read opertion for the x-axis and storing
    cs = 0;          
    spi.write(0xA8);
    short xAxisLowBit = spi.write(0x00);
    cs = 1;
    cs = 0;
    spi.write(0xA9);
    short xAxisHighBit = spi.write(0x00);
    cs = 1;
    short xAxis = xAxisHighBit << 8 | xAxisLowBit;
    angular_vel_x[count] = xAxis;

    // read opertion for the y-axis and storing
    cs = 0;
    spi.write(0xAA); 
    short yAxisLowBit = spi.write(0x00);
    cs = 1;
    cs = 0;
    spi.write(0xAB);
    short yAxisHighBit = spi.write(0x00);
    cs = 1;
    short yAxis = yAxisHighBit << 8 | yAxisLowBit;
    angular_vel_y[count] = yAxis;

    // read opertion for the z-axis and storing
    cs = 0;
    spi.write(0xAC); 
    short zAxisLowBit = spi.write(0x00);
    cs = 1;
    cs = 0;
    spi.write(0xAD);
    short zAxisHighBit = spi.write(0x00);
    cs = 1;
    short zAxis = zAxisHighBit << 8 | zAxisLowBit;
    angular_vel_z[count] = zAxis;

    printf("Sample collected at t= %0.4f\n", (count+1) * 0.5);

    printf("X axis Raw Output: %0.4f  Y axis Raw Output: %0.4f  Z Raw Output: %0.4f\n", float(xAxis),float(yAxis),float(zAxis));


    count+=1; //incrementing index
    sample=false; //setting false, only an interrupt would turn on sampling

    if (count>40) //stop condition
    {
    linear_dist = cal_linear_distance(angular_vel_x,angular_vel_y,angular_vel_z); //calling cal_linear_dis funct
    printf("Linear Distance Travelled:%0.4f\n\n", linear_dist);
    count = 0;
    break;
    }
    }
    
  }

}
