#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int maxVoltageThreshold = 253; // considering rms voltage in normal condition is 230 V
int minVoltageThreshold = 207; // and maximum 10% voltage regulation
int currentThreshold = 15; // considering the current limit of the device is 15 A
int lastVoltage = 0;

const int spikeThreshold = 40;

boolean faultLatched = false;
boolean prevFaultState = false;

const int STATE_MONITOR = 0;
const int STATE_EDIT_VOLTAGE = 1;
const int STATE_EDIT_CURRENT = 2;

int currentState = STATE_MONITOR;
int prevState = -1;

unsigned long lastLCDUpdate = 0;
const unsigned long LCD_INTERVAL = 250;

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("System Ready");

  pinMode(15, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);

  pinMode(4, OUTPUT);
  pinMode(23, OUTPUT);

  digitalWrite(4, HIGH);
  digitalWrite(23, LOW);

  delay(1500);
  lcd.clear();
}

void renderLCDHeaders() {
  lcd.clear();

  if (currentState == STATE_MONITOR) {
    if (faultLatched) {
      lcd.setCursor(0, 0);
      lcd.print("FAULT - LATCHED ");

      lcd.setCursor(0, 1);
      lcd.print("Reset - green ");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("V: ");

      lcd.setCursor(0, 1);
      lcd.print("C: ");
    }
  }
}

void loop() {

  // Mode selection button
  if (digitalRead(0) == LOW) {
    currentState = (currentState + 1) % 3;
    delay(200);
  }

  // Fault reset button
  if (digitalRead(13) == LOW) {
    faultLatched = false;
    renderLCDHeaders();
    delay(200);
  }

  // Update LCD headers when the state changes
  if (currentState != prevState ||
      faultLatched != prevFaultState) {

    renderLCDHeaders();

    prevState = currentState;
    prevFaultState = faultLatched;
  }

  // STATE 0: Monitor voltage and current
  if (currentState == STATE_MONITOR) {

    int rawVoltage = analogRead(34);
    int rawCurrent = analogRead(35);

    int voltageValue = map(rawVoltage, 0, 4095, 0, 300);
    int currentValue = map(rawCurrent, 0, 4095, 0, 30);

    int voltageChange = abs(voltageValue - lastVoltage);

    // Fault detection
    if (voltageValue > maxVoltageThreshold || voltageValue < minVoltageThreshold ||
        voltageChange > spikeThreshold ||
        currentValue > currentThreshold) {

      faultLatched = true;
    }

    // Protection outputs
    if (faultLatched) {
      digitalWrite(4, LOW);
      digitalWrite(23, HIGH);
    } else {
      digitalWrite(4, HIGH);
      digitalWrite(23, LOW);
    }

    // Display readings when no fault is present
    if (!faultLatched &&
        (millis() - lastLCDUpdate >= LCD_INTERVAL)) {

      lastLCDUpdate = millis();

      lcd.setCursor(3, 0);
      lcd.print(voltageValue);
      lcd.print("V   ");

      lcd.setCursor(3, 1);
      lcd.print(currentValue);
      lcd.print("A   ");
    }

    lastVoltage = voltageValue;
  }

  // STATE 1: Edit voltage threshold
  else if (currentState == STATE_EDIT_VOLTAGE) {

    if (digitalRead(2) == LOW) {
      if (maxVoltageThreshold < 300) maxVoltageThreshold += 1;
      delay(150);
    }

    if (digitalRead(15) == LOW) {
      if (minVoltageThreshold > 0) minVoltageThreshold -= 1;
      delay(150);
    }

    if (millis() - lastLCDUpdate >= LCD_INTERVAL) {

      lastLCDUpdate = millis();

      lcd.setCursor(0, 0);
      lcd.print("EDIT VOLTAGE    ");

      lcd.setCursor(0, 1);
      lcd.print("Limit: ");
      lcd.print(minVoltageThreshold);
      lcd.print("-");
      lcd.print(maxVoltageThreshold);
    }
  }

  // STATE 2: Edit current threshold
  else if (currentState == STATE_EDIT_CURRENT) {

    if (digitalRead(2) == LOW) {
      if (currentThreshold < 30) currentThreshold += 1;
      delay(150);
    }

    if (digitalRead(15) == LOW) {
      if (currentThreshold > 0) currentThreshold -= 1;
      delay(150);
    }

    if (millis() - lastLCDUpdate >= LCD_INTERVAL) {

      lastLCDUpdate = millis();

      lcd.setCursor(0, 0);
      lcd.print("EDIT CURRENT    ");

      lcd.setCursor(0, 1);
      lcd.print("Limit: ");
      lcd.print(currentThreshold);
      lcd.print("A   ");
    }
  }

  delay(20);
}
