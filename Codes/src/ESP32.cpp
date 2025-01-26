#include "ESP32.hpp"

namespace
{
#define CONVERT_TO_SECONDS 1000000
};

// Default constructor
Esp32S3::Esp32S3(int minute, int hour)
{
    // Constructor with no parameters, server is not created here
    if (minute == -1 && hour == -1)
    {
        return;
    }
    else
    {
        _initialTime = millis() - ((hour * 3600 + minute * 60 + 0) * 1000);
    }
}

// Rest of the class implementation...

void Esp32S3::begin()
{
    //Serial.begin(115200);
    pinMode(KEY_PIN, INPUT_PULLUP);
};

void Esp32S3::setTimerInterrupt(int seconds, TimerCallbackFunction callback)
{
    // Ensure that the interval is within the valid range
    uint64_t interval = (uint64_t)seconds * CONVERT_TO_SECONDS;

    esp_timer_create_args_t timerConfig = {
        .callback = callback,
        .name = "my_timer"};

    esp_timer_create(&timerConfig, &timer);
    esp_timer_start_periodic(timer, interval);
}

void Esp32S3::sleepSet(int timerSeconds, gpio_num_t interruptPin)
{
    esp_sleep_enable_timer_wakeup(timerSeconds*CONVERT_TO_SECONDS);
    esp_sleep_enable_ext0_wakeup(interruptPin, LOW);
    
}

bool Esp32S3::SmartDelay(unsigned long ms, int flag)
{

    if (millis() - _timeFlags[flag] > ms)
    {
        _timeFlags[flag] = millis();
        return true;
    }
    return false;
}

String Esp32S3::getTime()
{
    unsigned long elapsedTime = millis() - _initialTime;
    unsigned long seconds = (elapsedTime / 1000) % 60;
    unsigned long minutes = (elapsedTime / (1000 * 60)) % 60;
    unsigned long hours = (elapsedTime / (1000 * 60 * 60)) % 24;
    String time = "";
    time += (String)hours;
    time += ":";
    time += (String)minutes;
    time += ":";
    time += (String)seconds;
    return time;
}