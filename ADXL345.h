//This program Writed By AMIN HOSSEINI AT 2019/9/25
// Just For Test ADXL345 MODULE 


#include <delay.h>
#include <math.h>



#ifndef __ADXL345_H
    #define __ADXL345_H
#pragma used+

unsigned int Accel_Xout_Val=0, Accel_Yout_Val=0, Accel_Zout_Val=0;
int Accel_Xout_Normalized_Val=0, Accel_Yout_Normalized_Val=0, Accel_Zout_Normalized_Val=0;
unsigned char Offset_X_Val=0,Offset_Y_Val=0,Offset_Z_Val=0;
float Accel_XAngle=0, Accel_YAngle=0, Accel_ZAngle=0;


#define     ADXL345_Addr    0xA6

//ADXL345 REGISTER ADDRESS MAP:
#define     RA_DEVID            0x00    // R     0xE5   Device ID
#define     RA_THRESH_TAP       0x1D    // R/W   0      Tap threshold
#define     RA_OFSX             0x1E    // R/W   0      X-axis offset
#define     RA_OFSY             0x1F    // R/W   0      Y-axis offset
#define     RA_OFSZ             0x20    // R/W   0      Z-axis offset
#define     RA_DUR              0x21    // R/W   0      Tap duration
#define     RA_Latent           0x22    // R/W   0      Tap latency
#define     RA_Window           0x23    // R/W   0      Tap window
#define     RA_THRESH_ACT       0x24    // R/W   0      Activity threshold
#define     RA_THRESH_INACT     0x25    // R/W   0      Inactivity threshold
#define     RA_TIME_INACT       0x26    // R/W   0      Inactivity time
#define     RA_ACT_INACT_CTL    0x27    // R/W   0      Axis enable control for activity and inactivity detection
#define     RA_THRESH_FF        0x28    // R/W   0      Free-fall threshold
#define     RA_TIME_FF          0x29    // R/W   0      Free-fall time
#define     RA_TAP_AXES         0x2A    // R/W   0      Axis control for single tap/double tap
#define 	RA_ACT_TAP_STATUS   0x2B 	// R 	 0      Source of single tap/double tap
#define 	RA_BW_RATE 	        0x2C 	// R/W 	 0x0A 	Data rate and power mode control
#define 	RA_POWER_CTL 	    0x2D 	// R/W 	 0   	Power-saving features control
#define 	RA_INT_ENABLE 	    0x2E 	// R/W 	 0 	    Interrupt enable control
#define 	RA_INT_MAP 	        0x2F 	// R/W 	 0 	    Interrupt mapping control
#define 	RA_INT_SOURCE 	    0x30 	// R 	 0x02 	Source of interrupts
#define 	RA_DATA_FORMAT 	    0x31 	// R/W 	 0   	Data format control
#define 	RA_DATAX0 	        0x32 	// R 	 0 	    X-Axis Data 0
#define 	RA_DATAX1 	        0x33 	// R 	 0 	    X-Axis Data 1
#define 	RA_DATAY0 	        0x34 	// R 	 0 	    Y-Axis Data 0
#define 	RA_DATAY1 	        0x35 	// R 	 0 	    Y-Axis Data 1
#define 	RA_DATAZ0 	        0x36 	// R 	 0 	    Z-Axis Data 0
#define 	RA_DATAZ1 	        0x37 	// R 	 0 	    Z-Axis Data 1
#define 	RA_FIFO_CTL 	    0x38 	// R/W 	 0 	    FIFO control
#define 	RA_FIFO_STATUS 	    0x39 	// R 	 0 	    FIFO status


unsigned char read_i2c(unsigned char BusAddres , unsigned char Reg , unsigned char Ack )
{
    unsigned char Data;
    i2c_start();
    i2c_write(BusAddres);
    i2c_write(Reg);
    i2c_start();
    i2c_write(BusAddres + 1);
    delay_ms(1);
    Data=i2c_read(Ack);
    i2c_stop();
    return Data;
}

void write_i2c(unsigned char BusAddres , unsigned char Reg , unsigned char Data)
{
    i2c_start();
    i2c_write(BusAddres);
    i2c_write(Reg);
    i2c_write(Data);
    i2c_stop();
}
//AMIN HOSSEINI
unsigned char GetDeviceID(void)
{
    return read_i2c(ADXL345_Addr,RA_DEVID,0);
}

void Set_X_Offset(unsigned char X_Val)
{
    write_i2c(ADXL345_Addr,RA_OFSX,X_Val);
}

