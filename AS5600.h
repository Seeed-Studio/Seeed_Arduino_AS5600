#ifndef _AS5600_H_
#define _AS5600_H_

#include "Arduino.h"
#include <Wire.h>

#define _AS5600_CPR 4096
#define _AS5600_CPR_HALF 2048

#define _AS5600_ADDR 0x36

#define _ZMCOAddress 0x00
#define _ZPOSAddressMSB 0x01
#define _ZPOSAddressLSB 0x02
#define _MPOSAddressMSB 0x03
#define _MPOSAddressLSB 0x04
#define _MANGAddressMSB 0x05
#define _MANGAddressLSB 0x06
#define _CONFAddressMSB 0x07
#define _CONFAddressLSB 0x08
#define _RAWANGLEAddressMSB 0x0C
#define _RAWANGLEAddressLSB 0x0D
#define _ANGLEAddressMSB 0x0E
#define _ANGLEAddressLSB 0x0F
#define _STATUSAddress 0x0B
#define _AGCAddress 0x1A
#define _MAGNITUDEAddressMSB 0x1B
#define _MAGNITUDEAddressLSB 0x1C
#define _BURNAddress 0xFF

class AS5600
{
  public:
    AS5600();
    long getPosition();
    int getAngle();
    int getStatus();
    void setZero();

  private:
    long output, last_output;
    int revolutions, offset, first;

    long _msb;
    long _lsb;
    long _getRegisters2(byte registerMSB, byte registerLSB);
    int _getRegister(byte register1);
};

#endif
