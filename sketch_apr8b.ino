#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);

bool lock = true;
unsigned long lastUnlockTime = 0;
const unsigned long autoLockDelay = 5000;

void setup() {
  Serial.begin(9600);
  servo.attach(3);
  servo.write(180); // locked position
  lcd.init();
  lcd.backlight();
  SPI.begin();
  rfid.PCD_Init();

  lcd.setCursor(2, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  lcd.setCursor(4, 0);
  lcd.print("Welcome!");
  lcd.setCursor(1, 1);
  lcd.print("Put your card");

  // Send UID if a card is present
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(rfid.uid.uidByte[i], HEX);
      if (i < rfid.uid.size - 1) uid += " ";
    }

    uid.toUpperCase();
    Serial.println(uid);  // Send UID to Python

    rfid.PICC_HaltA();
    delay(1000);
  }

  // Listen for LOCK/UNLOCK commands from Python
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "UNLOCK") {
      servo.write(0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door is open");
      lock = false;
      lastUnlockTime = millis();
    } else if (cmd == "LOCK") {
      servo.write(180);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door is locked");
      lock = true;
    }
  }

  // Auto-lock after delay
  if (!lock && millis() - lastUnlockTime >= autoLockDelay) {
    servo.write(180);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Auto-locking");
    lock = true;
    delay(2000);
    lcd.clear();
  }
}