void Set_Y_Offset(unsigned char Y_Val)
{
    write_i2c(ADXL345_Addr,RA_OFSY,Y_Val);
}

void Set_Z_Offset(unsigned char Z_Val)
{
    write_i2c(ADXL345_Addr,RA_OFSZ,Z_Val);
}

void Get_Offset_Values(void)
{
	Offset_X_Val = read_i2c(ADXL345_Addr,RA_OFSX,0);
    Offset_Y_Val = read_i2c(ADXL345_Addr,RA_OFSY,0);
    Offset_Z_Val = read_i2c(ADXL345_Addr,RA_OFSZ,0);
}

void Get_Accel_Values(void)
{
    Get_Offset_Values();

    Accel_Xout_Val = ( (read_i2c(ADXL345_Addr,RA_DATAX1,0)<<8) | read_i2c(ADXL345_Addr,RA_DATAX0,0) )-Offset_X_Val;
    Accel_Yout_Val = ( (read_i2c(ADXL345_Addr,RA_DATAY1,0)<<8) | read_i2c(ADXL345_Addr,RA_DATAY0,0) )-Offset_Y_Val;
	Accel_Zout_Val = ( (read_i2c(ADXL345_Addr,RA_DATAZ1,0)<<8) | read_i2c(ADXL345_Addr,RA_DATAZ0,0) )-Offset_Z_Val;
}

void Get_Averge_Accel_Values(void)
{
  #define    NumAve      20
  unsigned long int Ave=0;
  unsigned char i=NumAve;

  Get_Offset_Values();

  while(i--)
  {
	Accel_Xout_Val = ( (read_i2c(ADXL345_Addr,RA_DATAX1,0)<<8) | read_i2c(ADXL345_Addr,RA_DATAX0,0) )-Offset_X_Val;
    Ave+=Accel_Xout_Val;
  }
  Accel_Xout_Val=Ave/NumAve;
  Ave=0;
  i=NumAve;
  while(i--)
  {
	Accel_Yout_Val = ( (read_i2c(ADXL345_Addr,RA_DATAY1,0)<<8) | read_i2c(ADXL345_Addr,RA_DATAY0,0) )-Offset_Y_Val;
    Ave+=Accel_Yout_Val;
  }
  Accel_Yout_Val=Ave/NumAve;
  Ave=0;
  i=NumAve;
  while(i--)
  {
	Accel_Zout_Val = ( (read_i2c(ADXL345_Addr,RA_DATAZ1,0)<<8) | read_i2c(ADXL345_Addr,RA_DATAZ0,0) )-Offset_Z_Val;
    Ave+=Accel_Zout_Val;
  }
  Accel_Zout_Val=Ave/NumAve;
  Ave=0;

}

void Get_Accel_Angles(void)
{
    Get_Accel_Values();
//  Convert Unsigned Int To Int
    Accel_Xout_Normalized_Val = Accel_Xout_Val;
    Accel_Yout_Normalized_Val = Accel_Yout_Val;
    Accel_Zout_Normalized_Val = Accel_Zout_Val;
//  Calculate The Angle Of Each Axis
	Accel_XAngle = 57.295*atan((float) Accel_Xout_Normalized_Val / sqrt(pow((float)Accel_Zout_Normalized_Val,2)+pow((float)Accel_Yout_Normalized_Val,2)));
	Accel_YAngle = 57.295*atan((float) Accel_Yout_Normalized_Val / sqrt(pow((float)Accel_Zout_Normalized_Val,2)+pow((float)Accel_Xout_Normalized_Val,2)));
  	Accel_ZAngle = 57.295*atan((float) sqrt(pow((float)Accel_Xout_Normalized_Val,2)+pow((float)Accel_Yout_Normalized_Val,2))/ Accel_Zout_Normalized_Val );
}

