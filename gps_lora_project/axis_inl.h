
#include <Arduino.h>
#include <Wire.h>
// libraries
#include "bmm150.h"
#include "bmm150_defs.h"

BMM150 bmm = BMM150();

bmm150_mag_data value_offset;


void axis_setup(){

    Serial.begin(9600);
    if (bmm.initialize() == BMM150_E_ID_NOT_CONFORM) {
        Serial.println("Chip ID can not read! Exiting!!");
        while (1);
    } else {
        value_offset.x = -28;
        value_offset.y = -3;
        value_offset.z = -25;
        Serial.println("Initialize done!");
    }

}



void axis_loop() {
    bmm150_mag_data value;
    bmm.read_mag_data();

    value.x = bmm.raw_mag_data.raw_datax;
    value.y = bmm.raw_mag_data.raw_datay;
    value.z = bmm.raw_mag_data.raw_dataz;

    float xyHeading = atan2(value.x, value.y);
    float zxHeading = atan2(value.z, value.x);
    float heading = xyHeading;

    if (heading < 0) {
        heading += 2 * PI;
    }
    if (heading > 2 * PI) {
        heading -= 2 * PI;
    }
    float headingDegrees = heading * 180 / M_PI;
    float xyHeadingDegrees = xyHeading * 180 / M_PI;
    float zxHeadingDegrees = zxHeading * 180 / M_PI;

    Serial.print("Heading:");
    Serial.println(headingDegrees);
    Serial.print(",");
    Serial.print("xyHeading:");
    Serial.println(xyHeadingDegrees);
    Serial.print(",");
    Serial.print("zxHeading:");
    Serial.println(zxHeadingDegrees);

    delay(100);
}