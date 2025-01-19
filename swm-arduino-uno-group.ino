#include <Servo.h>
#include <Wire.h>

// Constants
Servo servo1;
Servo servo2;

const int ironSensorPin = 2;        // Pin connected to the iron sensor
const int moisturePin = A0;         // Moisture sensor pin
const int buzzerPin = 6;            // Buzzer pin

const int trigPin = A5;             // Ultrasonic sensor TRIG pin
const int echoPin = A4;             // Ultrasonic sensor ECHO pin

// Variables
int moistureValue = 0;
int state = 0;

void setup() {
  Serial.begin(9600);

  servo1.attach(3);  // Attach first servo
  servo1.write(90);  // Initialize servo1 to 90 degrees (closed)

  servo2.attach(5);  // Attach second servo
  servo2.write(0);   // Initialize servo2 to 0 degrees (home position)

  pinMode(moisturePin, INPUT);   // Moisture sensor
  pinMode(ironSensorPin, INPUT); // Iron sensor
  pinMode(buzzerPin, OUTPUT);    // Buzzer

  pinMode(trigPin, OUTPUT);      // Ultrasonic sensor TRIG pin
  pinMode(echoPin, INPUT);       // Ultrasonic sensor ECHO pin
}

void openServo() {
  servo1.write(180); // Open the terminal (servo1)
  delay(2000);       // 2-second delay for servo movement
}

void closeServo() {
  delay(500);
  servo1.write(90);  // Close the terminal to 90 degrees (servo1)
  delay(2000);       // 2-second delay for servo movement
}

void resetServo2() {
  delay(500);
  servo2.write(0);   // Reset servo2 to 0 degrees
  delay(2000);       // 2-second delay for servo movement
}

// Function to measure distance using the ultrasonic sensor
long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); // Measure the echo time
  long distance = (duration * 0.034) / 2; // Convert to centimeters
  return distance;
}

void loop() {
  long distance = measureDistance(); // Get distance from ultrasonic sensor

  if (distance < 10) { // If an object is closer than 10 cm
    digitalWrite(buzzerPin, HIGH);
    delay(1000); // 1-second delay for buzzer
    digitalWrite(buzzerPin, LOW);

    // Iron Detection Logic
    int ironDetected = digitalRead(ironSensorPin); // Read iron sensor
    delay(1000); // 1-second delay to debounce sensor

    if (ironDetected == LOW) { // If iron is detected
      state = 1;
      Serial.println(1); // Print 1 when iron is detected

      // Perform actions when iron is detected
      digitalWrite(buzzerPin, HIGH);
      delay(1000); // 1-second delay for buzzer
      digitalWrite(buzzerPin, LOW);

      int degrees = 2700;
      int pulseWidth = map(degrees % 360, 0, 360, 544, 2400); // Map to servo's pulse width
      servo2.writeMicroseconds(pulseWidth); // Write pulse width for servo2
      delay(2000);       // 2-second delay for servo movement
      openServo();       // Open the terminal (servo1)
      resetServo2();     // Reset servo2 to 0 degrees
      closeServo();      // Close the terminal (servo1)
      return;            // Return to loop after performing actions
    } else { // Check for wet waste
      moistureValue = analogRead(moisturePin); // Read moisture sensor
      delay(1000); // 1-second delay to avoid false readings

      if (moistureValue < 500) { // If wet waste is detected
        state = 2;
        Serial.println(state);
        digitalWrite(buzzerPin, HIGH);
        delay(1000); // 1-second delay for buzzer
        digitalWrite(buzzerPin, LOW);

        servo2.write(130); // Rotate servo2 to 240 degrees
        delay(2000);       // 2-second delay for servo movement
        openServo();       // Open the terminal (servo1)
        resetServo2();     // Reset servo2 to 0 degrees
        closeServo();      // Close the terminal (servo1)
        return;            // Return to loop after performing actions
      } else { // If neither iron nor wet waste is detected
        state = 3;
        Serial.println(state);
        digitalWrite(buzzerPin, HIGH);
        delay(1000); // 1-second delay for buzzer
        digitalWrite(buzzerPin, LOW);

        delay(2000);       // 2-second delay for servo movement
        openServo();       // Open the terminal (servo1)
        delay(2000);
        closeServo();      // Close the terminal (servo1)
        return;            // Return to loop after performing actions
      }
    }
  } else { // If no object is near
    state = 0;
    Serial.println(state);
    delay(1000); // 1-second delay before the next check
  }
}
