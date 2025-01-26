#include "DS1302.hpp"
#include <Arduino.h>

DS1302_RTC::DS1302_RTC(int ioPin, int sclkPin, int cePin) : myWire(ioPin, sclkPin, cePin), Rtc(myWire) {}

void DS1302_RTC::begin() {
    Rtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);

    if (!Rtc.IsDateTimeValid()) {
        //Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected()) {
        //Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning()) {
        //Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) {
        //Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) {
        //Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) {
        //Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
}

void DS1302_RTC::printDateTime(const RtcDateTime& dt) {
    char datestring[26];
    snprintf_P(datestring, 
            sizeof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second());
    Serial.println(datestring);
}
String DS1302_RTC::getDateTimeMessage() {
    String datestring;

    if (!Rtc.IsDateTimeValid()) {
        //Serial.println("RTC lost confidence in the DateTime!");
        RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
        Rtc.SetDateTime(compiled);
    }

    datestring.reserve(26);  // Reserve space for the string

    datestring = String(Rtc.GetDateTime().Month()) + "/";
    datestring += String(Rtc.GetDateTime().Day()) + "/";
    datestring += String(Rtc.GetDateTime().Year()) + " ";
    datestring += String(Rtc.GetDateTime().Hour()) + ":";
    datestring += String(Rtc.GetDateTime().Minute()) + ":";
    datestring += String(Rtc.GetDateTime().Second());

    return datestring;
}

bool DS1302_RTC::setDateTime(const RtcDateTime& compiled) {
    Rtc.SetDateTime(compiled);
    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        //Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
        return false;
    }
    return true;
}

RtcDateTime DS1302_RTC::getDateTime() {
    return Rtc.GetDateTime();
}

void DS1302_RTC::updateDateTime(const RtcDateTime& compiled) {
    Rtc.SetDateTime(compiled);
}

char DS1302_RTC::getMonth() {
    return Rtc.GetDateTime().Month();
}

char DS1302_RTC::getDay() {
    return Rtc.GetDateTime().Day();
}

char DS1302_RTC::getYear() {
    return Rtc.GetDateTime().Year();
}

char DS1302_RTC::getHour() {
    return Rtc.GetDateTime().Hour();
}

char DS1302_RTC::getMinute() {
    return Rtc.GetDateTime().Minute();
}

char DS1302_RTC::getSecond() {
    return Rtc.GetDateTime().Second();
}


