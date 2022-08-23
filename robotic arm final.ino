/*Third version of robotic arm
-Servos run indivually to reduce jittering
-slowServo() function was added to control servo speed
-Capacitors can be added to smooth power flow, and further reduce jittering
to motors*/

#include <Servo.h>

Servo servos[4];

const int arraysize = 100; //maximum array size

int i = 0;  //to keep track of arrrays
int j = 0;

int servo1_array[arraysize]; //used to record arm's position
int servo2_array[arraysize];
int servo3_array[arraysize];
int servo4_array[arraysize];

int potArray[] = {A0, A1, A2, A3};  //potentiometer pins
int analogValueArray[4];  //stores values read from potentiometer
int servoPins[] = {3, 5, 6, 9}; //servo (PWM) pins

const int buttonRecord = 4; //pin for record button
const int buttonPlay = 7; //pin for play button 

const int blueLED = 10; //pin for leds
const int redLED = 11;

void setup() {
  Serial.begin(9600);
  pinMode(buttonRecord, INPUT_PULLUP);  
  pinMode(buttonPlay, INPUT_PULLUP);

  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void slowServo(Servo servo, int angle){ //slows Servo motors
 int starting_pos = servo.read();
 if(angle >= starting_pos){
  for(int i = 0; i < angle - starting_pos; i++){
    servo.write(starting_pos + i);
    delay(15);  //changing the time value will alter the motor's speed
  }
 }
 else {
   for(int i = 0; i < starting_pos - angle; i++){
     servo.write(starting_pos - i);
     delay(15);
  }
 }
}


void runServo(Servo servo, int analogValue, int servopin, int potpin){  //runs an indivdual servo and then detaches it
  //Serial.print("servo connected\n");
  servo.attach(servopin);
  analogValue = analogRead(potpin);
  analogValue = map(analogValue, 0, 1023, 0, 180);
  //Serial.print(analogValue);
  //Serial.print("\n");
  slowServo(servo, analogValue); 
  
  servo.detach();
}

void loop() {

  if(i >= arraysize){ //reset array pointer if it goes out of array bounds
    i = 0;
  } 

  if(digitalRead(buttonRecord) == LOW && digitalRead(buttonPlay) == LOW){ //press both buttons to reset memory 
    Serial.print("reset button pressed\n");
    digitalWrite(redLED, HIGH);
    digitalWrite(blueLED, HIGH);
    for(int k = 0; k < arraysize; k++){
      servo1_array[k] = 0;
      servo2_array[k] = 0;
      servo3_array[k] = 0;
      servo4_array[k] = 0;
    }
    i = 0;
    delay(250);
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, LOW);
    
  }
  
  while(i < arraysize && digitalRead(buttonRecord) == LOW && digitalRead(buttonPlay) == HIGH){ //record button
    //Serial.print("record button is pressed\n");
    digitalWrite(redLED, HIGH);
        
    servo1_array[i] = analogRead(potArray[0]); //record position in array
    servo2_array[i] = analogRead(potArray[1]);
    servo3_array[i] = analogRead(potArray[2]);
    servo4_array[i] = analogRead(potArray[3]);

    servo1_array[i] = map(servo1_array[i], 0, 1023, 0, 180); //scale down to angle
    servo2_array[i] = map(servo2_array[i], 0, 1023, 0, 180);
    servo3_array[i] = map(servo3_array[i], 0, 1023, 0, 180);
    servo4_array[i] = map(servo4_array[i], 0, 1023, 0, 180);

    Serial.print("servo positions");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(servo1_array[i]);
    Serial.print(" ");
    Serial.print(servo2_array[i]);
    Serial.print(" ");
    Serial.print(servo3_array[i]);
    Serial.print(" ");
    Serial.print(servo4_array[i]);
    Serial.print("\n");
    
    i++;

    delay(500);
    digitalWrite(redLED, LOW);
  }

  while(j <= i && digitalRead(buttonPlay) == LOW && digitalRead(buttonRecord) == HIGH){ //play button is pressed
    Serial.print("play button is pressed\n");
    digitalWrite(blueLED, HIGH);

    servos[0].attach(servoPins[0]);
    servos[1].attach(servoPins[1]); 
    servos[2].attach(servoPins[2]); 
    servos[3].attach(servoPins[3]);   //runs each motor indivuallly
    
    //servos[0].write(servo1_array[j]);
    slowServo(servos[0], servo1_array[j]);
    Serial.print(servo1_array[j]);
    Serial.print("  ");
    servos[0].detach();
    
    //servos[1].write(servo2_array[j]);
    slowServo(servos[1], servo2_array[j]);
    Serial.print(servo2_array[j]);
    Serial.print("  ");
    servos[1].detach();

    //servos[2].write(servo3_array[j]);
    slowServo(servos[2], servo3_array[j]);
    Serial.print(servo3_array[j]);
    Serial.print("  ");
    servos[2].detach();
    
    //servos[3].write(servo4_array[j]);
    slowServo(servos[3], servo4_array[j]);
    Serial.print(servo4_array[j]);
    Serial.print("\n");
    servos[3].detach();
    
    j++;
    digitalWrite(blueLED, LOW);
  }
  j = 0;

  for (int k = 0; k < 3; k++){  //run servos if no other button is pressed
    runServo(servos[k], analogValueArray[k], servoPins[k], potArray[k]);
  }
}
