#include "BME280.hpp"

BME280::BME280() : _bme(BME_CS_PIN){};

void BME280::begin()
{
    unsigned status;
    status = _bme.begin();

    if (!status)
    {
        //Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        //Serial.print("SensorID was: 0x");
        //Serial.println(_bme.sensorID(), 16);
        //Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        //Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        //Serial.print("        ID of 0x60 represents a BME 280.\n");
        //Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1)
            delay(10);
    }
    else
    {
        //Serial.println("BME Started successfully");
    }
}

void BME280::readAllData()
{
    _temperture = _bme.readTemperature();
    _pressure = _bme.readPressure() * PA_TO_hPA;
    _altitude = _bme.readAltitude(SEA_LEVEL_PRESSURE_hPA);
    _humidity = _bme.readHumidity();
}

void BME280::readTemperature()
{
    _temperture = _bme.readTemperature();
}

void BME280::readPressure()
{
    _pressure = _bme.readPressure() * PA_TO_hPA;
}

void BME280::readAltitude()
{
    _altitude = _bme.readAltitude(SEA_LEVEL_PRESSURE_hPA);
}

void BME280::readHumidity()
{
    _humidity = _bme.readHumidity();
}

void BME280::printAllData()
{
    Serial.print("Temperature = ");
    Serial.print(_temperture);
    Serial.println(" °C");

    Serial.print("Pressure = ");
    Serial.print(_pressure);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(_altitude);
    Serial.print(" m  ");

    Serial.print("Altitude Kalman= ");
    Serial.print(_altitudeKalman);
    Serial.println("m ");

    Serial.print("Humidity = ");
    Serial.print(_humidity);
    Serial.println(" %");

    Serial.println();
}

float BME280::getAltitude()
{
    return _altitude;
}

float BME280::getPressure()
{
    return _pressure;
}

float BME280::getHumidity()
{
    return _humidity;
}
float BME280::getTemperature()
{
    return _temperture;
}