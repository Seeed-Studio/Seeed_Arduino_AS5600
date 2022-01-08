
/****************************************************
  AMS 5600 class for Arduino platform
  Author: Tom Denton
  Date: 15 Dec 2014 
  File: AMS_5600.cpp
  Version 1.00
  www.ams.com
   
  Description:  This class has been designed to
  access the AMS 5600 “potuino” shield.
*****************************************************/

#include "Arduino.h"
#include "AS5600.h"
#include "Wire.h"

/****************************************************
  Method: AMS_5600
  In: none
  Out: none
  Description: constructor class for AMS 5600
*****************************************************/
AMS_5600::AMS_5600()
{
  /* set i2c address */
  _ams5600_Address = 0x36;

  /* load register values */
  /* c++ class forbids pre loading of variables */
  _zmco = 0x00;
  _stat = 0x0b;
  _agc = 0x1a;
  _burn = 0xff;

  _addr_zpos    = 0x01; // 0x02 - LSB
  _addr_mpos    = 0x03; // 0x04 - LSB
  _addr_mang     = 0x05; // 0x06 - LSB
  _addr_conf    = 0x07; // 0x08 - LSB
  _addr_raw_ang = 0x0c; // 0x0d - LSB
  _addr_ang     = 0x0e; // 0x0f - LSB
  _addr_mag     = 0x1b; // 0x1c - LSB
}

/* mode = 0, output PWM, mode = 1 output analog (full range from 0% to 100% between GND and VDD */
void AMS_5600::setOutPut(uint8_t mode)
{
  int _conf_lo = _addr_conf+1; // lower byte address
  uint8_t config_status;
  config_status = readOneByte(_conf_lo);
  if (mode == 1)
  {
    config_status = config_status & 0xcf;
  }
  else
  {
    uint8_t config_status;
    config_status = readOneByte(_conf_lo);
    if (mode == 1)
    {
      config_status = config_status & 0xcf;
    }
    else
    {
      config_status = config_status & 0xef;
    }
    writeOneByte(_conf_lo, lowByte(config_status));
  }
}

/****************************************************
  Method: AMS_5600
  In: none
  Out: i2c address of AMS 5600
  Description: returns i2c address of AMS 5600
****************************************************/
int AMS_5600::getAddress()
{
  return _ams5600_Address;
}

/*******************************************************
  Method: setMaxAngle
  In: new maximum angle to set OR none
  Out: value of max angle register
  Description: sets a value in maximum angle register.
  If no value is provided, method will read position of
  magnet.  Setting this register zeros out max position
  register.
*******************************************************/
word AMS_5600::setMaxAngle(word newMaxAngle)
{
  word retVal;
  if (newMaxAngle == -1)
  {
    _maxAngle = getRawAngle();
  }
  else
    _maxAngle = newMaxAngle;

  writeOneByte(_addr_mang, highByte(_maxAngle));
  delay(2);
  writeOneByte(_addr_mang+1, lowByte(_maxAngle));
  delay(2);

  retVal = readTwoBytes(_addr_mang);
  return retVal;
}

/*******************************************************
  Method: getMaxAngle
  In: none
  Out: value of max angle register
  Description: gets value of maximum angle register.
*******************************************************/
word AMS_5600::getMaxAngle()
{
  return readTwoBytes(_addr_mang);
}

/*******************************************************
  Method: setStartPosition
  In: new start angle position
  Out: value of start position register
  Description: sets a value in start position register.
  If no value is provided, method will read position of
  magnet.  
*******************************************************/
word AMS_5600::setStartPosition(word startAngle)
{
  if (startAngle == -1)
  {
    _rawStartAngle = getRawAngle();
  }
  else
    _rawStartAngle = startAngle;

  writeOneByte(_addr_zpos, highByte(_rawStartAngle));
  delay(2);
  writeOneByte(_addr_zpos+1, lowByte(_rawStartAngle));
  delay(2);
  _zPosition = readTwoBytes(_addr_zpos);

  return (_zPosition);
}

