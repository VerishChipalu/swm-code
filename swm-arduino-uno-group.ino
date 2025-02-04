#include <Servo.h>

Servo myServo;  // Create a Servo object

void setup() {
  Serial.begin(9600);  // Start Serial Monitor
  myServo.attach(5);   // Attach servo to pin 5
  myServo.write(90);   // Set servo to initial position (Stop)
  Serial.println("Servo Ready. Enter B or C to move.");
}

void loop() {
  if (Serial.available() > 0) {  // Check if data is available
    char command = Serial.read();  // Read the input

    if (command == 'B') {  
      Serial.println("Moving to 120° and returning to 0°");

      // Move forward to ~120°
      myServo.write(120);
      delay(700);  // Adjust for precise movement

      // Stop for stability
      myServo.write(90);
      delay(1000);  

      // Move backward to return to 0°
      myServo.write(60);
      delay(700);  

      // Stop at 0°
      myServo.write(90);
      Serial.println("Returned to 0°");
    } 

    else if (command == 'C') {  
      Serial.println("Moving to 360° and returning to 0°");

      // Move forward for ~360°
      myServo.write(120);
      delay(2400);  

      // Stop for stability
      myServo.write(90);
      delay(1000);  

      // Move backward to return to 0°
      myServo.write(60);
      delay(2400);  

      // Stop at 0°
      myServo.write(90);
      Serial.println("Returned to 0°");
    } 

    else {
      Serial.println("Invalid Input! Enter B or C.");
    }
  }
}
