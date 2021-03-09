#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

long timer = 0;

void setup() 
{
    Serial.begin(115200);
    Wire.begin(33,32);
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
}

void loop() 
{
    mpu6050.update();
    Serial.printf("%+02.02f,%+02.02f,%+02.02f,%+02.02f,%+02.02f,%+02.02f\n", mpu6050.getAccX(), mpu6050.getAccY(), mpu6050.getAccZ(),
                                            mpu6050.getGyroX(), mpu6050.getGyroY(), mpu6050.getGyroZ());
    delay(1);
}