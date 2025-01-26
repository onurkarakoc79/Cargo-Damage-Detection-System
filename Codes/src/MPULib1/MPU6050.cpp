#include "MPU6050.hpp"

MPU6050::MPU6050(int address)
{
    _address = address;
}

void MPU6050::begin(bool sleepmode,int sda, int scl, int freq, bool calibration)
{
    //pinMode(MPU_INT_PIN, INPUT_PULLUP);
    Wire.begin(sda, scl);
    delay(250);
    enableSleep(sleepmode);
    if (calibration)
        CalibrateGyro(_filter, _sensitivityFactor);
}

void MPU6050::readGyro(int filter, int sensitivityFactor)
{
    enableFilter(filter);
    setSensitivityFactor(sensitivityFactor);
    Wire.beginTransmission(_address);
    Wire.write(MPU6050_GYROREG);
    Wire.endTransmission();
    Wire.requestFrom(_address, GYRO_MESSAGE_BYTES);
    int16_t gyroX = Wire.read() << 8 | Wire.read();
    int16_t gyroY = Wire.read() << 8 | Wire.read();
    int16_t gyroZ = Wire.read() << 8 | Wire.read();
    _roll = (float)gyroX / (float)_LSBGyro;
    _pitch = (float)gyroY / (float)_LSBGyro;
    _yaw = (float)gyroZ / (float)_LSBGyro;
}

bool MPU6050::enableFilter(int rate)
{
    if (rate < 0x00 || rate > 0x06)
    {
        Serial.println("Error: Filter rate must be between 0x00 and 0x07");
        return false;
    }
    Wire.beginTransmission(_address);
    Wire.write(MPU6050_FILTERREG);
    Wire.write(rate);
    Wire.endTransmission();
    return true;
}

bool MPU6050::setSensitivityFactor(int rate)
{
    switch (rate)
    {
    case 0:
        _LSBGyro = 131;
        break;
    case 1:
        _LSBGyro = 65.5;
        break;
    case 2:
        _LSBGyro = 32.8;
        break;
    case 3:
        _LSBGyro = 16.4;
        break;
    default:
        Serial.println("LSB value is not correct, sensitivy is not setted, Please define rate 0:3");
        return false;
    }
    Wire.beginTransmission(_address);
    Wire.write(MPU6050_SENSITIVITYREG); // sensitiviy factor register
    rate = pow(2, rate + 2);            // Equivalent to a = pow(2, rate + 2);
    Wire.write(rate);
    Wire.endTransmission();
    return true;
}

bool MPU6050::enableSleep(bool enable)
{
    if (enable)
    {
        Wire.beginTransmission(_address);
        Wire.write(MPU6050_SLEEPREG);
        Wire.write(0x40);
        Wire.endTransmission();
        return true;
    }
    else
    {
        Wire.beginTransmission(_address);
        Wire.write(MPU6050_SLEEPREG);
        Wire.write(0x00);
        Wire.endTransmission();
        return false;
    }
}

void MPU6050::printNonCalibratedGyro()
{
    Serial.print("Roll: ");
    Serial.print(_roll);
    Serial.print(" Pitch: ");
    Serial.print(_pitch);
    Serial.print(" Yaw: ");
    Serial.println(_yaw);
}

void MPU6050::printCalibratedGyro()
{
    Serial.print("Calibrated Roll");
    Serial.print(_roll - _calibrationRoll);
    Serial.print(" Pitch: ");
    Serial.print(_pitch - _calibrationPitch);
    Serial.print(" Yaw: ");
    Serial.println(_yaw - _calibrationYaw);
}

void MPU6050::CalibrateGyro(int filter, int sensitivityFactor)
{
    Serial.println("Calibraiton process..");
    for (int rateCalibratonNumber = 0; rateCalibratonNumber < calibrationSampleNumber; rateCalibratonNumber++)
    {
        readGyro(filter, sensitivityFactor);
        _calibrationPitch += _pitch;
        _calibrationRoll += _roll;
        _calibrationYaw += _yaw;
        delay(1);
    }
    _calibrationPitch /= calibrationSampleNumber;
    _calibrationYaw /= calibrationSampleNumber;
    _calibrationRoll /= calibrationSampleNumber;
}

void MPU6050::setAccelSensitivity(int sensitivityFactor)
{
    ;
}

