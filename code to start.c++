#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// ===== CHANNEL MAP =====
// BACK
#define LB_SHOULDER 0
#define LB_UPPER    1
#define LB_KNEE     2

#define RB_SHOULDER 3
#define RB_UPPER    4
#define RB_KNEE     5

// FRONT
#define LF_SHOULDER 6
#define LF_UPPER    7
#define LF_KNEE     8

#define RF_SHOULDER 9
#define RF_UPPER    10
#define RF_KNEE     11

// ===== SERVO RANGE =====
#define SERVOMIN 110
#define SERVOMAX 510

// ===== ANGLES =====
// BACK
int LB_s = 50, LB_u = 56, LB_k = 148;
int RB_s = 134, RB_u = 114, RB_k = 40;

// FRONT
int LF_s = 90, LF_u = 64, LF_k = 154;
int RF_s = 90, RF_u = 132, RF_k = 74;

char selected = 'q';

// ===== CONVERT =====
int angleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

// ===== UPDATE =====
void updateAll(){

  // BACK
  pwm.setPWM(LB_SHOULDER, 0, angleToPulse(LB_s));
  pwm.setPWM(LB_UPPER,    0, angleToPulse(LB_u));
  pwm.setPWM(LB_KNEE,     0, angleToPulse(LB_k));

  pwm.setPWM(RB_SHOULDER, 0, angleToPulse(RB_s));
  pwm.setPWM(RB_UPPER,    0, angleToPulse(RB_u));
  pwm.setPWM(RB_KNEE,     0, angleToPulse(RB_k));

  // FRONT
  pwm.setPWM(LF_SHOULDER, 0, angleToPulse(LF_s));
  pwm.setPWM(LF_UPPER,    0, angleToPulse(LF_u));
  pwm.setPWM(LF_KNEE,     0, angleToPulse(LF_k));

  pwm.setPWM(RF_SHOULDER, 0, angleToPulse(RF_s));
  pwm.setPWM(RF_UPPER,    0, angleToPulse(RF_u));
  pwm.setPWM(RF_KNEE,     0, angleToPulse(RF_k));
}

// ===== PRINT POSITIONS =====
void printPositions() {

  Serial.print("LB: ");
  Serial.print(LB_s); Serial.print(",");
  Serial.print(LB_u); Serial.print(",");
  Serial.print(LB_k);

  Serial.print(" | RB: ");
  Serial.print(RB_s); Serial.print(",");
  Serial.print(RB_u); Serial.print(",");
  Serial.print(RB_k);

  Serial.print(" | LF: ");
  Serial.print(LF_s); Serial.print(",");
  Serial.print(LF_u); Serial.print(",");
  Serial.print(LF_k);

  Serial.print(" | RF: ");
  Serial.print(RF_s); Serial.print(",");
  Serial.print(RF_u); Serial.print(",");
  Serial.print(RF_k);

  Serial.println();
}

void setup() {
  Serial.begin(115200);

  Wire.begin(); // D2 SDA, D1 SCL

  pwm.begin();
  pwm.setPWMFreq(50);

  delay(10);
  updateAll();
  printPositions();

  Serial.println("=== 4 LEG CONTROL (PCA9685) ===");
  Serial.println("BACK:  q w e → LB | a s d → RB");
  Serial.println("FRONT: u i o → LF | j k l → RF");
  Serial.println("+ / - to adjust");
}

void loop() {

  if (Serial.available()) {
    char cmd = Serial.read();

    // SELECT
    if (strchr("qweasduiojkl", cmd)) {
      selected = cmd;
    }

    if (cmd == '+' || cmd == '-') {

      int step = (cmd == '+') ? 2 : -2;

      // BACK
      if (selected=='q') LB_s += step;
      if (selected=='w') LB_u += step;
      if (selected=='e') LB_k += step;

      if (selected=='a') RB_s += step;
      if (selected=='s') RB_u += step;
      if (selected=='d') RB_k += step;

      // FRONT
      if (selected=='u') LF_s += step;
      if (selected=='i') LF_u += step;
      if (selected=='o') LF_k += step;

      if (selected=='j') RF_s += step;
      if (selected=='k') RF_u += step;
      if (selected=='l') RF_k += step;

      // LIMITS
      LB_s = constrain(LB_s,0,180);
      LB_u = constrain(LB_u,0,180);
      LB_k = constrain(LB_k,0,180);

      RB_s = constrain(RB_s,0,180);
      RB_u = constrain(RB_u,0,180);
      RB_k = constrain(RB_k,0,180);

      LF_s = constrain(LF_s,0,180);
      LF_u = constrain(LF_u,0,180);
      LF_k = constrain(LF_k,0,180);

      RF_s = constrain(RF_s,0,180);
      RF_u = constrain(RF_u,0,180);
      RF_k = constrain(RF_k,0,180);

      updateAll();
      printPositions();   // ⭐ THIS IS WHAT YOU WANTED
    }
  }
}
