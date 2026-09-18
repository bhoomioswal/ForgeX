# Wiring Overview

## Power

```text
5200 mAh LiPo
   ├──> L298N motor drivers
   └──> regulated logic supply
          ├──> ESP32
          ├──> LCD
          └──> Servo
```

Use a suitable regulator and verify the battery cell count before
connecting the ESP32 or servo.

## Motors

Two L298N dual H-bridge boards provide four motor channels, one for
each wheel.

## Receiver

```text
FS-i6X
  ↓
FS-iA10B
  ↓
PWM / IBUS
  ↓
ESP32
```

## LCD

The LCD uses the I2C interface. Verify the actual SDA/SCL pins and LCD
I2C address before deployment.
