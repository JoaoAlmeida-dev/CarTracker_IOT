#include "gps.h"
#include "lora.h"
#include "axis.h"
#include "buzzer.h"
#include <time.h>


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
constexpr int ALARM_MOVEMENT_THRESHOLD = 50;

// variables will change:
uint8_t newButton = 0; // variable for reading the pushbutton status
uint8_t oldButton = 0;

//Controls the whole app state, with two states available:
// * when appState==True the app is in GPS state, it will measure current gps location and attempt to send it to backend via LoRaWan.
// * when appState == False, the app is in security/gyroMode, the gyro sensor is activated and if any sudden movements are detected, alarm starts playing and alert message is sent via LoRaWan
bool appState = false;

unsigned long current_time = 0;
unsigned long previous_time_gps = 0;
constexpr unsigned long GPS_WAITTIME = 5UL*60UL*1000UL;
unsigned long last_alarm = 0;
constexpr unsigned long ALARM_WAITTIME = 10 * 1000UL;

inline void blink_built_in(){
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
    
  randomSeed(analogRead(5));

  blink_built_in();
  gps_setup();
  blink_built_in();
  button_setup();
  blink_built_in();
  axis_setup();
  blink_built_in();
  lora_setup();
  blink_built_in();
}

void loop()
{
  delay(1000);
  button_update_state();
  current_time = millis();
  
  // if(current_time - previous_time_downlink > DOWNLINK_WAITTIME ){
  //     debug("Reading downlink message from lora every ");
  //     debug(DOWNLINK_WAITTIME);
  //     debugln(" milliseconds");
  //     previous_time_downlink = current_time;
  //     lora_downlink();
  // }

  if (appState)
  {
    //debugln("GPS state");
    if(current_time - previous_time_gps > GPS_WAITTIME ){
      previous_time_gps = current_time;
      debug("Reading GPS data and sending it to Lora every ");
      debug(GPS_WAITTIME);
      debugln(" milliseconds");
      gps_read();
    }
  }
  else
  {
    //debugln("Gyro state");
    if(axis_loop(ALARM_MOVEMENT_THRESHOLD))
    {
      debugln("***Alert***");
      if(current_time - last_alarm  > ALARM_WAITTIME)
      {
        last_alarm = current_time;
        
        String alertMessage {"{\"a\":"};
        alertMessage.concat(true);
        alertMessage.concat("}");
        debug("Sending Alert message:");
        debugln(alertMessage);
        
        handle_lora_message(alertMessage);
        buzzer_play(BUZZ_PIN,BUZZ_TEMPO);
      }
    }
  }
}

//Where messages are sent through lora and the corresponding downlink message is parsed and corresponding changes are made
inline void handle_lora_message(String message){
  String received_message = lora_uplink(message);
  
  if (received_message.indexOf("state") > -1) {
    set_app_state( received_message.indexOf("1") > -1);
  }
}

inline void set_app_state(bool new_state){
  appState = new_state;
  debug("changed app state to: ");
  debugln(appState);
  String appStateMessage {"{\"s\":"};
  appStateMessage.concat(appState);
  appStateMessage.concat("}");

  handle_lora_message(appStateMessage);
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
      debugln("BUTTON turned on");
      digitalWrite(LED_BUILTIN, HIGH);
      set_app_state(true);
    }
    else
    {
      // OFF
      debugln("BUTTON turned off");
      digitalWrite(LED_BUILTIN, LOW);
      set_app_state(false);
    }
  }
  oldButton = newButton;
}

inline void button_setup()
{
  debugln("Setting up button");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  debugln("Button setup done");
}

#include "gps_inl.h"
#include "lora_inl.h"
#include "axis_inl.h"
#include "buzzer_inl.h"