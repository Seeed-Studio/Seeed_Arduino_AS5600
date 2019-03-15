#include <AS5600.h>

AS5600 encoder;

unsigned long tim = 0;
long enc, lenc = 0;

void setup()
{
  Serial.begin(9600);
  encoder.setZero();
}

void loop()
{

  enc = encoder.getPosition();

  if (enc != lenc)
  {
    Serial.println(enc);
    lenc = enc;
  }
  if (abs(enc) > 20000)
  {
    encoder.setZero();
  }
}
