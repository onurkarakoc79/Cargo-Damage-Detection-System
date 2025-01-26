#ifndef DS1302_RTC_H
#define DS1302_RTC_H

#include <RtcDS1302.h>

class DS1302_RTC {
public:
    DS1302_RTC(int ioPin, int sclkPin, int cePin);
    void begin();
    void printDateTime(const RtcDateTime& dt);
    RtcDateTime getDateTime();
    bool setDateTime(const RtcDateTime& compiled);
    void updateDateTime(const RtcDateTime& compiled);
    String getDateTimeMessage();
    char getMonth();
    char getDay();
    char getYear();
    char getHour();
    char getMinute();
    char getSecond();
private:
    ThreeWire myWire;
    RtcDS1302<ThreeWire> Rtc;
    char _month, _day, _year, _hour, _minute, _second;
};

#endif // DS1302_RTC_H
