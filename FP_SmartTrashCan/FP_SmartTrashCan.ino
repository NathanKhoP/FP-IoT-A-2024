#include <ESP32Servo.h>

#define TRIG_PIN 18
#define ECHO_PIN 19
#define SERVO_PIN 13
#define BUZZER_PIN 25

#define DISTANCE_THRESHOLD 10 // 10 cm
#define FULL_TIME 10000

Servo servo1;
bool trashFull = false;
unsigned long detectionStartTime = 0;

void setup() {
  // init
  Serial.begin(115200);
  servo1.attach(SERVO_PIN);
  servo1.write(0);          // 0 deg
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // turn off

  Serial.println("ESP32 Servo, Ultrasonic Sensor, and Buzzer Setup Complete");
}

void loop() {
  // Check for serial input
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove whitespace or newline

    if (command == "open") {
      for(int posDegrees = 0; posDegrees <= 110; posDegrees++) {
        servo1.write(posDegrees);
        // Serial.println(posDegrees);
        delay(20);
      }
      Serial.println("");
      Serial.println("[Servo: Opened]");
    } else if (command == "close") {
        for(int posDegrees = 110; posDegrees >= 0; posDegrees--) {
          servo1.write(posDegrees);
          // Serial.println(posDegrees);
          delay(20);
        }
        Serial.println("");
        Serial.println("[Servo: Closed]");
      }
    }

  // Check ultrasonic sensor
  long distance = measureDistance();
  // Serial.print("Distance: ");
  // Serial.print(distance);
  // Serial.print(" - | - ");

  if (distance > 0 && distance < DISTANCE_THRESHOLD) {
    if (!trashFull) {
      Serial.print("[Distance: ");
      Serial.print(distance);
      Serial.print("] | ");
      // Start timer if detection begins
      if (detectionStartTime == 0) {
        detectionStartTime = millis();
      } else if (millis() - detectionStartTime >= FULL_TIME) {
        // Trigger alert after 30 seconds
        Serial.println("The trashcan is full.");
        trashFull = true;
        activateBuzzer();
      }
    }
  } else {
    detectionStartTime = 0;
    trashFull = false;
    deactivateBuzzer();
  }
}

long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

void activateBuzzer() {
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("Buzzer Activated");
}
void deactivateBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  // Serial.println("Buzzer Deactivated");
}
