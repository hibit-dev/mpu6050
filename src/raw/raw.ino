// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------

#include "Wire.h"
#include "I2C.h"

#define MPU6050_IMU_ADDRESS 0x68

#define GYRO_FULL_SCALE_250_DPS  0x00
#define GYRO_FULL_SCALE_500_DPS  0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define ACC_FULL_SCALE_2G  0x00
#define ACC_FULL_SCALE_4G  0x08
#define ACC_FULL_SCALE_8G  0x10
#define ACC_FULL_SCALE_16G 0x18

#define TEMPERATURE_OFFSET 21 // As defined in documentation

#define INTERVAL_MS_PRINT 1000

#define G 9.80665

struct gyroscope_raw {
  int16_t x, y, z;
} gyroscope;

struct accelerometer_raw {
  int16_t x, y, z;
} accelerometer;

struct temperature_raw {
  int16_t value;
} temperature;

struct {
  struct {
    float x, y, z;
  } accelerometer, gyroscope;

  float temperature;
} normalized;

unsigned long lastPrintMillis = 0;

void setup()
{
  Wire.begin();
  Serial.begin(115200);

  I2CwriteByte(MPU6050_IMU_ADDRESS, 27, GYRO_FULL_SCALE_1000_DPS); // Configure gyroscope range
  I2CwriteByte(MPU6050_IMU_ADDRESS, 28, ACC_FULL_SCALE_2G); // Configure accelerometer range
  I2CwriteByte(MPU6050_IMU_ADDRESS, 56, 0x01); // Enable interrupt pin for raw data
}

void loop()
{
  unsigned long currentMillis = millis();

  if (isImuReady()) {
    readRawImu();

    normalize(gyroscope);
    normalize(accelerometer);
    normalize(temperature);
  }

  if (currentMillis - lastPrintMillis > INTERVAL_MS_PRINT) {
    Serial.print("TEMP:\t");
    Serial.print(normalized.temperature, 2);
    Serial.print("\xC2\xB0"); //Print degree symbol
    Serial.print("C");
    Serial.println();

    Serial.print("GYR (");
    Serial.print("\xC2\xB0"); //Print degree symbol
    Serial.print("/s):\t");
    Serial.print(normalized.gyroscope.x, 3);
    Serial.print("\t\t");
    Serial.print(normalized.gyroscope.y, 3);
    Serial.print("\t\t");
    Serial.print(normalized.gyroscope.z, 3);
    Serial.println();

    Serial.print("ACC (m/s^2):\t");
    Serial.print(normalized.accelerometer.x, 3);
    Serial.print("\t\t");
    Serial.print(normalized.accelerometer.y, 3);
    Serial.print("\t\t");
    Serial.print(normalized.accelerometer.z, 3);
    Serial.println();

    Serial.println();

    lastPrintMillis = currentMillis;
  }
}
