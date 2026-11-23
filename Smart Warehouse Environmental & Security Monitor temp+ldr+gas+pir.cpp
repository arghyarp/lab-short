#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int tempPin   = A0;
const int lightPin  = A1;
const int gasPin    = A2;
const int pirPin    = 2;
const int alarmPin  = 13;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(pirPin, INPUT);
  pinMode(alarmPin, OUTPUT);
}

void loop() {
  float voltage = analogRead(tempPin) * (5.0 / 1024.0);
  int tempC = (voltage - 0.5) * 100.0;

  int lightPct = map(analogRead(lightPin), 0, 1023, 0, 100);
  int gasVal   = analogRead(gasPin);
  int motion   = digitalRead(pirPin);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempC);
  lcd.print("C L:");
  lcd.print(lightPct);
  lcd.print("% M:");
  lcd.print(motion == HIGH ? "Y " : "N ");

  lcd.setCursor(0, 1);
  if (gasVal > 400) {
    digitalWrite(alarmPin, HIGH);
    lcd.print("ALARM: GAS LEAK ");
  } else if (tempC > 45) {
    digitalWrite(alarmPin, HIGH);
    lcd.print("ALARM: OVERHEAT ");
  } else if (motion == HIGH && lightPct < 25) {
    digitalWrite(alarmPin, HIGH);
    lcd.print("ALERT: INTRUDER ");
  } else {
    digitalWrite(alarmPin, LOW);
    lcd.print("Gas:");
    lcd.print(gasVal);
    lcd.print(" NOMINAL  ");
  }

  delay(250);
}

/*
CONNECTIONS:
- LCD 16x2 (I2C): VCC -> 5V, GND -> GND, SDA -> A4, SCL -> A5
- TMP36 Temperature Sensor: Left Pin (Power) -> 5V, Middle Pin (Vout) -> A0, Right Pin (GND) -> GND
- Photoresistor (LDR): Terminal 1 -> 5V, Terminal 2 -> A1 + 10k Ohm resistor to GND
- Gas Sensor (MQ-series): Top Pins (A1, H1, A2) -> 5V, Bottom Pin (H2) -> GND, Bottom Pins (B1, B2) -> A2 + 10k Ohm resistor to GND
- PIR Motion Sensor: Power (VCC) -> 5V, Signal (OUT) -> D2, Ground (GND) -> GND
- Alarm LED: Anode (+) -> 220 Ohm resistor -> D13, Cathode (-) -> GND

WHAT IT DOES:
- Monitors warehouse safety, air quality, and after-hours intrusion using four sensors simultaneously.
- Reads temperature (TMP36 on A0), ambient light percentage (LDR on A1), smoke/gas concentration (Gas Sensor on A2), and movement (PIR on D2).
- Displays live temperature, light level, and motion status on the top row of the LCD.
- Evaluates priority safety conditions on the bottom row and activates the D13 alarm LED during gas leaks (Gas > 400), fire/overheat conditions (Temp > 45 C), or unauthorized movement in the dark (Motion detected while Light < 25%).
*/