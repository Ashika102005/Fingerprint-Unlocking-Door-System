#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

#define FINGERPRINT_RX 2
#define FINGERPRINT_TX 3
#define LOCK_PIN 12
#define BUZZER_PIN 4 // Define the buzzer pin

SoftwareSerial mySerial(FINGERPRINT_RX, FINGERPRINT_TX);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal lcd(8, 9, 10, 7, 6, 5);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(LOCK_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT); // Initialize the buzzer pin
  digitalWrite(LOCK_PIN, LOW);
  finger.begin(57600);
  if (!finger.verifyPassword()) {
    Serial.println("Fingerprint sensor not found");
    lcd.print("Fingerprint sensor not found");
    while (1) delay(1);
  }
  lcd.print("   Place finger");
}

void loop() {
  getFingerprintID();
  delay(50);
}

void unlockDoor() {
  digitalWrite(LOCK_PIN, HIGH);
  Serial.println("Door open");
  lcd.clear();
  lcd.print("  Door  Open");
}

void lockDoor() {
  digitalWrite(LOCK_PIN, LOW);
  Serial.println("Door closed");
  lcd.clear();
  lcd.print("   Door Closed");
}

void buzz() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  Serial.println("Wrong Fingerprint.Please wipe your finger!");
  digitalWrite(BUZZER_PIN, LOW);
}

void getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return;
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    unlockDoor();
    delay(5000);
    lockDoor();
  } else {
    lockDoor();
    buzz(); // Activate the buzzer for wrong fingerprint
  }
}
