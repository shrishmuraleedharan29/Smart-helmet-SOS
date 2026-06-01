#include <Wire.h>
#include <MPU6050.h>
#include "BluetoothSerial.h"

MPU6050 mpu;
BluetoothSerial SerialBT;

// --- CONFIGURATION ---
const float crashThreshold = 22.0; 
const int buttonPin = 13;      // Button to GPIO 13 and GND
const int BUZZER_PIN = 27;     // Your 2N2222 Base connection

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT); // Initialize the buzzer pin
  
  Wire.begin(21, 22);
  delay(1000); 

  SerialBT.begin("Smart_Helmet_SOS");
  Serial.println("Bluetooth Active. Waiting 2s for power to settle...");
  delay(2000); 

  Serial.println("Initializing MPU6050...");
  mpu.initialize();

  Wire.beginTransmission(0x68);
  if (Wire.endTransmission() == 0) {
    Serial.println("SUCCESS: Sensor found at 0x68. System Online!");
    SerialBT.println("SYSTEM ONLINE: Monitoring for Impact...");
  } else {
    Serial.println("ERROR: Sensor not responding! Check VIN and GND.");
    while (1) {
      // Slow blink buzzer to indicate sensor error
      digitalWrite(BUZZER_PIN, HIGH); delay(500);
      digitalWrite(BUZZER_PIN, LOW);  delay(500);
    }
  }
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float totalForce = sqrt(sq(ax/16384.0) + sq(ay/16384.0) + sq(az/16384.0)) * 9.8;

  if (totalForce > crashThreshold) {
    Serial.println("!!! IMPACT DETECTED !!!");
    SerialBT.println("--- IMPACT DETECTED ---");
    SerialBT.println("You have 10s to CANCEL SOS");

    bool cancelled = false;
    
    // 10 Second Countdown Loop with "Beep-Beep"
    for (int i = 10; i > 0; i--) {
      Serial.print("SOS in: "); Serial.print(i); Serial.println("s");
      SerialBT.print("SOS SENDING IN: "); SerialBT.print(i); SerialBT.println("s");
      
      // --- START BEEP-BEEP PATTERN ---
      digitalWrite(BUZZER_PIN, HIGH); delay(100);
      digitalWrite(BUZZER_PIN, LOW);  delay(100);
      digitalWrite(BUZZER_PIN, HIGH); delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      // -------------------------------

      // Check button frequently during the remaining 700ms of the second
      for(int check = 0; check < 70; check++) {
        if (digitalRead(buttonPin) == LOW) {
          cancelled = true;
          break; 
        }
        delay(10); 
      }
      
      if (cancelled) break;
    }

    if (cancelled) {
      Serial.println("ALARM CANCELLED BY USER.");
      SerialBT.println(">>> SOS CANCELLED BY RIDER <<<");
      // Short low-tone "success" chirp if you want, or just silence
      digitalWrite(BUZZER_PIN, LOW); 
    } else {
      Serial.println("TIMEOUT: SENDING SOS SIGNAL!");
      SerialBT.println("ALARM_TRIGGER"); 
      
      // --- HEAVY ALARM TRIGGER (800ms blasts) ---
      // We'll do 5 heavy blasts before returning to monitoring
      for(int j = 0; j < 5; j++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(800); 
        digitalWrite(BUZZER_PIN, LOW);
        delay(200);
      }
    }
    
    Serial.println("Monitoring Resumed.");
    SerialBT.println("System Monitoring...");
  }
  delay(100);
}