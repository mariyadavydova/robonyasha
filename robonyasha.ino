/* 
 *  Plan:
 *  - [ X ] Receiver gets signal in infinite loop (always).
 *  - [ TODO ] LED shows some info (always).
 *  - [ X ] Robonysha goes and stops (on PLAY an EQ).
 *  - [ X ] Robonysha changes speed (on VOL- and VOL+).
 *  - [ TODO ] Rononysha turns head (on PREV and NEXT).
 *  - [ TODO ] Robonysha turns left, right, goes forward (on CH-, CH+, CH).
 *  - [ TODO ] Digital line sensors measure speed (always).
 *  - [ TODO ] Analog line sensors follow line (on 100+).
 *  - [ TODO ] Vision module follows a hand (on 200+).
 *  - [ TODO ] Speed is set up to m/s, not Volts to Pin (always).
 */

#include <IRremote.h>
#include <Servo.h>

// PINS

#define RECV_PIN    11
#define LED_PIN      3

#define SERVO_PIN    8

#define SPEED_L      5 
#define DIR_L        4 
#define SPEED_R      6
#define DIR_R        7

#define LINE_A_L    A0
#define LINE_A_R    A1

#define LINE_D_L    10
#define LINE_D_R     9

#define EYES_MAIN   12
#define EYES_TRIG   13

// DRIVERS
 
IRrecv irrecv(RECV_PIN);
decode_results results;

Servo head;

// STATE

boolean rIsGoing;
int rSpeed;  // min 50, max ???
boolean rIsTurningLeft;
boolean rIsTurningRight;

// UTILITIES 

String decodeCommand(int value) {
  switch(value) {
    case 16753245 : return "ch-";
    case 16736925 : return "ch";
    case 16769565 : return "ch+";

    case 16720605 : return "prev"; 
    case 16712445 : return "next"; 
    case 16761405 : return "play";

    case 16769055 : return "vol-";
    case 16754775 : return "vol+"; 
    case 16748655 : return "eq";

    case 16738455 : return "0";
    case 16750695 : return "100+";
    case 16756815 : return "200+";
    
    default : return "";
  }
}


void setup() {
  Serial.begin(9600);

  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");

  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }

  head.attach(SERVO_PIN);

  rIsGoing = false;
  rSpeed = 50;
  rIsTurningLeft = false;
  rIsTurningRight = false;
}

void loop() {
  if (irrecv.decode(&results)) {
    String command = decodeCommand(results.value);
    Serial.println(command);

    if (command == "play") {
      rIsGoing = true;
    }

    if (command == "eq") {
      rIsGoing = false;
    }

    if (command == "vol-") {
      rSpeed -= 10;
    }

    if (command == "vol+") {
      rSpeed += 10;
    }
    
    irrecv.resume(); // Receive the next value
  }

  if (rIsGoing) {
    digitalWrite(DIR_L, LOW);
    analogWrite(SPEED_L, rSpeed);
    digitalWrite(DIR_R, HIGH);
    analogWrite(SPEED_R, rSpeed);
  } else {
    analogWrite(SPEED_L, 0);
    analogWrite(SPEED_R, 0);
  }
  
  delay(100);
}
