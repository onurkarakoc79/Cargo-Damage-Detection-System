#ifndef ESP32_HPP
#define ESP32_HPP

#include <Arduino.h>
#include <esp_timer.h>
#include <esp_sleep.h>

class Esp32S3
{
private:
    unsigned long _initialTime;
    constexpr static uint8_t KEY_PIN = 0;
    int _timeFlags[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    esp_timer_handle_t timer;
    typedef void (*TimerCallbackFunction)(void* arg);

public:
    Esp32S3(int minute=-1,int hour=-1);
    String getTime();
    void begin();
    void setTimerInterrupt(int seconds, TimerCallbackFunction callback);
    void sleepSet(int timerSeconds,gpio_num_t interruptPin);
    bool SmartDelay(unsigned long ms, int flag);
    
};

#endif // ESP32_HPP
