#include <TinyGPS++.h>


// Create a TinyGPS++ object to encode data from gps module
TinyGPSPlus gps;

constexpr int PRECISION = 10;

void gps_setup()
{
  Serial1.begin(9600);
  Serial.println("Setup gps done");
}

void gps_read(){
  while (Serial1.available() > 0 ){
    delay(3);
    char c = Serial1.read();
    if(gps.encode(c)){
      String gpsMessage = gps_info();
      gps_display_info();
      if(gpsMessage.length() > 0)
      {
        Serial.print("Sending: " + gpsMessage + " - ");
        for (unsigned int i = 0; i < gpsMessage.length(); i++) {
          Serial.print(gpsMessage[i] >> 4, HEX);
          Serial.print(gpsMessage[i] & 0xF, HEX);
          Serial.print(" ");
        }
        Serial.println("");


        lora_send_msg(gpsMessage);
        delay(60*1000 + 0);

      }
    }
  }
  Serial.println("\nFinished receive");
  
  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
  }  
}

  // Sending Gps message by Lora: {"location": {"lat":38.70,"lng":-9.20,"alt":0.00},"date": {"day":22,"month":11,"year":2022},"time": {"hour":16,"minute":10,"second":3,"centisecond":0},}
/*
N Satelites: 0
Latitude: 38.704510
Longitude: -9.202282
Altitude: 78.20
Date: 11/22/2022
Time: 16:56:05.00
Sending Gps message by Lora: {"satelites":0,"location": {"lat":38.70,"lng":-9.20,"alt":78.20},"date": {"day":22,"month":11,"year":2022},"time": {"hour":16,"minute":56,"second":5,"centisecond":0},}

*/
  /*
  {
   "location":{
      "lat":10,
      "lng":13,
      "alt":20
   },
   "date":{
      "month":3,
      "day":1,
      "year":2000
   },
   "time":{
      "hour":19,
      "minute":30,
      "second":35
   }
  }

function Decoder(bytes) {
  var result = "";
  for (var i = 0; i < bytes.length; i++) {
    result += String.fromCharCode(parseInt(bytes[i]));
  }
  
  //return { var1: result };
  return JSON.parse(result);
}

  */
String gps_info(){
  String message = "";
  String json = "";

  if(message.length() == 0){ 
    message.concat("{");
  }else{
    message.concat(",");
  }
  json = "\"satelites\":";
  json.concat(gps.satellites.value());
  message.concat(json);

  if (gps.location.isValid())
  {
    if(message.length() == 0) {
      message.concat("{");
    }else{
      message.concat(",");
    }

    json = "\"location\": {\"lat\":";
    json.concat(String(gps.location.lat(),PRECISION));
    json.concat(",\"lng\":");
    json.concat(String(gps.location.lng(), PRECISION));
    json.concat(",\"alt\":");
    json.concat(String(gps.altitude.meters(), PRECISION));
    json.concat("}");

    message.concat(json); 
  }else{
    if(message.length() == 0){ 
      message.concat("{");
    }else{
      message.concat(",");
    }
    json = "\"location\": {\"lat\":";
    json.concat(String(38.704510, PRECISION));
    json.concat(",\"lng\":");
    json.concat(String(-9.202282, PRECISION));
    json.concat(",\"alt\":");
    json.concat(String( 78.20, PRECISION));
    json.concat("}");

    message.concat(json); 
  }
  

  if (gps.date.isValid())
  {
    if(message.length() == 0) {
      message.concat("{");
    }else{
      message.concat(",");
    }
    json = "\"date\": {\"day\":";
    json.concat(gps.date.day());
    json.concat(",\"month\":");
    json.concat(gps.date.month());
    json.concat(",\"year\":");
    json.concat(gps.date.year());
    json.concat("}");


    message.concat(json);

  }

  if (gps.time.isValid())
  {
    if(message.length() == 0) {
      message.concat("{");
    }else{
      message.concat(",");
    }

    json = "\"time\": {";

    json.concat("\"hour\":");
    json.concat(gps.time.hour());
    json.concat(",\"minute\":");
    json.concat(gps.time.minute());
    json.concat(",\"second\":");
    json.concat(gps.time.second());      
    json.concat(",\"centisecond\":");
    json.concat(gps.time.centisecond());
    json.concat("}");
  }
  message.concat(json);
  
  
  if(message.length() > 0)
  {
    message.concat("}");
  }
  return message;
}


void gps_display_info()
{
  Serial.print("N Satelites: ");
  Serial.println(gps.satellites.value());
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Date Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Time Not Available");
  }
}