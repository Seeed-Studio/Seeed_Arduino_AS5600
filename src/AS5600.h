
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

  /* Registers */
  int _zmco;
  int _stat;
  int _agc;
  int _burn;

  // specify starting address
  // addr   = upper byte of data
  // addr+1 = lower byte of data
  int _addr_zpos; // only bits 0:3 of upper byte is used
  int _addr_mpos; // only bits 0:3 of upper byte is used
  int _addr_mang;  // only bits 0:3 of upper byte is used
  int _addr_conf;
  int _addr_raw_ang;
  int _addr_ang;
  int _addr_mag;

  int readOneByte(int in_adr);
  word readTwoBytes(int addr_in);
  void writeOneByte(int adr_in, int dat_in);
};
#endif