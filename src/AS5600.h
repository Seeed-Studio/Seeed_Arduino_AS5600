
/****************************************************
  AMS 5600 class for Arduino platform
  Author: Tom Denton
  Date: 15 Dec 2014
  File: AMS_5600.h 
  Version 1.00
  www.ams.com
   
  Description:  This class has been designed to
  access the AMS 5600 “potuino” shield.
***************************************************/

// updated jan 2022 by isc - read two bytes together

// datasheet: https://ams.com/documents/20143/36005/AS5600_DS000365_5-00.pdf

#ifndef AMS_5600_h
#define AMS_5600_h

#include <Arduino.h>

class AMS_5600
{
public:
  AMS_5600(void);
  int getAddress();

  word setMaxAngle(word newMaxAngle = -1);
  word getMaxAngle();

  word setStartPosition(word startAngle = -1);
  word getStartPosition();

  word setEndPosition(word endAngle = -1);
  word getEndPosition();

  word getRawAngle();
  word getScaledAngle();

  int detectMagnet();
  int getMagnetStrength();
  int getAgc();
  word getMagnitude();
  word getConf();
  void setConf(word _conf);

  int getBurnCount();
  int burnAngle();
  int burnMaxAngleAndConfig();
  void setOutPut(uint8_t mode);

private:
  int _ams5600_Address;

  word _rawStartAngle;
  word _zPosition;
  word _rawEndAngle;
  word _mPosition;
  word _maxAngle;

  // single byte registers
  int _addr_status; // magnet status
  int _addr_agc;    // automatic gain control
  int _addr_burn;   // permanent burning of configs (zpos, mpos, mang, conf)
  int _addr_zmco;   // number of times zpos/mpos has been permanently burned
                    // zpos/mpos can be permanently burned 3x
                    // mang/conf can be burned only once

  // double byte registers, specify starting address (lower addr, but higher byte data)
  // addr   = upper byte of data (MSB), only bits 0:3 are used
  // addr+1 = lower byte of data (LSB)
  int _addr_zpos;      // zero position (start)
  int _addr_mpos;      // maximum position (stop)
  int _addr_mang;      // maximum angle
  int _addr_conf;      // configuration
  int _addr_raw_angle; // raw angle
  int _addr_angle;     // mapped angle
  int _addr_magnitude; // magnitude of internal CORDIC

  int readOneByte(int in_adr);
  word readTwoBytesSeparately(int addr_in);
  word readTwoBytesTogether(int addr_in);
  void writeOneByte(int adr_in, int dat_in);
};
#endif