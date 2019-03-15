#include "Arduino.h"
#include "AS5600.h"

AS5600::AS5600()
{
    Wire.begin();
    revolutions = 0;
    offset = 0;
    first = 1;
}

long AS5600::getPosition()
{
    output = _getRegisters2(_RAWANGLEAddressMSB, _RAWANGLEAddressLSB);
    if (first)
    {
        last_output = output;
        first = 0;
    }
    if ((output - last_output) < -_AS5600_CPR_HALF)
    {
        revolutions++;
    }
    else if ((output - last_output) > _AS5600_CPR_HALF)
    {
        revolutions--;
    }
    last_output = output;
    return (output - offset) + _AS5600_CPR * revolutions;
}

int AS5600::getAngle()
{
    return _getRegisters2(_ANGLEAddressMSB, _ANGLEAddressLSB);
}

int AS5600::getStatus()
{
    return _getRegister(_STATUSAddress) & 0b00111000;
}

void AS5600::setZero()
{
    revolutions = 0;
    offset = _getRegisters2(_RAWANGLEAddressMSB, _RAWANGLEAddressLSB);
}

int AS5600::_getRegister(byte register1)
{
    Wire.beginTransmission(_AS5600_ADDR);
    Wire.write(register1);
    Wire.endTransmission();

    Wire.requestFrom(_AS5600_ADDR, 1);

    if (Wire.available() <= 1)
    {
        _msb = Wire.read();
    }

    return _msb;
}

long AS5600::_getRegisters2(byte registerMSB, byte registerLSB)
{
    _lsb = 0;
    _msb = 0;

    Wire.beginTransmission(_AS5600_ADDR);
    Wire.write(registerMSB);
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(_AS5600_ADDR, 1);

    if (Wire.available() <= 1)
    {
        _msb = Wire.read();
    }

    Wire.requestFrom(_AS5600_ADDR, 1);

    Wire.beginTransmission(_AS5600_ADDR);
    Wire.write(registerLSB);
    Wire.endTransmission();

    if (Wire.available() <= 1)
    {
        _lsb = Wire.read();
    }

    return (_lsb) + (_msb & 0x0F) * 256;
}
