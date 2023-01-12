#include <TinyGPS++.h>


// Create a TinyGPS++ object to encode data from gps module
TinyGPSPlus gps;

constexpr int PRECISION = 5;

void gps_setup() {
  debugln("Setting up gps");
  Serial1.begin(9600);
  debugln("Setup gps done");
}

void gps_read() {
  while (Serial1.available() > 0) {
    delay(3);
    char c = Serial1.read();
    if (gps.encode(c)) {
      String gpsMessage = gps_info();
      gps_display_info();
      if (gpsMessage.length() > 0) {
        lora_uplink(gpsMessage);
        return;
      }
    }
  }
  debugln("\nFinished receive");

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    debugln("No GPS detected");
  }
}

String gps_info() {
  String message = "";
  String json = "";

  if (message.length() == 0) {
    message.concat("{");
  } else {
    message.concat(",");
  }
  json = "\"s\":";
  json.concat(gps.satellites.value());
  message.concat(json);

  if (gps.location.isValid()) {
    if (message.length() == 0) {
      message.concat("{");
    } else {
      message.concat(",");
    }

    json = "\"l\": {\"lat\":";
    json.concat(String(gps.location.lat(), PRECISION));
    json.concat(",\"lng\":");
    json.concat(String(gps.location.lng(), PRECISION));
    json.concat(",\"alt\":");
    json.concat(String(gps.altitude.meters(), PRECISION));
    json.concat("}");

    message.concat(json);
  } else {
    if (message.length() == 0) {
      message.concat("{");
    } else {
      message.concat(",");
    }
    json = "\"l\": {\"lat\":";
    json.concat(String(38.704510, PRECISION));
    json.concat(",\"lng\":");
    json.concat(String(-9.202282, PRECISION));
    json.concat(",\"alt\":");
    json.concat(String(78.20, PRECISION));
    json.concat("}");

    message.concat(json);
  }

  if (message.length() > 0) {
    message.concat("}");
  }
  return message;
}


void gps_display_info() {
  debug("N Satelites: ");
  debugln(gps.satellites.value());
  if (gps.location.isValid()) {
#if DEBUG
    debug("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    debug("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    debug("Altitude: ");
    debugln(gps.altitude.meters());
#endif
  } else {
    debugln("Location: Not Available");
  }

  debug("Date: ");
  if (gps.date.isValid()) {
    debug(gps.date.month());
    debug("/");
    debug(gps.date.day());
    debug("/");
    debugln(gps.date.year());
  } else {
    debugln("Date Not Available");
  }

  debug("Time: ");
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) debug(F("0"));
    debug(gps.time.hour());
    debug(":");
    if (gps.time.minute() < 10) debug(F("0"));
    debug(gps.time.minute());
    debug(":");
    if (gps.time.second() < 10) debug(F("0"));
    debug(gps.time.second());
    debug(".");
    if (gps.time.centisecond() < 10) debug(F("0"));
    debugln(gps.time.centisecond());
  } else {
    debugln("Time Not Available");
  }
}