# Testing and Results

## Documented Results

| Parameter | Value |
|---|---|
| Maximum PWM | 255 |
| Battery life | 30–45 minutes |
| Turning radius | Zero / in-place |
| Arm reach | TBD |

## Tests

### Forward
Both left and right wheel groups rotate forward.

### Backward
Both left and right wheel groups rotate backward.

### Left Spin
Left wheels reverse while right wheels move forward.

### Right Spin
Left wheels move forward while right wheels reverse.

### Arm
The servo is controlled through a dedicated transmitter channel.

## Observed Challenges
- Voltage drop under combined motor and servo load
- ESP32 resets under heavy load
- L298N heating and voltage drop
- Wheel scrub during skid-steer turns
- Stick neutral drift
- Chassis flex/misalignment
- Servo jitter during simultaneous motor operation
