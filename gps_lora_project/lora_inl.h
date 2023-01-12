

#include "arduino_secrets.h"
#include <MKRWAN.h>

LoRaModem modem;

void lora_setup() {
  // put your setup code here, to run once:
  debugln("Setting up Lora");
  Serial.begin(115200);
  while (!Serial);
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    debugln("Failed to start module; EXITING!!");
    while (1) {}
  };
  debug("Your module version is: ");
  debugln(modem.version());
  debug("Your device EUI is: ");
  debugln(modem.deviceEUI());

  int connected = modem.joinOTAA(SECRET_APP_EUI, SECRET_APP_KEY);
  while (!connected){
    debugln("Something went wrong; are you indoor? Trying again");
    connected = modem.joinOTAA(SECRET_APP_EUI, SECRET_APP_KEY);
  }
  debugln("Sucessfuly connected to Lora Modem");

  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
}


void lora_loop() {
  float h = 10.0;
  float t = 10.2;
  String msg{"" + String(t) + "," + String(h)};
  #if DEBUG
    Serial.print("Sending: " + msg + " - ");
    for (unsigned int i = 0; i < msg.length(); i++) {
      Serial.print(msg[i] >> 4, HEX);
      Serial.print(msg[i] & 0xF, HEX);
      Serial.print(" ");
    }
    Serial.println(""); 
  #endif
  lora_uplink(msg);
  delay(60*1000);
}

void lora_uplink(String msg){

  int err;
  modem.beginPacket();
  #if DEBUG
    Serial.print("Sending: " + msg + " - ");
    for (unsigned int i = 0; i < msg.length(); i++) {
      Serial.print(msg[i] >> 4, HEX);
      Serial.print(msg[i] & 0xF, HEX);
      Serial.print(" ");
    }
    Serial.println("");
  #endif
  modem.print(msg);
  err = modem.endPacket(true);
  if (err > 0) {
    debugln("Message sent correctly!");
  } else {
    debugln("Error sending message :(you may send a limited amount of messages per minute, depending on the signal strength it may vary from 1 message every couple of seconds to 1 message every minute)");
  }
  delay(1000);
  lora_downlink();
}


void lora_downlink(){
   delay(1000);
   if (!modem.available()) {
    debugln("No downlink message received at this time.");
    return;
  }
  char rcv[64];
  int i = 0;
  while (modem.available()) {
    rcv[i++] = (char)modem.read();
  }
  #if DEBUG
    Serial.print("Received: ");
    for (unsigned int j = 0; j < i; j++) {
      Serial.print(rcv[j] >> 4, HEX);
      Serial.print(rcv[j] & 0xF, HEX);
      Serial.print(" ");
    }
    Serial.println();
  #endif
}
