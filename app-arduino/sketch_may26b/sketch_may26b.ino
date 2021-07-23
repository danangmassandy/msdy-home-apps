#include "ESP8266WiFi.h"

// disable debug mode for serial connection to ATmega328p
//#define APP_DEBUG_MODE = 1

const char* ssid = "BANYUBIRU.Co."; //Enter SSID
const char* password = "Amigdala1212"; //Enter Password

// Set web server port number to 8080
WiFiServer server(8080);

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Set your Static IP address
IPAddress local_IP(192, 168, 7, 77);
// Set your Gateway IP address
IPAddress gateway(192, 168, 7, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const int SERVO_MAX_FEED_ROTATION = 3;

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
  Serial.begin(115200);
  delay(1000);
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    debugPrintln("STA Failed to configure");
  }  
  debugPrintln("Connecting....");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     debugPrint("*");
  }
  debugPrintln("");
  debugPrintln("WiFi connection Successful");
  debugPrint("The IP Address of ESP8266 Module is: ");
  debugPrintln(WiFi.localIP().toString());// Print the IP address
  // start the http server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {                             // If a new client connects,
    debugPrintln("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    String adrAction = "";
    String adrValue1 = "";
    String adrValue2 = "";
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        debugPrint(c);                    // print it out the serial monitor

        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/plain");
            client.println("Connection: close");
            client.println();

            client.println("OK");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else {
            if (currentLine != "") {
              if (currentLine.indexOf("ard_action") != -1) {
                if (currentLine.length() > 12) {
                  adrAction = currentLine.substring(12);
                }
              } else if (currentLine.indexOf("ard_value1") != -1) {
                if (currentLine.length() > 12) {
                  adrValue1 = currentLine.substring(12);
                }
              } else if (currentLine.indexOf("ard_value2") != -1) {
                if (currentLine.length() > 12) {
                  adrValue2 = currentLine.substring(12);
                }                
              }
            }
            // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') { // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Close the connection
    client.stop();
    debugPrintln("Client disconnected.");

    // do processing here
    if (adrAction != "") {
      processAdrAction(adrAction, adrValue1, adrValue2);
    }
  }
}

void processAdrAction(String adrAction, String adrValue1, String adrValue2) {
  debugPrintln("Processing action " + adrAction);
  if (adrAction == "feed") {
    feedMyPet();
  } else if (adrAction == "feedWithRotation") {
    int maxValue = SERVO_MAX_FEED_ROTATION;
    if (adrValue1 != "") {
      maxValue = adrValue1.toInt();
    }
    debugPrint("feedMyPetWithRotation maxValue ");
    debugPrintln(String(maxValue));
    feedMyPetWithRotation(maxValue);
  } else if (adrAction == "reset") {
    resetServoRotation();
  } else if (adrAction == "manualset") {
    manualSetServoRotation(adrValue1);
  }
}

void feedMyPet() {
  for (int i = 0; i < SERVO_MAX_FEED_ROTATION; ++i) {
    debugPrint("Feeding pet i ");
    debugPrintln(String(i));
    feedMyPetOnce();
    delay(1000);
  }
}

void feedMyPetWithRotation(int max) {
  int maxIteration = 12;
  if (max < maxIteration) {
    // set max from input if less than 12 rotation
    maxIteration = max;
  }

  for (int i = 0; i < maxIteration; ++i) {
    debugPrint("Feeding pet i ");
    debugPrintln(String(i));
    feedMyPetOnce();
    delay(1000);
  }
}

void feedMyPetOnce() {
  Serial.println("cmd|feed");
}

void resetServoRotation() {
  Serial.println("cmd|reset");
}

void manualSetServoRotation(String adrValue) {
  Serial.println("cmd|manualset|"+adrValue);
}
