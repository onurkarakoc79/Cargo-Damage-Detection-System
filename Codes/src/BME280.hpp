#ifndef BME280_HPP
#define BME280_HPP



#include <Arduino.h>
#include <Adafruit_BME280.h>
#include "ESP32_Pins.hpp"




class BME280
{
private:
    /* data */
    bool _bmeEnabled=true;
    constexpr static float SEA_LEVEL_PRESSURE_hPA=1013.25;
    constexpr static float PA_TO_hPA=1.0F/100.0F;
    Adafruit_BME280 _bme;
    float _humidity;
    float _altitude;
    float _temperture;
    float _pressure;
    float _altitudeKalman;


public:
    BME280(/* args */);
    void readAllData();
    void readTemperature();
    void readPressure();
    void readAltitude();
    void readHumidity();
    void printAllData();
    void begin();
    float getTemperature();
    float getHumidity();
    float getPressure();
    float getAltitude();

};






#endif // BME280_HPP