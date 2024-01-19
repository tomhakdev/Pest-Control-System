#include <WiFiNINA.h>

// Replace with your network credentials
const char *ssid = "ArduinoAP";
const char *password = "Password";

// Define pins for motion sensors, LED, and buzzer
const int motionSensor1 = 2;
const int motionSensor2 = 3;
const int motionSensor3 = 4;
//const int ledPin1 = 11; 
//const int ledPin2 = 12;
//const int ledPin3 = 13;
const int buzzerPin = 7;
const int buzzerPin1 = 12;
const int ledPin = 11;


// Set web server port number to 80
WiFiServer server(80);

void setup()
{
  // Initialize the LED pin as an output
  //pinMode(ledPin1, OUTPUT);
  //pinMode(ledPin2, OUTPUT);
  //pinMode(ledPin3, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buzzerPin1, OUTPUT);
  // Initialize the motion sensor pin as an input
  pinMode(motionSensor1, INPUT);
  pinMode(motionSensor2, INPUT);
  pinMode(motionSensor3, INPUT);

  // Start the Serial communication to send messages to the computer
  Serial.begin(9600);

  // Start the WiFi access point
  Serial.println("Setting AP (Access Point)");
  WiFi.beginAP(ssid, password);

  // Wait until the access point starts
  while (WiFi.status() != WL_AP_LISTENING)
  {
    delay(100);
  }

  Serial.println("Access Point Started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("New Client.");
    String currentLine = "";

    // An http request ends with a blank line
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        if (c == '\n')
        {

          // If the current line is blank, send the response
          if (currentLine.length() == 0)
          {
            // Send a standard http response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Web page to display motion status
            client.println("<html><head><meta http-equiv=\"refresh\" content=\"5\"></head><body>");
            client.println("<h1>Arduino Security System</h1>");

            if (motionDetected())
            {
              client.println("<p>Motion detected!</p>");
            }
            else
            {
              client.println("<script>setTimeout(function(){ location.reload(); }, 1000);</script>");
              client.println("<p>No motion detected.</p>");
            }

            client.println("</body></html>");
            client.println();

            // Break out of the while loop
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          // Add every character to the line not part of a new line
          currentLine += c;
        }
      }
    }
    // Close the connection
    client.stop();
    Serial.println("Client Disconnected.");
  }

  // Check for motion detection
  if (motionDetected())
  {
    //digitalWrite(ledPin1, HIGH);
    //digitalWrite(ledPin2, HIGH);
    //digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin, HIGH);
    //analogWrite(ledPin, 255);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(buzzerPin1, HIGH);
  }
  else
  {
    //digitalWrite(ledPin1, LOW);
    //digitalWrite(ledPin2, LOW);
    //digitalWrite(ledPin3, LOW);
    analogWrite(ledPin, 0);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(buzzerPin1, LOW);
  }
}

bool motionDetected() {
  bool sensor1 = digitalRead(motionSensor1) == HIGH;
  bool sensor2 = digitalRead(motionSensor2) == HIGH;
  bool sensor3 = digitalRead(motionSensor3) == HIGH;

  Serial.print("Sensor 1: ");
  Serial.print(sensor1);
  Serial.print(", Sensor 2: ");
  Serial.print(sensor2);
  Serial.print(", Sensor 3: ");
  Serial.println(sensor3);

  return sensor1 || sensor2 || sensor3;
}