#include <Servo.h>    // Instead of directly changing the PWM value, we'll use Servo library
Servo myservo;  
int angle = 0;

int doPin = 3;                 // output pin number to LED
int riPin = 4;                 
int mePin = 5;                 
int faPin = 6;                 
int soPin = 7;                 
int laPin = 8;                 
int xiPin = 9;          
int servoPin = 10;
int sensorPin = A0;   // voltage input. Use any ADC pin (starts with A)
int sensorValue = 0;  // stores value from ADC


// Define the melody as an array of note pins
int melody[] = {doPin, doPin, soPin, soPin, laPin, laPin, soPin, faPin, faPin, mePin, mePin, riPin, riPin, doPin, soPin, soPin, 
faPin, faPin, mePin, mePin, riPin};
// Define the rhythm (duration of each note)
int noteDurations[] = {1000, 1000, 1000, 1000, 1000, 1000, 2000, 1000, 1000, 1000, 1000, 1000, 1000, 2000};
// Define the correct key input serial according to the melody
// key mapping:
// e: 101, r: 114, t: 116, y: 121, u: 117, i: 105, o: 111
int correctInputs[] = { 101, 101, 117, 117, 105, 105, 117, 121, 121, 116, 116, 114, 114, 101, 117, 117, 121, 121, 116, 116, 114 };
int index = 0;

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;

// score of the piano playing performance
// also the angle of the servo motor (range from 0 to 170)
int score = 90;
int scoreGain = 15;

int minValue = 0;     // sensor value you consider to be minimum
int maxValue = 800;    // sensor value you consider to be maximum
int pressValue = 0;

void setup() {
  // Set all pins as outputs
  pinMode(doPin, OUTPUT);
  pinMode(riPin, OUTPUT);
  pinMode(mePin, OUTPUT);
  pinMode(faPin, OUTPUT);
  pinMode(soPin, OUTPUT);
  pinMode(laPin, OUTPUT);
  pinMode(xiPin, OUTPUT);

  Serial.begin(9600); // Start serial communication at 9600 bps

  // Set the servo control pin
  myservo.attach(servoPin);  
}

void loop() {

  if (index < sizeof(melody)/sizeof(melody[0])) {
    int notePin = melody[index];
    int duration = noteDurations[index];
    digitalWrite(notePin, HIGH); // Turn the LED on

    while (Serial.available() == 0) {    
    }

    int sensorValue = analogRead(sensorPin);   // reads the sensor
    int constrainedValue = constrain(sensorValue, minValue, maxValue);
    pressValue = map(constrainedValue, minValue, maxValue, 100, 255);
    Serial.println(pressValue);  // Prints the value via the serial port
    Serial.flush();
    // delay(duration); // Hold it for the note's duration
    //Play melody only when a signal is received
    int key = Serial.read(); // Read the incoming byte
    // Serial.println(key);
    Serial.flush();

    // compare the key input with the correct key should be played
    if (key == 49) {
      // this is a key down signal, do not need to do any thing

    } else if (key == correctInputs[index]) {
      // turn off this light and move on index
      moveOnWithDebounce(notePin);

      // increase the score
      if (score + scoreGain <= 170) {
        score += scoreGain;
      } else {
        score = 170;
      }

      Serial.print("score: ");
      Serial.println(score);

      // update the servo motor degree according to the new score
      scoreFeedback(score);

    } else {
      // turn off this light and move on index
      moveOnWithDebounce(notePin);

      // decrease the score
      if (score - scoreGain >= 0) {
        score -= scoreGain;
      } else {
        score = 0;
      }

      Serial.print("score: ");
      Serial.println(score);

      // update the servo motor degree according to the new score
      scoreFeedback(score);
    }
    Serial.flush();
  }else{
    index = 0;
  }
}

void moveOnWithDebounce(int notePin) {
  if (millis() - lastDebounceTime > debounceDelay) {
    // If it's a new valid signal, proceed
    digitalWrite(notePin, LOW); // Turn the LED off
    delay(1000); // Pause between notes
    lastDebounceTime = millis(); // Update the last debounce time

    // only increase the index when there is a key signal, rather than the dummy signal
    index++;
    Serial.print("index++: ");
    Serial.println(index);
  }
}

void scoreFeedback(int newscore) {
  // update the servo motor degree according to the new score
  myservo.write(170 - newscore);  // Set servo rotation angle
  delay(500);          // Wait 1 second
}




