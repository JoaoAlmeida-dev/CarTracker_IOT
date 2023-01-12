
#include <Arduino.h>
#include <Wire.h>
// libraries
#include "bmm150.h"
#include "bmm150_defs.h"

BMM150 bmm = BMM150();

bmm150_mag_data value_offset;

int previous_headingDegrees = 0;
int previous_xyHeadingDegrees = 0;
int previous_zxHeadingDegrees = 0;


void axis_setup() {

  Serial.begin(9600);
  debugln("Setting up 3axis");
  if (bmm.initialize() == BMM150_E_ID_NOT_CONFORM) {
    debugln("Chip ID can not read! Exiting!!");
    while (1)
      ;
  } else {
    value_offset.x = -28;
    value_offset.y = -3;
    value_offset.z = -25;
    debugln("Setup 3axis done!");
  }
}



bool axis_loop(int movement_variation_limit ) {
  bmm150_mag_data value;
  bmm.read_mag_data();

  value.x = bmm.raw_mag_data.raw_datax;
  value.y = bmm.raw_mag_data.raw_datay;
  value.z = bmm.raw_mag_data.raw_dataz;

  float xyHeading = atan2(value.x, value.y);
  float zxHeading = atan2(value.z, value.x);
  float heading = xyHeading;
  bool should_alarm = false;

  if (heading < 0) {
    heading += 2 * PI;
  }
  if (heading > 2 * PI) {
    heading -= 2 * PI;
  }
  float headingDegrees = heading * 180 / M_PI;
  float xyHeadingDegrees = xyHeading * 180 / M_PI;
  float zxHeadingDegrees = zxHeading * 180 / M_PI;

  // debug("x:");
  // debugln(value.x);
  // debug(",");
  // debug("y:");
  // debugln(value.y);
  // debug(",");
  // debug("z:");
  // debugln(value.z);

  debug("Heading:");
  debugln(headingDegrees);
  debug(",");
  debug("xyHeading:");
  debugln(xyHeadingDegrees);
  debug(",");
  debug("zxHeading:");
  debugln(zxHeadingDegrees);

  if(previous_headingDegrees == 0 ||
    previous_xyHeadingDegrees == 0 ||
    previous_zxHeadingDegrees == 0 
  ){
    previous_headingDegrees = headingDegrees;
    previous_xyHeadingDegrees = xyHeadingDegrees;
    previous_zxHeadingDegrees = zxHeadingDegrees;
  }
  
  if (
    abs(abs(previous_headingDegrees) - abs(headingDegrees)) > movement_variation_limit ||
    abs(abs(previous_xyHeadingDegrees) - abs(xyHeadingDegrees)) > movement_variation_limit||
    abs(abs(previous_zxHeadingDegrees) - abs(zxHeadingDegrees)) > movement_variation_limit 
    ) {
    should_alarm = true;
  }

  previous_headingDegrees = headingDegrees;
  previous_xyHeadingDegrees = xyHeadingDegrees;
  previous_zxHeadingDegrees = zxHeadingDegrees;

  return should_alarm;
}