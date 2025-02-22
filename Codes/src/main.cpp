#include <ESP32.hpp>
#include "BME280.hpp"
#include "DS1302.hpp"
#include "MPU6050.hpp"
#include "SDCard.hpp"

#define SAMPLING_TIME 60 // seconds
#define MOTION_EVENT_DURATION 40
#define MOTION_THRESHOLD 1
#define LSBAccel 2048
float accX, accY, accZ;
String createMessage();
void mpuGetAccel();
inline void mpuSetInterrupt();

Esp32S3 esp32;
BME280 bme280;
MPU6050 mpu6050;
SDCard sdCard;
DS1302_RTC rtc(RTC_DATA_PIN,RTC_CLK_PIN,RTC_RST_PIN);

void setup()
{
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  esp32.begin();
  rtc.begin();
  RtcDateTime complied=RtcDateTime(__DATE__,__TIME__);
  if(rtc.setDateTime(complied))
  {
    //Serial.println("RTC set successfully");
  }
  else
  {
    //Serial.println("RTC set failed");
  }
  sdCard.begin();
  sdCard.setFileName("/foo.txt");
  bme280.begin();
  mpu6050.initialize();
  //Serial.println("Testing device connections...");
  //Serial.println(mpu6050.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  mpuSetInterrupt();
  
  /*
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    //Serial.println("Woke up from external interrupt (EXT0)");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    //Serial.println("Woke up from timer");
    break;
  // Add more cases as needed for other wakeup causes
  default:
    //Serial.println("Woke up from unknown cause");
  }
  */
  esp32.sleepSet(3, MPU_INT_PIN);
}

void loop()
{
  
  mpuGetAccel();
  bme280.readAllData();
  //Serial.println(createMessage());
  sdCard.writeData(sdCard.FILE_NAME,createMessage());
  esp_light_sleep_start();
}

String createMessage()
{
  String message = "";
  message += "Time: ";
  message += String(rtc.getDateTimeMessage());
  message += " T(C): ";
  message += String(bme280.getTemperature());
  message += " P(hPA): ";
  message += String(bme280.getPressure());
  message += " A(m): ";
  message += String(bme280.getAltitude());
  message += " H(%): ";
  message += String(bme280.getHumidity());

  message += " AccX(g): ";
  message += String(accX-0.07);
  message += " AccY(g): ";
  message += String(accY+0.12);
  message += " AccZ(g): ";
  message += String(accZ-1.19);

  message += "\n";
  return message;
}

inline void mpuSetInterrupt()
{
  mpu6050.setAccelerometerPowerOnDelay(3);
  mpu6050.setFullScaleAccelRange(MPU6050_ACCEL_FS_16); // 0 -> 2g, 1 -> 4g, 2 -> 8g, 3 -> 16g
  mpu6050.setInterruptMode(true);                      // Interrupts enabled
  mpu6050.setInterruptLatch(0);                        // Interrupt pulses when triggered instead of remaining on until cleared
  mpu6050.setIntMotionEnabled(true);                   // Interrupts sent when motion detected

  // Set sensor filter mode.
  // 0 -> Reset (disable high pass filter)
  // 1 -> On (5Hz)
  // 2 -> On (2.5Hz)
  // 3 -> On (1.25Hz)
  // 4 -> On (0.63Hz)
  // 5 -> Hold (Future outputs are relative to last output when this mode was set)
  mpu6050.setDHPFMode(1);

  // Motion detection acceleration threshold. 1LSB = 2mg.
  mpu6050.setMotionDetectionThreshold(MOTION_THRESHOLD);

  // Motion detection event duration in ms
  mpu6050.setMotionDetectionDuration(MOTION_EVENT_DURATION);
}

IRAM_ATTR inline void mpuGetAccel()
{
  int16_t accXRaw, accYRaw, accZRaw;
  mpu6050.getAcceleration(&accXRaw, &accYRaw, &accZRaw);
  accX = ((float)accXRaw / (float)LSBAccel); // do not forget add calibration number here in PCB card
  accY = ((float)accYRaw / (float)LSBAccel); // do not forget add calibration number here in PCB card
  accZ = ((float)accZRaw / (float)LSBAccel); // do not forget add calibration number here in PCB card
}