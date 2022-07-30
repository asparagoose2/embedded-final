//#include "LabC5_Accel_BitBang.X/System/delay.h"
//#include "LabC5_Accel_BitBang.X/System/system.h"
//#include "accel.h"
//#include <stdlib.h>
//#include <stdbool.h>
//#include <math.h>
//
//#define CLOCK_MILISECONDS 50
//
////  ======  Helper Functions  ================================================================
//
//void NOP()
//{
//    DELAY_microseconds(CLOCK_MILISECONDS);
//}
//
////  ======  Internal BitBanging Phases  ======================================================
//static void i2cMasterStrt(void)
//{
//    SCL = SDA = 1;  //Start
//    SDACTRL = TAKE;
//    SDA = 0;
//    asm("NOP");
//    SCL = 0;
//}
//
//static void i2cMasterSnd(unsigned char b)
//{
//    SDACTRL = TAKE;
//    NOP();
//    for(int i = 0; i < 8; i++) 
//    {
//        SCL = 0;
//        
//        SDA = b >> (7-i) & 1;
//        NOP();
//        SCL = 1;
////        DELAY_microseconds(5);
////        asm("NOP");
//        NOP();
//    }
//    SCL = 0;
//    
//}
//
//static unsigned char i2cGetAck(void)
//{
//    SDACTRL = RELEASE;
//    NOP();
//    unsigned char b = 0;
//    SCL = 0;
//    NOP();
////    asm("NOP");
////    asm("NOP");
//    
//    SCL = 1;
//    NOP();
////    asm("NOP");
////    asm("NOP");
//
//    b = SDA;
//    SCL = 0;
//    
//    return b;
//}
//
//static unsigned char i2cRcvSlave(void)
//{
//	unsigned char b;
//    SDACTRL = RELEASE;
//    for(int i = 0; i < 8; i++) 
//    {
//        SCL = 0;
//        NOP();
//        b = b << 1;
//        SCL = 1;
//        if(SDA == 1) {
//            b +=1;
//        }
//        NOP();
//        asm("NOP");
//    }
//	return b;
//}
//
//static void i2cSendAck(void)
//{
//    SCL = 0;
//    SDACTRL = TAKE;
//    SCL = 1;
//    asm("NOP");
//    SDACTRL = RELEASE;
//    SCL = 0;
//}
//
//static void i2cSendNack(void)
//{
//	SDACTRL = RELEASE;
//    SDA=1;
//    asm("NOP");
//    SDA=0;
//    
//}
//
//static void i2cSendStop(void)
//{
//    SDA=1;
//    SDACTRL = TAKE;
//    SDA = 0;
//    SCL = 1;
//    asm("NOP");
//    SDA = 1;
//    SDACTRL = RELEASE;
//}
//
//static void i2cWriteMode(void)
//{
//    SCL=0;
//    SDACTRL = TAKE;
//    SDA=0;
//    SCL=1;
//    asm("NOP");
//    SCL=0;
//}
//static void i2cReadMode(void)
//{
//    SCL=0;
//    SDACTRL = TAKE;
//    SDA=1;
//    SCL=1;
//    asm("NOP");
//    SCL=0;
//}
//
//// 0 -> write
//// 1-> read
//
////  ======  Public BitBanging  ===============================================================
//
//I2Cerror i2cReadSlaveRegister_BB(unsigned char devAddW, unsigned char regAdd, unsigned char *reg)
//{
//	i2cMasterStrt();
//    NOP();
//    asm("NOP");
//    SDACTRL = TAKE;
//    // salve address + write  
//    devAddW = devAddW << 1;
//    i2cMasterSnd(devAddW);
//    if(i2cGetAck())
//    {
//        return ACK;
//    }
//    i2cMasterSnd(regAdd);
//    if(i2cGetAck())
//    {
//        return BAD_REG;
//    }
//    i2cMasterStrt();
//    NOP();
//    asm("NOP");
////    asm("NOP");
//    devAddW++;
//    i2cMasterSnd(devAddW);
////    i2cReadMode();
//    if(i2cGetAck())
//    {
//        return BAD_ADDR;
//    }
//    asm("NOP");
//    asm("NOP");
////    asm("NOP");
////    asm("NOP");
//    *reg = i2cRcvSlave();
//    i2cSendNack();
//    asm("NOP");
//    asm("NOP");
//    i2cSendStop();
//    return OK;
//    
//}
//
//unsigned char i2cReadSlaveMultRegister_BB(unsigned char devAddW, unsigned char regAdd, int len, unsigned char *buf)
//{
//    
//	i2cMasterStrt();
////    asm("NOP");
////    asm("NOP");
//    NOP();
//    SDACTRL = TAKE;
//    devAddW = devAddW << 1;
//    i2cMasterSnd(devAddW);
//    if(i2cGetAck())
//    {
//        return ACK;
//    }
//    i2cMasterSnd(regAdd);
//    if(i2cGetAck())
//    {
//        return ACK;
//    }
//    i2cMasterStrt();
//    NOP();
////    asm("NOP");
////    asm("NOP");
//    devAddW++;
//    i2cMasterSnd(devAddW);
//    if(i2cGetAck())
//    {
//        return ACK;
//    }
//    asm("NOP");
//    asm("NOP");
//    NOP();
////    asm("NOP");
////    asm("NOP");
//    for(int i = 0; i < len; i ++) 
//    {
//        buf[i] = i2cRcvSlave();
//        asm("NOP");
//        i2cSendAck();
//    }
//    i2cSendNack();
//    asm("NOP");
//    asm("NOP");
//    i2cSendStop();
//    return OK;
//}
//
//unsigned char i2cWriteSlave_BB(unsigned char devAddW, unsigned char regAdd, unsigned char data)
//{
//	i2cMasterStrt();
//    NOP();
//    asm("NOP");
//    asm("NOP");
//    devAddW = devAddW << 1;
//    i2cMasterSnd(devAddW);
//    if(i2cGetAck())
//    {
//        return ACK;
//    }
////    NOP();
//    i2cMasterSnd(regAdd);
//    if(i2cGetAck())
//    {
//        return ACK;
//    }
////    NOP();
//    i2cMasterSnd(data);
//    if(i2cGetAck())
//    {
//        return ACK;
//    }
//    NOP();
//    i2cSendStop();
//    return OK;
//    
//}
//
//
//void ACCEL_INIT()
//{
//    i2cWriteSlave_BB(ACCEL_ADDR, 0x2D, 8);      //Start Measuring Acceleration
//    i2cWriteSlave_BB(ACCEL_ADDR, 0x31, 2);      //range to +- 8g
//}
//
//
//bool isDeviceShaken()
//{
//    I2Cerror rc;
//    int status = 0;
//    unsigned int x, y, z;
//    unsigned char xyz[6] = {0};
//    
//    rc = i2cReadSlaveRegister_BB(ACCEL_ADDR, 0x30, &status);
//    if(rc) return false;
//    if(!(128 & status)) // DATA_READY is nor set, there is no data 
//        return false;; 
//
//    rc = i2cReadSlaveMultRegister_BB(ACCEL_ADDR, 0x32, 6, xyz);
//    if(rc) return false;
//    x = xyz[0]+(xyz[1]&3)*256;  //2xbytes ==> word
//    y = xyz[2]+(xyz[3]&3)*256;
//    z = xyz[4]+(xyz[5]&3)*256;
//    
//    double norm = sqrt(x*x + y*y + z*z);
//    if(norm > 200) {
//        return true;
//    }
//    return false;
//    
//}
