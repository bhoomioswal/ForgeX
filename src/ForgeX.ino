#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

/*
  Forge-X
  Reference ESP32 control implementation.

  IMPORTANT:
  The project report does not contain the original source code or exact
  GPIO assignments. Verify every pin and receiver channel before use.
*/

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo armServo;

/* Reference motor pins */
const int L1_IN1 = 25, L1_IN2 = 26, L1_EN = 27;
const int L2_IN1 = 14, L2_IN2 = 12, L2_EN = 13;
const int R1_IN1 = 32, R1_IN2 = 33, R1_EN = 4;
const int R2_IN1 = 18, R2_IN2 = 19, R2_EN = 5;

/* Reference servo and receiver pins */
const int SERVO_PIN = 23;
const int CH_THROTTLE = 34;
const int CH_STEERING = 35;
const int CH_ARM = 36;

const int DEAD_BAND = 30;

void motorForward(int a, int b, int en, int speed) {
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  analogWrite(en, constrain(speed, 0, 255));
}

void motorBackward(int a, int b, int en, int speed) {
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  analogWrite(en, constrain(speed, 0, 255));
}

void stopMotors() {
  int pins[] = {L1_IN1,L1_IN2,L2_IN1,L2_IN2,R1_IN1,R1_IN2,R2_IN1,R2_IN2};
  for (int p : pins) digitalWrite(p, LOW);
  analogWrite(L1_EN, 0); analogWrite(L2_EN, 0);
  analogWrite(R1_EN, 0); analogWrite(R2_EN, 0);
}

void moveForward(int s) {
  motorForward(L1_IN1,L1_IN2,L1_EN,s);
  motorForward(L2_IN1,L2_IN2,L2_EN,s);
  motorForward(R1_IN1,R1_IN2,R1_EN,s);
  motorForward(R2_IN1,R2_IN2,R2_EN,s);
}

void moveBackward(int s) {
  motorBackward(L1_IN1,L1_IN2,L1_EN,s);
  motorBackward(L2_IN1,L2_IN2,L2_EN,s);
  motorBackward(R1_IN1,R1_IN2,R1_EN,s);
  motorBackward(R2_IN1,R2_IN2,R2_EN,s);
}

void spinLeft(int s) {
  motorBackward(L1_IN1,L1_IN2,L1_EN,s);
  motorBackward(L2_IN1,L2_IN2,L2_EN,s);
  motorForward(R1_IN1,R1_IN2,R1_EN,s);
  motorForward(R2_IN1,R2_IN2,R2_EN,s);
}

void spinRight(int s) {
  motorForward(L1_IN1,L1_IN2,L1_EN,s);
  motorForward(L2_IN1,L2_IN2,L2_EN,s);
  motorBackward(R1_IN1,R1_IN2,R1_EN,s);
  motorBackward(R2_IN1,R2_IN2,R2_EN,s);
}

void updateLCD(const char* action, bool motorOn) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ACT:");
  lcd.print(action);
  lcd.setCursor(0,1);
  lcd.print(motorOn ? "MOTOR: ON" : "MOTOR: OFF");
}

void setup() {
  Serial.begin(115200);

  int outputPins[] = {
    L1_IN1,L1_IN2,L1_EN,L2_IN1,L2_IN2,L2_EN,
    R1_IN1,R1_IN2,R1_EN,R2_IN1,R2_IN2,R2_EN
  };
  for (int p : outputPins) pinMode(p, OUTPUT);

  pinMode(CH_THROTTLE, INPUT);
  pinMode(CH_STEERING, INPUT);
  pinMode(CH_ARM, INPUT);

  armServo.attach(SERVO_PIN);
  armServo.write(90);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("FORGE-X");
  lcd.setCursor(0,1);
  lcd.print("System Ready");

  stopMotors();
  delay(1500);
}

void loop() {
  unsigned long throttle = pulseIn(CH_THROTTLE, HIGH, 25000);
  unsigned long steering = pulseIn(CH_STEERING, HIGH, 25000);
  unsigned long armInput = pulseIn(CH_ARM, HIGH, 25000);

  if (throttle == 0) {
    stopMotors();
    updateLCD("SIGNAL LOST", false);
    delay(20);
    return;
  }

  int t = map(throttle, 1000, 2000, -255, 255);
  int s = map(steering, 1000, 2000, -255, 255);
  t = constrain(t, -255, 255);
  s = constrain(s, -255, 255);

  if (armInput > 0) {
    int angle = map(armInput, 1000, 2000, 0, 180);
    armServo.write(constrain(angle, 0, 180));
  }

  if (abs(t) < DEAD_BAND && abs(s) < DEAD_BAND) {
    stopMotors();
    updateLCD("STOP", false);
  }
  else if (abs(t) >= DEAD_BAND && abs(s) < DEAD_BAND) {
    if (t > 0) {
      moveForward(abs(t));
      updateLCD("FORWARD", true);
    } else {
      moveBackward(abs(t));
      updateLCD("BACKWARD", true);
    }
  }
  else if (abs(t) < DEAD_BAND && abs(s) >= DEAD_BAND) {
    if (s > 0) {
      spinRight(abs(s));
      updateLCD("SPIN RIGHT", true);
    } else {
      spinLeft(abs(s));
      updateLCD("SPIN LEFT", true);
    }
  }
  else {
    int left = constrain(t + s, -255, 255);
    int right = constrain(t - s, -255, 255);

    if (left >= 0) {
      motorForward(L1_IN1,L1_IN2,L1_EN,left);
      motorForward(L2_IN1,L2_IN2,L2_EN,left);
    } else {
      motorBackward(L1_IN1,L1_IN2,L1_EN,abs(left));
      motorBackward(L2_IN1,L2_IN2,L2_EN,abs(left));
    }

    if (right >= 0) {
      motorForward(R1_IN1,R1_IN2,R1_EN,right);
      motorForward(R2_IN1,R2_IN2,R2_EN,right);
    } else {
      motorBackward(R1_IN1,R1_IN2,R1_EN,abs(right));
      motorBackward(R2_IN1,R2_IN2,R2_EN,abs(right));
    }

    updateLCD("DIFF DRIVE", true);
  }

  delay(20);
}
