// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------

void normalize(gyroscope_raw gyroscope)
{
  // Sensitivity Scale Factor (MPU datasheet page 8)
  normalized.gyroscope.x = gyroscope.x / 32.8;
  normalized.gyroscope.y = gyroscope.y / 32.8;
  normalized.gyroscope.z = gyroscope.z / 32.8;
}

void normalize(accelerometer_raw accelerometer)
{
  // Sensitivity Scale Factor (MPU datasheet page 9)
  normalized.accelerometer.x = accelerometer.x * G / 16384;
  normalized.accelerometer.y = accelerometer.y * G / 16384;
  normalized.accelerometer.z = accelerometer.z * G / 16384;
}

void normalize(temperature_raw temperature)
{
  // Sensitivity Scale Factor (MPU datasheet page 11) & formula (MPU registers page 33)
  normalized.temperature = ((temperature.value - TEMPERATURE_OFFSET) / 333.87) + TEMPERATURE_OFFSET;
}
