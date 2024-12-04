#include <NewPing.h>

// Motor Pin Definitions
const int motor1Pin1 = 2;
const int motor1Pin2 = 3;
const int motor2Pin1 = 4;
const int motor2Pin2 = 5;
const int ENA = 9;
const int ENB = 10;

// Sensor Pin Definitions
const int leftSensorPin = A2;
const int rightSensorPin = A3;
const int backSensorPin = A5;
const int irSensorPin = A4;
const int trigPin = 6;
const int echoPin = 7;

// Constants
const int lightThreshold = 50;
const int maxDistance = 200;
const int normalSpeed = 100;
const int attackSpeed = 50;
const int turnSpeed = 80;
const int slowSpeed = 40;

// NewPing Object
NewPing sonar(trigPin, echoPin, maxDistance);

// Timing Variables
unsigned long actionStartMillis = 0;
bool performingAction = false;
unsigned long actionDuration = 0;

// Setup
void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  delay(5000); // Startup delay for initialization
}

// Main Loop
void loop() {
  int distance = sonar.ping_cm();
  int leftSensorValue = analogRead(leftSensorPin);
  int rightSensorValue = analogRead(rightSensorPin);
  int backSensorValue = analogRead(backSensorPin);
  int irSensorValue = analogRead(irSensorPin);

  if (!performingAction) {
    if (distance > 0 && distance <= 30) { // Enemy detected
      moveForward(attackSpeed);
      Serial.println("Enemy detected");
    } else { // Default movement
      moveForward(normalSpeed);

      if (leftSensorValue < lightThreshold) { 
        startCombinedAction(1000, "backwardRight");
      } else if (rightSensorValue < lightThreshold) { 
        startCombinedAction(1000, "backwardLeft");
      } else if (backSensorValue < lightThreshold) { 
        startCombinedAction(1000, "forward");
      } else if (irSensorValue > 600 && distance <= 100) { 
        startCombinedAction(500, "turnLeft");
      }
    }
  } else {
    if (millis() - actionStartMillis >= actionDuration) {
      performingAction = false;
      moveForward(slowSpeed); // Resume forward movement
    }
  }
}

// Start a Combined Action
void startCombinedAction(unsigned long duration, String action) {
  performingAction = true;
  actionStartMillis = millis();
  actionDuration = duration;

  if (action == "backwardRight") {
    combinedBackwardRight();
  } else if (action == "backwardLeft") {
    combinedBackwardLeft();
  } else if (action == "forward") {
    moveForward(normalSpeed);
    Serial.println("Moving forward");
  } else if (action == "turnLeft") {
    turnLeft(turnSpeed);
    Serial.println("Turning left");
  } else if (action == "turnRight") {
    turnRight(turnSpeed);
  }
}

// Combined Backward + Right Turn
void combinedBackwardRight() {
  moveBackward(normalSpeed);
  delay(1000); // Move straight backward briefly
  turnRight(turnSpeed);
  delay(600); // Turn right while moving backward
  stopMotors();
  Serial.println("Combined backward-right completed");
}

// Combined Backward + Left Turn
void combinedBackwardLeft() {
  moveBackward(normalSpeed);
  delay(1000); // Move straight backward briefly
  turnLeft(turnSpeed);
  delay(600); // Turn left while moving backward
  stopMotors();
  Serial.println("Combined backward-left completed");
}

// Motor Control Functions
void moveForward(int speed) {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void moveBackward(int speed) {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void turnLeft(int speed) {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void turnRight(int speed) {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}