void MPU6050::setAceelRange(int range)
{
    if (range < 0x00 || range > 0x03)
    {
        Serial.println("Error: Accel range must be between 0x00 and 0x03");
        return;
    }
    switch (range)
    {
    case 0x00:
        _LSBAccel = 16384;
        Wire.beginTransmission(_address);
        Wire.write(MPU6050_ACCEL_RANGEREG);
        Wire.write(0x00);
        Wire.endTransmission();
        break;
    case 0x01:
        _LSBAccel = 8192;
        Wire.beginTransmission(_address);
        Wire.write(MPU6050_ACCEL_RANGEREG);
        Wire.write(0x08);
        Wire.endTransmission();
        break;
    case 0x02:
        _LSBAccel = 4096;
        Wire.beginTransmission(_address);
        Wire.write(MPU6050_ACCEL_RANGEREG);
        Wire.write(0x10);
        Wire.endTransmission();
        break;
    case 0x03:
        _LSBAccel = 2048;
        Wire.beginTransmission(_address);
        Wire.write(MPU6050_ACCEL_RANGEREG);
        Wire.write(0x18);
        Wire.endTransmission();
        break;
    }
}

void MPU6050::readAccel(int filter, int range, bool measureAngle)
{
    enableFilter(filter);
    setAceelRange(range);
    Wire.beginTransmission(_address);
    Wire.write(MPU6050_ACCELREG);
    Wire.endTransmission();
    Wire.requestFrom(_address, ACCEL_MESSAGE_BYTES);
    int16_t accXLSB = Wire.read() << 8 | Wire.read();
    int16_t accYLSB = Wire.read() << 8 | Wire.read();
    int16_t accZLSB = Wire.read() << 8 | Wire.read();
    _accX = (float)accXLSB / (float)_LSBAccel; // do not forget add calibration number here in PCB card
    _accY = (float)accYLSB / (float)_LSBAccel; // do not forget add calibration number here in PCB card
    _accZ = (float)accZLSB / (float)_LSBAccel; // do not forget add calibration number here in PCB card
    if (measureAngle)
    {
        _angleRoll = atan(_accY / sqrt(pow(_accX, 2) + pow(_accZ, 2))) * 180 / PI;
        _anglePitch = atan(-1 * (_accX / sqrt(pow(_accY, 2) + pow(_accZ, 2)))) * 180 / PI;
    }
}

void MPU6050::printAccel()
{
    Serial.print("Accel X: ");
    Serial.print(_accX);
    Serial.print(" Accel Y: ");
    Serial.print(_accY);
    Serial.print(" Accel Z: ");
    Serial.println(_accZ);
}

float MPU6050::calculateVelocityVertical()
{
    _accZInertial = (-1) * _accX * sin(_anglePitch * PI / 180) +
                    _accY * sin(_angleRoll * PI / 180) * cos(_anglePitch * PI / 180) +
                    _accZ * cos(_anglePitch * PI / 180) * cos(_angleRoll * PI / 180);

    _accZInertial = (_accZInertial - 1) * 9.81;
    _velocityVertical = _velocityVertical + _accZInertial * 0.004;
    return _velocityVertical;
}

void MPU6050::setMotionDetection(float threshold, uint8_t duration)
{
    uint8_t thresholdBy= constrain(map(threshold,0,16,0,255),0,255);

    Wire.beginTransmission(_address);
    Wire.write(MPU6050_SLEEPREG);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(MPU6050_INT_PIN_CFG);
    Wire.write(0x10);
    Wire.endTransmission();
    
    Wire.beginTransmission(_address);
    Wire.write(MPU6050_MOTION_THR);
    Wire.write((uint8_t)1);
    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(MPU6050_MOTION_DUR);
    Wire.write(duration);
    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(MPU6050_INT_ENABlE);
    Wire.write(0x40);
    Wire.endTransmission();

};

float MPU6050::getAccX()
{
    return _accX;
}

float MPU6050::getAccY()
{
    return _accY;
}

float MPU6050::getAccZ()
{
    return _accZ;
}

float MPU6050::getRateRoll()
{
    return _roll - _calibrationRoll;
}

float MPU6050::getRatePitch()
{
    return _pitch - _calibrationPitch;
}

float MPU6050::getRateYaw()
{
    return _yaw - _calibrationYaw;
}

float MPU6050::getRoll()
{
    return _roll - _calibrationRoll;
}

float MPU6050::getYaw()
{
    return _yaw - _calibrationYaw;
}

float MPU6050::getPitch()
{
    return _pitch - _calibrationPitch;
}

float MPU6050::getAngleRoll()
{
    return _angleRoll;
}

float MPU6050::getAnglePitch()
{
    return _anglePitch;
}