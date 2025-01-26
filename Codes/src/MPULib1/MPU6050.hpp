#ifndef MPU6050_HPP
#define MPU6050_HPP

#include <Arduino.h>
#include <Wire.h>
#include "ESP32_Pins.hpp"
#include "MPU6050REG.hpp"

class MPU6050
{
private:
    int _address;
    float _roll;
    float _pitch;
    float _yaw;
    float _calibrationRoll;
    float _calibrationPitch;
    float _calibrationYaw;
    float _accX,_accY,_accZ;
    float _angleRoll,_anglePitch;
    constexpr static const int calibrationSampleNumber=2000;
    constexpr static const int _filter=0x05;
    constexpr static const int _sensitivityFactor=1;
    float _LSBGyro;
    float _LSBAccel;
    float _accelRange;
    float _accZInertial;
    float _velocityVertical;

    constexpr static const float _gForceThreshold=1.2;
    

public:
    float getRoll();
    float getPitch();
    float getYaw();

    MPU6050(int address = MPU6050_ADDRESS);
    void begin(bool sleepmode=false,int sda=SDA_PIN, int scl=SCL_PIN, int freq=400E3,bool calibration=false);
    
    void readGyro(int filer,int sensitivityFactor);
    void printNonCalibratedGyro();
    void printCalibratedGyro();
    void CalibrateGyro(int filter,int sensitivityFactor);
    
    void readAccel(int filter,int range, bool measureAngle=false);
    void printAccel();
    void setAccelSensitivity(int sensitivityFactor);
    void setAceelRange(int range);

    float calculateVelocityVertical();

    float getAccX();
    float getAccY();
    float getAccZ();
    float getRateRoll();
    float getRatePitch();
    float getRateYaw();
    float getAngleRoll();
    float getAnglePitch();
    
    bool enableFilter(int rate=0x05);
    bool setSensitivityFactor(int rate=0x8);
    bool enableSleep(bool enable);

    void setMotionDetection(float threshold,uint8_t duration);

};

#endif // MPU6050_HPP
