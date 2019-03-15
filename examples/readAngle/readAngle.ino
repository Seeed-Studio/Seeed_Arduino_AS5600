#include <AS5600.h>

AS5600 encoder;

int ang, lang = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{

  ang = encoder.getAngle();

  if (ang != lang)
  {
    Serial.println(ang);
    lang = ang;
  }
}
