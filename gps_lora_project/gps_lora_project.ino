#include "gps.h"
#include "lora.h"
#include "axis.h"
#include "buzzer.h"


#define DEBUG 1 
#if DEBUG
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif


// constants won't change. They're used here to set pin numbers:
constexpr uint8_t BUTTON_PIN = 2; // the number of the pushbutton pin
constexpr uint8_t BUZZ_PIN = 1;  // the number of the buzzer pin
constexpr uint8_t BUZZ_TEMPO = 120;  // the number of the buzzer pin
constexpr int ALARM_MOVEMENT_THRESHOLD = 20;

// variables will change:
uint8_t newButton = 0; // variable for reading the pushbutton status
uint8_t oldButton = 0;

//Controls the whole app state, with two states available:
// * when appState==True the app is in GPS state, it will measure current gps location and attempt to send it to backend via LoRaWan.
// * when appState == False, the app is in security/gyroMode, the gyro sensor is activated and if any sudden movements are detected, alarm starts playing and alert message is sent via LoRaWan
bool appState = false;
int lastAlarm = millis();

void setup()
{
  // receive pins on setup methods
  //gps_setup();
  //lora_setup();
  button_setup();
  axis_setup();
}

void loop()
{
  button_update_state();
  if (appState)
  {
    debugln("GPS state");
    //gps_read();
    delay(1000);
  }
  else
  {
    debugln("Gyro state");
    int current_time = millis();
    if(axis_loop(ALARM_MOVEMENT_THRESHOLD))
    {
      debugln("***Alert***");
      if(current_time - lastAlarm  > 1000)
      {
        lastAlarm = current_time;
        buzzer_play(BUZZ_PIN,BUZZ_TEMPO);
      }
    }
    delay(200);
  }
}
//Directly changes the ledState variable according to if the button was pushed or not
inline void button_update_state(){
  newButton = digitalRead(BUTTON_PIN);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (oldButton == 0 && newButton == 1)
  {
    if (!appState)
    {
      // ON
      // tone(buzzpin, 2750);
      debugln("BUTTON turned on");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      // OFF
      debugln("BUTTON turned off");
      digitalWrite(LED_BUILTIN, LOW);
    }
    appState = !appState;
  }
  oldButton = newButton;
}

inline void button_setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  debug("Button setup done");
}

#include "gps_inl.h"
#include "lora_inl.h"
#include "axis_inl.h"
#include "buzzer_inl.h"