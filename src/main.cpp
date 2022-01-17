// Plotclock
// cc - by Johannes Heberlein 2014

#include <Arduino.h>
#include <Servo.h>
#include <TimeLib.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <Wire.h>

#include "draw.h"

ThreeWire myWire(A4, A5, A3);  // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
int last_min = 0;

// #define CALIBRATION      // enable calibration mode

void printDateTime(const RtcDateTime& dt) {
  char datestring[20];

  snprintf_P(datestring, 20,
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"), dt.Month(), dt.Day(),
             dt.Year(), dt.Hour(), dt.Minute(), dt.Second());
  Serial.println(datestring);
}

void initRTC() {
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Serial.print("compiller time: ");
  printDateTime(compiled);
  if (!Rtc.IsDateTimeValid()) {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  Serial.print("Time from RTC: ");
  printDateTime(now);
  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    Serial.println("RTC is newer than compile time. (this is expected)");
  } else if (now == compiled) {
    Serial.println(
        "RTC is the same as compile time! (not expected but all is fine)");
  }
}

void setup() {
  Serial.begin(115200);

  initServo();
  initRTC();

  RtcDateTime now = Rtc.GetDateTime();
  setTime(now.Hour(), now.Minute(), now.Second(), now.Day(), now.Month(), now.Year());

  drawTo(71.0, 47.2);
  lift(0);
  delay(1000);
}

void loop() {
#ifdef CALIBRATION
  lift(0);
  drawTo(71.0, 47.2);

#else

  int i = 0;
  if (last_min != minute()) {
    lift(0);

    hour();
    while ((i + 1) * 10 <= hour()) {
      i++;
    }

    number(3, 3, 111, 1);
    number(5, 25, i, 0.9);
    number(19, 25, (hour() - i * 10), 0.9);
    number(28, 25, 11, 0.9);

    i = 0;
    while ((i + 1) * 10 <= minute()) {
      i++;
    }
    number(34, 25, i, 0.9);
    number(48, 25, (minute() - i * 10), 0.9);
    lift(2);
    drawTo(71.0, 47.2);
    lift(1);
    last_min = minute();
    delay(580);
  }

#endif
}
