# System Architecture

Forge-X integrates three major subsystems.

## 1. Locomotion
A 4WD skid-steer base provides forward, backward, left, right and
zero-radius turning.

## 2. Manipulation
A servo-controlled single robotic arm provides basic lightweight-object
handling.

## 3. Control and Communication
The Flysky FS-i6X transmitter sends commands through a 2.4 GHz AFHDS 2A
link to the FS-iA10B receiver. The receiver passes PWM/IBUS information
to the ESP32, which controls motors, servo and LCD feedback.

```text
FS-i6X
  │
  ▼
2.4 GHz RF
  │
  ▼
FS-iA10B
  │
  ▼
ESP32
 ├──> L298N #1 ──> Left motors
 ├──> L298N #2 ──> Right motors
 ├──> Servo ─────> Arm
 └──> I2C LCD ──> Status
```
