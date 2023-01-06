#include "gps.h"
#include "lora.h"

// constants won't change. They're used here to set pin numbers:
constexpr uint8_t BUTTON_PIN = 2; // the number of the pushbutton pin
constexpr uint8_t BUZZ_PIN = 13;  // the number of the buzzer pin

// variables will change:
uint8_t newButton = 0; // variable for reading the pushbutton status
uint8_t oldButton = 0;
bool ledState = false;

void setup()
{
  // receive pins on setup methods
  //gps_setup();
  //lora_setup();
  button_setup();
}

void loop()
{
  button_update_state();
  if (ledState)
  {
    Serial.println("GPS state");
    //gps_read();
  }
  else
  {
    Serial.println("Gyro state");
  }
  delay(1000);
}

inline void button_update_state(){
  newButton = digitalRead(BUTTON_PIN);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (oldButton == 0 && newButton == 1)
  {
    if (!ledState)
    {
      // ON
      // tone(buzzpin, 2750);
      Serial.println("BUTTON turned on");
      digitalWrite(LED_BUILTIN, HIGH);
      // ledState = true;
    }
    else
    {
      // OFF
      Serial.println("BUTTON turned off");
      digitalWrite(LED_BUILTIN, LOW);
      // ledState = false;
    }
    ledState = !ledState;
  }
  oldButton = newButton;
}

inline void button_setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Button setup done");
}

#include "gps_inl.h"
#include "lora_inl.h"