void ADXL345_SelfTest(void)
{
    //  VS=ON   VDDi/o=ON
    delay_ms(2);
    //  INITIAL COMMAND SEQUENCE
    write_i2c(ADXL345_Addr,RA_DATA_FORMAT,0x0B);   //  +/-16g , 13-BIT MODE
    write_i2c(ADXL345_Addr,RA_POWER_CTL,0x08);     //  START MEASUREMENT
    write_i2c(ADXL345_Addr,RA_INT_ENABLE,0x80);    //  ENABLE DATA_READY INTERRUPT

    delay_ms(2);

    Get_Accel_Values();
    Get_Accel_Values();
    Get_Accel_Values();

    //  ACTIVATE SELF-TEST
    write_i2c(ADXL345_Addr,RA_DATA_FORMAT,0x8B);   //  +/-16g , 13-BIT MODE , SELF TEST ON

    delay_ms(2000);

    Get_Accel_Values();
    Get_Accel_Values();
    Get_Accel_Values();
    //  CALCULATE SELF-TESTDELTA AND COMPARE IT TO DATASHEET LIMITS
    delay_ms(4000);

    //  INACTIVATE SELF-TEST
    write_i2c(ADXL345_Addr,RA_DATA_FORMAT,0x0B);   //  +/-16g , 13-BIT MODE , SELF TEST OFF
}

void ADXL345_Offset_Calib(void)
{
  #ifndef NumAve
    #define    NumAve      10
  #endif
  unsigned long int Ave=0;
  unsigned char i=NumAve;

    //  PLACE SENSOR IN  X=0g, Y=0g, Z=+1g  ORIENTATIONVS
    //  VS=ON   VDDi/o=ON
    delay_ms(2);

    write_i2c(ADXL345_Addr,RA_DATA_FORMAT,0x0B);   //  +/-16g , 13-BIT MODE
    write_i2c(ADXL345_Addr,RA_POWER_CTL,0x08);     //  START MEASUREMENT
    write_i2c(ADXL345_Addr,RA_INT_ENABLE,0x80);    //  ENABLE DATA_READY INTERRUPT

    delay_ms(12);

  //  Get Average of Accel For Each Axis
  while(i--)
  {
	Accel_Xout_Val = ((read_i2c(ADXL345_Addr,RA_DATAX1,0)<<8)|
                       read_i2c(ADXL345_Addr,RA_DATAX0,0)    );
    Ave += Accel_Xout_Val;
  }
  Accel_Xout_Val = Ave / NumAve;
  Ave = 0;
  i = NumAve;
  while(i--)
  {
	Accel_Yout_Val = ((read_i2c(ADXL345_Addr,RA_DATAY1,0)<<8)|
                       read_i2c(ADXL345_Addr,RA_DATAY0,0)    );
    Ave += Accel_Yout_Val;
  }
  Accel_Yout_Val = Ave / NumAve;
  Ave = 0;
  i = NumAve;
  while(i--)
  {
	Accel_Zout_Val = ((read_i2c(ADXL345_Addr,RA_DATAZ1,0)<<8)|
                       read_i2c(ADXL345_Addr,RA_DATAZ0,0)    );
    Ave += Accel_Zout_Val;
  }
  Accel_Zout_Val = Ave / NumAve;
  Ave=0;

    //  CALCULATE CALIBRATION VALUE
    Offset_X_Val = -(Accel_Xout_Val/4.0);
    Offset_Y_Val = -(Accel_Yout_Val/4.0);
    Offset_Z_Val = -((Accel_Zout_Val-256)/4.0);

    //  WRITE TO OFSTx REGISTERS
    Set_X_Offset(Offset_X_Val);
    Set_Y_Offset(Offset_Y_Val);
    Set_Z_Offset(Offset_Z_Val);
}

void ADXL345_Init(void)
{
    write_i2c(ADXL345_Addr,RA_DATA_FORMAT,0x0B);   //  +/-16g , 13-BIT MODE
    write_i2c(ADXL345_Addr,RA_POWER_CTL,0x08);     //  START MEASUREMENT
    write_i2c(ADXL345_Addr,RA_INT_ENABLE,0x80);    //  ENABLE DATA_READY INTERRUPT
}

void ADXL345_Regs_Reset(void)
{
    //Set Register to default value.

    write_i2c(ADXL345_Addr,RA_THRESH_TAP,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_OFSX,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_OFSY,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_OFSZ,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_DUR,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_Latent,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_Window,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_THRESH_ACT,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_THRESH_INACT,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_TIME_INACT,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_ACT_INACT_CTL,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_THRESH_FF,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_TIME_FF,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_TAP_AXES,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_BW_RATE,0xA0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_POWER_CTL,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_INT_ENABLE,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_INT_MAP,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_DATA_FORMAT,0);
    delay_ms(50);
    write_i2c(ADXL345_Addr,RA_FIFO_CTL,0);
    delay_ms(50);

    //Registers Reset successfully, you can use accelerometer now.
}
//AMIN HOSSEINI
#pragma used-
#endif
