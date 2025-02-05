#include <Servo.h>

// Constants
Servo myServo;
Servo servo1;

const int Iron_Sensocr_Pin = 2;        // Pin connected to the iron sensor
const int moisturePin = A0;         // Moisture sensor pin
const int buzzerPin = 6;            // Buzzer pin

const int trigPin = A5;              // Ultrasonic sensor TRIG pin
const int echoPin = A4;              // Ultrasonic sensor ECHO pin

// Variables
int moistureValue = 0;

void setup() {
  Serial.begin(9600);

  servo1.attach(3); 
  servo1.write(0);  // Initialize servo1 to 90 degrees (closed)

  myServo.attach(5); 
  myServo.write(90);   // Initialize servo2 to 0 degrees (home position)

  pinMode(moisturePin, INPUT);   // Moisture sensor
  pinMode(Iron_Sensocr_Pin, INPUT); // Iron sensor
  pinMode(buzzerPin, OUTPUT);    // Buzzer

  pinMode(trigPin, OUTPUT);      // Ultrasonic sensor TRIG pin
  pinMode(echoPin, INPUT);       // Ultrasonic sensor ECHO pin
  
  Serial.println("Smart Dustbin Initialized");
}


void openServo() {
  servo1.write(180); // Open the terminal
  delay(2000);       // 2-second delay for servo movement
}

void closeServo() {
    delay(500); 
  servo1.write(90);  // Close the terminal to 90 degrees
  delay(2000);       // 2-second delay for servo movement
}
 void buzzer(){
    digitalWrite(buzzerPin, HIGH);
    delay(1000); // 1-second delay for buzzer
    digitalWrite(buzzerPin, LOW);
 }
// call the function if iron detected
void iron_detected(){
  myServo.write(120);
      delay(700);   
      myServo.write(90);
      delay(1000);      // 2-second delay for servo movement
      openServo(); 
       myServo.write(60);
      delay(700); 
      myServo.write(90);
      delay(1000);        // Open the terminaL     // Reset servo2 to 0 degrees
      closeServo(); 
}
// call the function if dry detected
void dry_detected(){
 delay(2000);       // 2-second delay for servo movement
        openServo();        // Open the terminal     // Reset servo2 to 0 degrees
        closeServo();
}
// call the function if wet detected
void wet_detected(){

  myServo.write(120);
      delay(2400);   
      myServo.write(90);
      delay(1000);      // 2-second delay for servo movement
      openServo(); 
         myServo.write(60);
      delay(2400); 
      myServo.write(90);
      delay(1000);        // Open the terminaL     // Reset servo2 to 0 degrees
      closeServo(); 

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
    Serial.print("Object detected at: ");
    Serial.print(distance);
    Serial.println(" cm");

   buzzer();
   

    // Iron Detection Logic
    int Iron_Detection_Status = digitalRead(Iron_Sensocr_Pin); // Read iron sensor
    delay(1000); // 1-second delay to debounce sensor
    
    Serial.print("Iron Sensor Value: ");
    Serial.println(Iron_Detection_Status);
     // Debugging

    if (Iron_Detection_Status == LOW) { // If iron is detected
      Serial.println("Iron detected!");
      buzzer();
      iron_detected();
      
         // Close the terminal

    } else { // Check for wet waste
      moistureValue = analogRead(moisturePin); // Read moisture sensor
      delay(1000); // 1-second delay to avoid false readings
      Serial.print("Moisture Value: ");
      Serial.println(moistureValue);
      
      if (moistureValue < 500) { // If wet waste is detected
        Serial.println("Wet waste detected!");
       buzzer();
       wet_detected();

          // Close the terminal
        
      } else { // If neither iron nor wet waste is detected
        Serial.println("Dry waste detected!");
        buzzer();
        dry_detected();

              // Close the terminal
      }
    }

  } else { // If no object is near
    Serial.println("No object near");
    delay(1000);
    }} // 1-second delay before the next check
