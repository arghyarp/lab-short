#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int tempPin    = A0;
const int trigPin    = 9;
const int echoPin    = 8;
const int warningPin = 13;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(warningPin, OUTPUT);
}

void loop() {
  float rawADC = analogRead(tempPin);
  float voltage = rawADC * (5.0 / 1024.0);
  float tempC = (voltage - 0.5) * 100.0;

  float speedOfSound = 0.03313 + (0.0000606 * tempC);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = (duration * speedOfSound) / 2;

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print((int)tempC);
  lcd.print("C  Dst:");
  lcd.print(distance);
  lcd.print("cm   ");

  lcd.setCursor(0, 1);
  if (tempC >= 75.0 && distance <= 15) {
    digitalWrite(warningPin, HIGH);
    lcd.print("CRIT: BOIL-OVER ");
  } else if (distance <= 10) {
    digitalWrite(warningPin, HIGH);
    lcd.print("ALARM: TANK FULL");
  } else if (tempC >= 70.0) {
    digitalWrite(warningPin, LOW);
    lcd.print("WARN: HIGH TEMP ");
  } else {
    digitalWrite(warningPin, LOW);
    lcd.print("Status: NOMINAL ");
  }

  delay(250);
}

/*
CONNECTIONS:
- LCD 16x2 (I2C): VCC -> 5V, GND -> GND, SDA -> A4, SCL -> A5
- TMP36 Temperature Sensor: Left Pin (Power) -> 5V, Middle Pin (Vout) -> A0, Right Pin (GND) -> GND
- HC-SR04 Ultrasonic Sensor: VCC -> 5V, GND -> GND, Trig -> D9, Echo -> D8
- Alert LED: Anode (+) -> 220 Ohm resistor -> D13, Cathode (-) -> GND

WHAT IT DOES:
- Measures liquid level inside a heated vessel while compensating for thermal shifts in the speed of sound.
- Reads ambient/vapor temperature from the TMP36 (A0) to dynamically compute the exact speed of sound (v = 0.03313 + 0.0000606 * T cm/us), then uses that value to calculate accurate ultrasonic echo distance (D8/D9).
- Displays temperature and headspace distance on the LCD and triggers the D13 alert LED during tank overflow (<= 10 cm) or boil-over conditions (>= 75 C and <= 15 cm).
*/