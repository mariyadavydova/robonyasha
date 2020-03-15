/* 
 *  Plan:
 *  - Receiver gets signal in infinite loop (always).
 *  - LED shows some info (always).
 *  - Robonysha goes straight (on PLAY an EQ).
 *  - Robonysha changes speed (on VOL- and VOL+).
 *  - Robonysha turns left and right (on PREV and NEXT).
 *  - Rononysha turns heads (on CH- and CH+).
 *  - Digital line sensors measure speed (always).
 *  - Analog line sensors follow line (on 100+).
 *  - Vision module follows a hand (on 200+).
 */

#include <IRremote.h>
#include <Servo.h>

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

// IR receiver 
IRrecv irrecv(RECV_PIN);
decode_results results;

Servo head;

String decode_command(int value) {
  switch(value) {
    case 16753245 : return "ch-";
    case 16736925 : return "ch";
    case 16769565 : return "ch+";

    case 16720605 : return "prev"; 
    case 16712445 : return "next"; 
    case 16761405 : return "play";

    case 16769055 : return "vol+";
    case 16754775 : return "vol-"; 
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
}

void loop() {
  // head.write(90);
  
  if (irrecv.decode(&results)) {
    String command = decode_command(results.value);
    Serial.println(command);

    // Real stuff here
    if (command == "play") {
      digitalWrite(DIR_L, LOW);
      analogWrite(SPEED_L, 60);
      digitalWrite(DIR_R, HIGH);
      analogWrite(SPEED_R, 60);
    }

    if (command == "eq") {
      analogWrite(SPEED_L, 0);
      analogWrite(SPEED_R, 0);
    }
    
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
