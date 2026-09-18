# Working Principle

1. The operator moves the Flysky FS-i6X controls.
2. The transmitter sends commands over 2.4 GHz AFHDS 2A.
3. The FS-iA10B receiver receives the signal.
4. PWM/IBUS information is passed to the ESP32.
5. The ESP32 interprets the control input.
6. L298N drivers operate the four DC motors.
7. The servo receives PWM for robotic-arm positioning.
8. The LCD displays operational status.

The transmitter's neutral position is treated as stop. A software
dead-band is used to reduce creep caused by small neutral deviations.
