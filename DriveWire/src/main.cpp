#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

// =====================
// Pin assignments
// =====================

// Motor B
const int BIN1 = 10;
const int BIN2 = 9;

// Motor A
const int AIN2 = 3;
const int AIN1 = 8;

// Current sensor I2C
const int SDA_PIN = 7;
const int SCL_PIN = 15;

// Battery voltage divider
const int VBAT_PIN = 4;

// Voltage divider values
// Battery + ---- R1 ---- Vo ---- R2 ---- GND
const float R1 = 10000.0;
const float R2 = 4700.0;
const float DIVIDER_SCALE = (R1 + R2) / R2;

// Motor PWM settings
const int PWM_MIN_TEST = 140;   // low-ish duty
const int PWM_MED_TEST = 160;  // medium duty
const int PWM_MAX_SAFE = 180;  // don't go cursed full send yet

Adafruit_INA219 ina219;
bool ina219_ok = false;

// =====================
// Helpers
// =====================

void motorStop()
{
  analogWrite(AIN1, 0);
  analogWrite(AIN2, 0);
  analogWrite(BIN1, 0);
  analogWrite(BIN2, 0);
}

void motorBrake()
{
  analogWrite(AIN1, 255);
  analogWrite(AIN2, 255);
  analogWrite(BIN1, 255);
  analogWrite(BIN2, 255);
}

void setMotorA(int pwm)
{
  pwm = constrain(pwm, -255, 255);

  if (pwm > 0)
  {
    analogWrite(AIN1, pwm);
    analogWrite(AIN2, 0);
  }
  else if (pwm < 0)
  {
    analogWrite(AIN1, 0);
    analogWrite(AIN2, -pwm);
  }
  else
  {
    analogWrite(AIN1, 0);
    analogWrite(AIN2, 0);
  }
}

void setMotorB(int pwm)
{
  pwm = constrain(pwm, -255, 255);

  if (pwm > 0)
  {
    analogWrite(BIN1, pwm);
    analogWrite(BIN2, 0);
  }
  else if (pwm < 0)
  {
    analogWrite(BIN1, 0);
    analogWrite(BIN2, -pwm);
  }
  else
  {
    analogWrite(BIN1, 0);
    analogWrite(BIN2, 0);
  }
}

void drive(int leftPWM, int rightPWM)
{
  // For now assuming:
  // Motor A = left motor
  // Motor B = right motor
  // If steering is backwards later, we just flip signs.
  setMotorA(leftPWM);
  setMotorB(rightPWM);
}

float readBatteryVoltage()
{
  // ESP32 Arduino has calibrated millivolt reading on many boards
  int mv = analogReadMilliVolts(VBAT_PIN);
  float v_adc = mv / 1000.0;
  float v_bat = v_adc * DIVIDER_SCALE;
  return v_bat;
}

void printTelemetry()
{
  float vbat = readBatteryVoltage();

  Serial.println();
  Serial.println("===== Telemetry =====");
  Serial.print("Battery voltage estimate: ");
  Serial.print(vbat, 2);
  Serial.println(" V");

  if (ina219_ok)
  {
    float busV = ina219.getBusVoltage_V();
    float shuntmV = ina219.getShuntVoltage_mV();
    float currentmA = ina219.getCurrent_mA();
    float powermW = ina219.getPower_mW();

    Serial.print("INA219 bus voltage: ");
    Serial.print(busV, 2);
    Serial.println(" V");

    Serial.print("INA219 shunt voltage: ");
    Serial.print(shuntmV, 2);
    Serial.println(" mV");

    Serial.print("INA219 current: ");
    Serial.print(currentmA, 1);
    Serial.println(" mA");

    Serial.print("INA219 power: ");
    Serial.print(powermW, 1);
    Serial.println(" mW");
  }
  else
  {
    Serial.println("INA219 not detected.");
  }

  Serial.println("=====================");
  Serial.println();
}

