#include<Servo.h>

// disable debug mode for serial connection to ATmega328p
//#define APP_DEBUG_MODE = 1

// SERVO
const int SERVO_PIN = 9;
const int SERVO_INITIAL_ANGLE = 90;
const int SERVO_MOVE_ANGLE = 0;
const int SERVO_MOVE_DELAY_1 = 500;
const int SERVO_MOVE_DELAY_2 = 350;
Servo myServo;
int myServoAngle = SERVO_INITIAL_ANGLE;

String serialResponse = "";

void debugPrintln(String str) {
#ifdef APP_DEBUG_MODE      
    Serial.println(str);
#endif    
}

void debugPrint(String str) {
#ifdef APP_DEBUG_MODE      
    Serial.print(str);
#endif    
}

void debugPrint(char c) {
#ifdef APP_DEBUG_MODE      
    Serial.print(c);
#endif    
}

void setup() {
  // init serial connection
  Serial.begin(115200);
  // connect to SERVO motor
  myServo.attach(SERVO_PIN);
  myServo.write(myServoAngle);
  debugPrintln("Initialization complete");
}

void loop() {
  // loop to check serial connection available
  if (Serial.available()) {
    serialResponse = Serial.readStringUntil('\r\n');
    if (serialResponse.indexOf("cmd|") != -1 && serialResponse.length() > 4) {
      processCmd(serialResponse.substring(4));
    }
  }
}

void processCmd(String cmd) {
  debugPrintln("Processing command: " + cmd);
  if (cmd.indexOf("feed") != -1) {
    feedMyPetOnce();
  } else if (cmd.indexOf("reset") != -1) {
    resetServoRotation();
  } else if (cmd.indexOf("manualset|") != -1) {
    String rotateVal = cmd.substring(10);
    if (rotateVal != "") {
      debugPrintln("Rotate manual val to " + rotateVal);
      setServoRotation(rotateVal.toInt());
    }
  }
}

void feedMyPetOnce() {
  // rotate to second angle to open the gate
  myServoAngle = SERVO_MOVE_ANGLE;
  myServo.write(myServoAngle);
  // 2nd delay
  delay(SERVO_MOVE_DELAY_1);
  // rotate to initial angle first
  myServoAngle = SERVO_INITIAL_ANGLE;
  myServo.write(myServoAngle);
  // 1st delay
  delay(SERVO_MOVE_DELAY_2);

}

void resetServoRotation() {
  // rotate to initial angle first
  myServoAngle = SERVO_INITIAL_ANGLE;
  myServo.write(myServoAngle);
  // 1st delay
  delay(SERVO_MOVE_DELAY_1);
}

void setServoRotation(int val) {
  myServoAngle = val;
  myServo.write(myServoAngle);
  delay(SERVO_MOVE_DELAY_1);
}