/*******************************************************
  Method: getStartPosition
  In: none
  Out: value of start position register
  Description: gets value of start position register.
*******************************************************/
word AMS_5600::getStartPosition()
{
  return readTwoBytes(_addr_zpos);
}

/*******************************************************
  Method: setEndtPosition
  In: new end angle position
  Out: value of end position register
  Description: sets a value in end position register.
  If no value is provided, method will read position of
  magnet.  
*******************************************************/
word AMS_5600::setEndPosition(word endAngle)
{
  if (endAngle == -1)
    _rawEndAngle = getRawAngle();
  else
    _rawEndAngle = endAngle;

  writeOneByte(_addr_mpos, highByte(_rawEndAngle));
  delay(2);
  writeOneByte(_addr_mpos+1, lowByte(_rawEndAngle));
  delay(2);
  _mPosition = readTwoBytes(_addr_mpos);

  return (_mPosition);
}

/*******************************************************
  Method: getEndPosition
  In: none
  Out: value of end position register
  Description: gets value of end position register.
*******************************************************/
word AMS_5600::getEndPosition()
{
  word retVal = readTwoBytes(_addr_mpos);
  return retVal;
}

/*******************************************************
  Method: getRawAngle
  In: none
  Out: value of raw angle register
  Description: gets raw value of magnet position.
  start, end, and max angle settings do not apply
*******************************************************/
word AMS_5600::getRawAngle()
{
  return readTwoBytes(_addr_raw_ang);
}

/*******************************************************
  Method: getScaledAngle
  In: none
  Out: value of scaled angle register
  Description: gets scaled value of magnet position.
  start, end, or max angle settings are used to 
  determine value
*******************************************************/
word AMS_5600::getScaledAngle()
{
  return readTwoBytes(_addr_ang);
}

/*******************************************************
  Method: detectMagnet
  In: none
  Out: 1 if magnet is detected, 0 if not
  Description: reads status register and examines the 
  MH bit
*******************************************************/
int AMS_5600::detectMagnet()
{
  int magStatus;
  int retVal = 0;
  /*0 0 MD ML MH 0 0 0*/
  /* MD high = magnet detected*/
  /* ML high = AGC Maximum overflow, magnet to weak*/
  /* MH high = AGC minimum overflow, Magnet to strong*/
  magStatus = readOneByte(_stat);

  if (magStatus & 0x20)
    retVal = 1;

  return retVal;
}

/*******************************************************
  Method: getMagnetStrength
  In: none
  Out: 0 if no magnet is detected
       1 if magnet is to weak
       2 if magnet is just right
       3 if magnet is to strong
  Description: reads status register andexamins the MH,ML,MD bits
*******************************************************/
int AMS_5600::getMagnetStrength()
{
  int magStatus;
  int retVal = 0;
  /*0 0 MD ML MH 0 0 0*/
  /* MD high = magnet detected */
  /* ML high = AGC Maximum overflow, magnet to weak*/
  /* MH high = AGC minimum overflow, Magnet to strong*/
  magStatus = readOneByte(_stat);
  if (detectMagnet() == 1)
  {
    retVal = 2; /*just right */
    if (magStatus & 0x10)
      retVal = 1; /*to weak */
    else if (magStatus & 0x08)
      retVal = 3; /*to strong */
  }

  return retVal;
}

/*******************************************************
  Method: get Agc
  In: none
  Out: value of AGC register
  Description: gets value of AGC register.
*******************************************************/
int AMS_5600::getAgc()
{
  return readOneByte(_agc);
}

/*******************************************************
  Method: getMagnitude
  In: none
  Out: value of magnitude register
  Description: gets value of magnitude register.
*******************************************************/
word AMS_5600::getMagnitude()
{
  return readTwoBytes(_addr_mag);
}