void pulseMotorA()
{
  Serial.println("Testing Motor A forward...");
  setMotorA(PWM_MIN_TEST);
  delay(700);
  motorStop();
  delay(500);

  Serial.println("Testing Motor A reverse...");
  setMotorA(-PWM_MIN_TEST);
  delay(700);
  motorStop();
}

void pulseMotorB()
{
  Serial.println("Testing Motor B forward...");
  setMotorB(PWM_MIN_TEST);
  delay(700);
  motorStop();
  delay(500);

  Serial.println("Testing Motor B reverse...");
  setMotorB(-PWM_MIN_TEST);
  delay(700);
  motorStop();
}

void testA() {
  Serial.println("Testing Motor A High");
  analogWrite(AIN1, 255);
  analogWrite(AIN2, 0);
}

void testB() {
  Serial.println("Testing Motor B High");
  analogWrite(BIN1, 255);
  analogWrite(BIN2, 0);
}

void pulseBoth()
{
  Serial.println("Testing both motors forward...");
  drive(PWM_MIN_TEST, PWM_MIN_TEST);
  delay(700);
  motorStop();
  delay(500);

  Serial.println("Testing both motors reverse...");
  drive(-PWM_MIN_TEST, -PWM_MIN_TEST);
  delay(700);
  motorStop();
}

void printHelp()
{
  Serial.println();
  Serial.println("DriveWire cursed prototype test menu:");
  Serial.println("t = print telemetry");
  Serial.println("a = pulse Motor A forward/reverse");
  Serial.println("b = pulse Motor B forward/reverse");
  Serial.println("p = pulse both motors forward/reverse");
  Serial.println("f = drive forward");
  Serial.println("r = drive reverse");
  Serial.println("l = turn left");
  Serial.println("x = turn right");
  Serial.println("s = stop/coast");
  Serial.println("k = brake");
  Serial.println("h = help");
  Serial.println();
}

// =====================
// Setup / loop
// =====================

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("DriveWire V1 bring-up test starting...");

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  
  motorStop();

  analogReadResolution(12);
  analogSetPinAttenuation(VBAT_PIN, ADC_11db);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (ina219.begin())
  {
    ina219_ok = true;
    Serial.println("INA219 detected.");
  }
  else
  {
    ina219_ok = false;
    Serial.println("INA219 not detected. Continuing without current sensor.");
  }

  printHelp();
  printTelemetry();

  Serial.println("Ready. Lift the chassis so the wheels can spin freely.");
}

void loop()
{
  if (Serial.available())
  {
    char cmd = Serial.read();

    switch (cmd)
    {
      case 't':
        printTelemetry();
        break;

      case 'a':
        pulseMotorA();
        printTelemetry();
        break;

      case 'b':
        pulseMotorB();
        printTelemetry();
        break;

      case 'p':
        pulseBoth();
        printTelemetry();
        break;

      case 'f':
        Serial.println("Forward");
        drive(PWM_MED_TEST, PWM_MED_TEST);
        break;

      case 'r':
        Serial.println("Reverse");
        drive(-PWM_MED_TEST, -PWM_MED_TEST);
        break;

      case 'l':
        Serial.println("Left turn");
        drive(-PWM_MIN_TEST, PWM_MIN_TEST);
        break;

      case 'x':
        Serial.println("Right turn");
        drive(PWM_MIN_TEST, -PWM_MIN_TEST);
        break;

      case 's':
        Serial.println("Stop/coast");
        motorStop();
        break;

      case 'k':
        Serial.println("Brake");
        motorBrake();
        break;

      case 'A':
        testA();
        break;
      
      case 'B':
        testB();
        break;

      case 'h':
        printHelp();
        break;

      default:
        break;
    }
  }

  // Optional slow telemetry print every 2 seconds while driving
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000)
  {
    lastPrint = millis();
    printTelemetry();
  }
}