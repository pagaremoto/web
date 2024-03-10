// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials  < MOVISTAR >
const char* ssid = "MOVISTAR_DFF5";
const char* password = "h5NEcs2CHwqd9P3HyVJf";
//
// Replace with your network credentials  < REDMI >
//const char* ssid = "Redmi_ESP32";
//const char* password = "paga1949redmi";


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output1State = "off";
String output2State = "off";
String output3State = "off";
String output4State = "off";


// Assign output variables to GPIO pins
const int output1 = 23;
const int output2 = 22;
const int output3 = 21;
const int output4 = 19;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);

  // Set outputs to LOW
  digitalWrite(output1, LOW);
  digitalWrite(output2, LOW);
  digitalWrite(output3, LOW);
  digitalWrite(output4, LOW);


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            //Encender 4 focos
            // turns the GPIOs on and off
            if (header.indexOf("GET /1/on") >= 0) {
              //1
              Serial.println("Out 1 on");
              output1State = "on";
              digitalWrite(output1, LOW);
              //2
              Serial.println("Out 2 on");
              output2State = "on";
              digitalWrite(output2, LOW);
              //3
              Serial.println("Out 3 on");
              output3State = "on";
              digitalWrite(output3, LOW);
              //4
              Serial.println("Out 4 on");
              output4State = "on";
              digitalWrite(output4, LOW);
            
             //Apagar 4 focos
            } else if (header.indexOf("GET /1/off") >= 0) {
              //1
              Serial.println("Out 1 off");
              output1State = "off";
              digitalWrite(output1, HIGH);
              //2
              Serial.println("Out 2 off");
              output2State = "off";
              digitalWrite(output2, HIGH);
              //3
              Serial.println("Out 3 off");
              output3State = "off";
              digitalWrite(output3, HIGH);
              //4
              Serial.println("Out 4 off");
              output4State = "off";
              digitalWrite(output4, HIGH);
 
              //**
            //} else if (header.indexOf("GET /2/on") >= 0) {
             // Serial.println("Out 2 on");
              //output2State = "on";
              //digitalWrite(output2, HIGH);
            } //else if (header.indexOf("GET /2/off") >= 0) {
              //Serial.println("Out 2 off");
              //output2State = "off";
              //digitalWrite(output2, LOW);
            //}
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Arial; display: inline-block; margin: 10px auto; text-align: center}");
            client.println("</style></head>");
            
            // Web Page Heading
            client.println("<body><h1> <FONT SIZE='5' COLOR='Red'>  Web Server Jardin Tacoronte</h1>");
            
            // Display current state, and ON/OFF buttons for out 1  
            client.println("<p> <FONT SIZE='9' COLOR=#3498DB> Out 1 - Estado " + output1State + "</p>");
            // If the output1State is off, it displays the ON button       
            if (output1State=="off") {
              client.println("<p><a href=\"/1/on\"><button style='width:230px; height:70px; background-color: #4CAF50; border-radius: 20px; color: white; padding: 16px 35px;font-size: 30px; margin: 2px; cursor: pointer' >ENCERNDER</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button style='width:230px; height:70px; background-color: #E74C3C; border-radius: 20px; color: white; padding: 16px 20px;font-size: 30px; margin: 2px; cursor: pointer' >APAGAR</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for out 2  
            //client.println("<p> <FONT SIZE='5' COLOR=#3498DB> Out 2 - State " + output2State +"</p>");
            //// If the output2State is off, it displays the ON button       
            //if (output2State=="off") {
              //client.println("<p><a href=\"/2/on\"><button style='width:100px; height:50px; background-color: #4CAF50; border: none; color: white; padding: 16px 35px;font-size: 20px; margin: 2px; cursor: pointer' >ON</button></a></p>");
            //} else {
              //client.println("<p><a href=\"/2/off\"><button style='width:100px; height:50px; background-color: #E74C3C; border: none; color: white; padding: 16px 20px;font-size: 20px; margin: 2px; cursor: pointer' >OFF</button></a></p>");
            //}
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  
}