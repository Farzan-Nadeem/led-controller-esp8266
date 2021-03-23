#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleLogin();
void handleNotFound(); 

void setup(void){
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
 
  wifiMulti.addAP("BELL377", "cogeco0616866");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("BELL377-5", "cogeco0616866"); 

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());               // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);        // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/login", HTTP_POST, handleLogin); // Call the 'handleLogin' function when a POST request is made to URI "/login"
  server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  // RGB UP functions
  server.on("/red_up", HTTP_POST, red_up);
  server.on("/green_up", HTTP_POST, green_up);
  server.on("/blue_up", HTTP_POST, blue_up);
  
  // RGB DOWN functions
  server.on("/red_down", HTTP_POST, red_down);
  server.on("/green_down", HTTP_POST, green_down);
  server.on("/blue_down", HTTP_POST, blue_down);
  
  // Brightness control functions
  server.on("/bright_up", HTTP_POST, bright_up);
  server.on("/bright_down", HTTP_POST, bright_down);

  // Mode change functions
  server.on("/mode", HTTP_POST, mode); 

  // Color change
  server.on("/color", HTTP_POST, color); 

  server.begin();                            // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();                     // Listen for HTTP requests from clients
}

void handleRoot() {
  server.send(200, "text/html", "you shouldnt be here");
}

void red_up() { 
    // Implement the signal to the LEDS
    server.send(200, "text/plain", "ok");
}
void green_up() { 
    // Implement the signal to the LEDS
    server.send(200, "text/plain", "ok");
}
void blue_up() { 
    // Implement the signal to the LEDS
    server.send(200, "text/plain", "ok");
}
void red_down() { 
    // Implement the signal to the LEDS
    server.send(200, "text/plain", "ok");
}
void green_down() { 
    // Implement the signal to the LEDS
    server.send(200, "text/plain", "ok");
}
void blue_down() { 
    // Implement the signal to the LEDS
    server.send(200, "text/plain", "ok");
}


void bright_up() { 
    // Implement the signal to the LEDS
    server.send(200, "text/plain", "ok");
}
void bright_down() { 
    // Implement the signal to the LEDS
    server.send(200, "text/plain", "ok");
}


void mode() { 
    if(server.arg("mode") == "fade") { 
        Serial.println("Going to start the fadeee");
    }

    server.send(200, "text/plain", "ok")
}


void color() { 
    int red = server.arg("red");
    int green = server.arg("green");
    int blue = server.arg("blue");

    server.send(200, "text/plain", "ok")
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}