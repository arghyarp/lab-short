#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int strainPin = A0;
const int flexPin   = A1;

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  int rawStrain = analogRead(strainPin);
  int rawFlex   = analogRead(flexPin);

  int loadPercent = map(rawStrain, 0, 900, 0, 100);
  loadPercent = constrain(loadPercent, 0, 100);

  int deflectionMm = map(rawFlex, 150, 400, 0, 50);
  deflectionMm = constrain(deflectionMm, 0, 50);

  lcd.setCursor(0, 0);
  lcd.print("Ld:");
  lcd.print(loadPercent);
  lcd.print("% ");
  lcd.print("Def:");
  lcd.print(deflectionMm);
  lcd.print("mm  ");

  lcd.setCursor(0, 1);
  if (loadPercent > 80 || deflectionMm > 35) {
    lcd.print("STRUCTURAL FAIL ");
  } else if (loadPercent > 50 || deflectionMm > 20) {
    lcd.print("Status: CAUTION ");
  } else {
    lcd.print("Status: NOMINAL ");
  }

  delay(250);
}

/*
CONNECTIONS:
- LCD 16x2 (I2C): VCC -> 5V, GND -> GND, SDA -> A4, SCL -> A5
- Force Sensor (FSR): Terminal 1 -> 5V, Terminal 2 -> A0 + 10k Ohm resistor to GND
- Flex Sensor: Terminal 1 -> 5V, Terminal 2 -> A1 + 10k Ohm resistor to GND

WHAT IT DOES:
- Simulates a structural load and deflection test bench using piezoresistive sensors.
- Reads analog voltages from the Force Sensor (A0) and Flex Sensor (A1), mapping them to percentage load (0-100%) and structural deflection (0-50 mm).
- Displays live load and deflection metrics on the top row of the LCD and classifies structural integrity on the bottom row (NOMINAL, CAUTION above 50% load or 20 mm bend, and STRUCTURAL FAIL above 80% load or 35 mm bend).
*/