#include "gps.h"
#include "lora.h"

void setup()
{
  gps_setup();
  lora_setup();
}

void loop()
{
  gps_read();
  
  delay(1000);
}



#include "gps_inl.h"
#include "lora_inl.h"





