#include <WiFiS3.h>  // This library works with the Arduino R4 WiFi

// Network credentials for the Access Point
const char* ssid = "Robot_AP";    // SSID (name) for your robot's Wi-Fi network
const char* pass = "password123";  // Password for the Wi-Fi network (optional)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

// Motor control pins
const int ENA_PIN = 5;
const int ENB_PIN = 3;
const int IN1_PIN = 7;
const int IN2_PIN = 8;
const int IN3_PIN = 4;
const int IN4_PIN = 2;

// Create a server on port 80
WiFiServer server(80);
int status = WL_IDLE_STATUS;

void setup() {
  // Serial Monitor for debugging
  Serial.begin(9600);
  while (!Serial) {

    ; // wait for serial port to connect. Needed for native USB port only

  }

  // Set up motor control pins
  pinMode(ENA_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  
  // Wait for the Access Point to be ready
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");

    // don't continue

    while (true);
  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {

    Serial.println("Please upgrade the firmware");

  }


  // you can override it with the following:

  WiFi.config(IPAddress(192,48,56,2));


  // print the network name (SSID);

  Serial.print("Creating access point named: ");

  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:

  status = WiFi.beginAP(ssid, pass);

  if (status != WL_AP_LISTENING) {

    Serial.println("Creating access point failed");

    // don't continue

    while (true);

  }

  // wait 10 seconds for connection:

  delay(3000);


  // start the web server on port 80

  server.begin();


  // you're connected now, so print out the status

  printWiFiStatus();
}

void loop() {
  // compare the previous status to the current status

  if (status != WiFi.status()) {

    // it has changed update the variable

    status = WiFi.status();


    if (status == WL_AP_CONNECTED) {

      // a device has connected to the AP

      Serial.println("Device connected to AP");

    } else {

      // a device has disconnected from the AP, and we are back in listening mode

      Serial.println("Device disconnected from AP");

    }

  }

  // Check if there is a client connected
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Process the request and control the motors
    if (request.indexOf("/forward") != -1) {
      moveForward();
    } else if (request.indexOf("/backward") != -1) {
      moveBackward();
    } else if (request.indexOf("/left") != -1) {
      turnLeft();
    } else if (request.indexOf("/right") != -1) {
      turnRight();
    } else if (request.indexOf("/stop") != -1) {
      stopRobot();
    }

    // Send response to client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>Robot Command Received</html>");
    client.stop();
  }
}

// Motor control functions
void moveForward() {
  Serial.println("Moving Forward");
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  analogWrite(ENA_PIN, 255);
  analogWrite(ENB_PIN, 255);
}

void moveBackward() {
  Serial.println("Moving Backward");
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  analogWrite(ENA_PIN, 255);
  analogWrite(ENB_PIN, 255);
}

void turnLeft() {
  Serial.println("Turning Left");
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  analogWrite(ENA_PIN, 255);
  analogWrite(ENB_PIN, 255);
}

void turnRight() {
  Serial.println("Turning Right");
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  analogWrite(ENA_PIN, 255);
  analogWrite(ENB_PIN, 255);
}

void stopRobot() {
  Serial.println("Stopping");
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  analogWrite(ENA_PIN, 0);
  analogWrite(ENB_PIN, 0);
}

void printWiFiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());


  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);


  // print where to go in a browser:

  Serial.print("To see this page in action, open a browser to http://");

  Serial.println(ip);


}