/*******************************************************
  Method: getConf
  In: none
  Out: value of CONF register 
  Description: gets value of CONF register.
*******************************************************/
word AMS_5600::getConf()
{
  return readTwoBytes(_addr_conf);
}

/*******************************************************
  Method: setConf
  In: value of CONF register
  Out: none
  Description: sets value of CONF register.
*******************************************************/
void AMS_5600::setConf(word _conf)
{
  writeOneByte(_addr_conf, highByte(_conf));
  delay(2);
  writeOneByte(_addr_conf+1, lowByte(_conf));
  delay(2);
}

/*******************************************************
  Method: getBurnCount
  In: none
  Out: value of zmco register
  Description: determines how many times chip has been
  permanently written to. 
*******************************************************/
int AMS_5600::getBurnCount()
{
  return readOneByte(_zmco);
}

/*******************************************************
  Method: burnAngle
  In: none
  Out: 1 success
      -1 no magnet
      -2 burn limit exceeded
      -3 start and end positions not set (useless burn)
  Description: burns start and end positions to chip.
  THIS CAN ONLY BE DONE 3 TIMES
*******************************************************/
int AMS_5600::burnAngle()
{
  int retVal = 1;
  _zPosition = getStartPosition();
  _mPosition = getEndPosition();
  _maxAngle = getMaxAngle();

  if (detectMagnet() == 1)
  {
    if (getBurnCount() < 3)
    {
      if ((_zPosition == 0) && (_mPosition == 0))
        retVal = -3;
      else
        writeOneByte(_burn, 0x80);
    }
    else
      retVal = -2;
  }
  else
    retVal = -1;

  return retVal;
}

/*******************************************************
  Method: burnMaxAngleAndConfig
  In: none
  Out: 1 success
      -1 burn limit exceeded
      -2 max angle is to small, must be at or above 18 degrees
  Description: burns max angle and config data to chip.
  THIS CAN ONLY BE DONE 1 TIME
*******************************************************/
int AMS_5600::burnMaxAngleAndConfig()
{
  int retVal = 1;
  _maxAngle = getMaxAngle();

  if (getBurnCount() == 0)
  {
    if (_maxAngle * 0.087 < 18)
      retVal = -2;
    else
      writeOneByte(_burn, 0x40);
  }
  else
    retVal = -1;

  return retVal;
}

/*******************************************************
  Method: readOneByte
  In: register to read
  Out: data read from i2c
  Description: reads one byte register from i2c
*******************************************************/
int AMS_5600::readOneByte(int in_adr)
{
  int retVal = -1;
  Wire.beginTransmission(_ams5600_Address);
  Wire.write(in_adr);
  Wire.endTransmission();
  Wire.requestFrom(_ams5600_Address, 1);
  while (Wire.available() == 0)
    ;
  retVal = Wire.read();

  return retVal;
}

/*******************************************************
  Method: readTwoBytes
  In: two registers to read
  Out: data read from i2c as a word
  Description: reads two bytes register from i2c
*******************************************************/
word AMS_5600::readTwoBytes(int addr_in)
{
  /* Read 2 Bytes */
  Wire.beginTransmission(_ams5600_Address);
  Wire.write(addr_in);
  Wire.endTransmission();
  Wire.requestFrom(_ams5600_Address, 2);
  while (Wire.available() < 2)
    ;
  
  int high = Wire.read();
  int low = Wire.read();
  return ( high << 8 ) | low;
}

/*******************************************************
  Method: writeOneByte
  In: address and data to write
  Out: none
  Description: writes one byte to a i2c register
*******************************************************/
void AMS_5600::writeOneByte(int adr_in, int dat_in)
{
  Wire.beginTransmission(_ams5600_Address);
  Wire.write(adr_in);
  Wire.write(dat_in);
  Wire.endTransmission();
}

/**********  END OF AMS 5600 CALSS *****************/
