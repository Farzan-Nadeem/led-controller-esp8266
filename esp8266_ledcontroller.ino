#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> 

// Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WiFiMulti wifiMulti; 
// Create a webserver object that listens for HTTP request on port 80
ESP8266WebServer server(80); 

#define RED_LED 4
#define GREEN_LED 0
#define BLUE_LED 2

int brightness = 255;
int rBright = 0;
int gBright = 0;
int bBright = 0;

// function prototypes for HTTP handlers
void handleRoot();
void handleNotFound();
void red_up();
void green_up();
void blue_up();
void red_down();
void green_down();
void blue_down();
void bright_up();
void bright_down();
void mode_run();
void color();

void setup(void) {
  delay(1000);
  Serial.begin(9600); 
  
  
  // Start the serial communication to send messages to the computer 
  Serial.println("Starting setup");
  delay(10);
  Serial.println('\n');
  Serial.println("Trying to connect");
  // Add Wi-Fi networks you want to connect to
  wifiMulti.addAP("BELL377", "cogeco0616866");

  Serial.println("Connecting ...");
  WiFi.disconnect();
  IPAddress staticIP(192,168,2,251); 
  IPAddress gateway(192,168,2,1);
  IPAddress subnet(255,255,255,0);

  const char* deviceName = "LEDESP8266";

  WiFi.hostname(deviceName);
  WiFi.config(staticIP, subnet, gateway);
  
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }

  Serial.println('\n');
  Serial.print("Connected to ");
  // Tell us what network we're connected to
  Serial.println(WiFi.SSID()); 
  Serial.print("IP address:\t");
  // Send the IP address of the ESP8266 to the computer
  Serial.println(WiFi.localIP()); 

  // Start the mDNS responder for esp8266.local
  if (MDNS.begin("esp8266")) { 
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/", HTTP_GET, handleRoot);  
  // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"leLogin); 
  server.onNotFound(handleNotFound); 

  // RGB UP functions
  server.on("/red_up", HTTP_GET, red_up);
  server.on("/green_up", HTTP_GET, green_up);
  server.on("/blue_up", HTTP_GET, blue_up);

  // RGB DOWN functions
  server.on("/red_down", HTTP_GET, red_down);
  server.on("/green_down", HTTP_GET, green_down);
  server.on("/blue_down", HTTP_GET, blue_down);

  // Brightness control functions
  server.on("/bright_up", HTTP_GET, bright_up);
  server.on("/bright_down", HTTP_GET, bright_down);

  // Mode change functions
  server.on("/mode", HTTP_GET, mode_run);

  // Color change
  server.on("/color", HTTP_GET, color);

  Serial.println("Starting server");
  // Start the server
  server.begin();

  Serial.println("Starting pins");
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  Serial.println("HTTP server started");
}

void loop() {
  // Listen for HTTP requests from clients
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/html", "you shouldnt be here");
}

void red_up() {
  rBright += 10;

  if (rBright > 255) {
    rBright = 255;
  }

  analogWrite(RED_LED, rBright);
  server.send(200, "text/plain", "ok");
}
void green_up() {
  gBright += 10;

  if (gBright > 255) {
    gBright = 255;
  }
  analogWrite(GREEN_LED, gBright);
  server.send(200, "text/plain", "ok");
}
void blue_up() {
  bBright += 10;

  if (bBright > 255) {
    bBright = 255;
  }

  analogWrite(BLUE_LED, bBright);
  server.send(200, "text/plain", "ok");
}
void red_down() {
  rBright -= 10;
  if (rBright < 0) {
    rBright = 0;
  }

  analogWrite(RED_LED, rBright);
  server.send(200, "text/plain", "ok");
}
void green_down() {
  gBright -= 10;
  if (gBright < 0) {
    gBright = 0;
  }

  analogWrite(GREEN_LED, gBright);
  server.send(200, "text/plain", "ok");
}
void blue_down() {
  bBright -= 10;
  if (bBright < 0) {
    bBright = 0;
  }
  analogWrite(BLUE_LED, bBright);
  server.send(200, "text/plain", "ok");
}

void bright_up() {
  rBright = rBright + (0.75 * (255 - rBright));
  gBright = gBright + (0.75 * (255 - gBright));
  bBright = bBright + (0.75 * (255 - bBright));

  analogWrite(RED_LED, rBright);
  analogWrite(GREEN_LED, gBright);
  analogWrite(BLUE_LED, bBright);

  server.send(200, "text/plain", "ok");
}
void bright_down() {
  rBright *= 0.75;
  gBright *= 0.75;
  bBright *= 0.75;

  analogWrite(RED_LED, rBright);
  analogWrite(GREEN_LED, gBright);
  analogWrite(BLUE_LED, bBright);

  server.send(200, "text/plain", "ok");
}


// For this, we'll have to see if the other events can interrupt this
// If they can then this can be implemented
// Otherwise, this is a no go
// There won't be any way break out from the mode function once it starts running
void mode_run() {
  if (server.arg("mode") == "fade") {
    server.send(200, "text/plain", "ok");
    fade();
  }

  server.send(200, "text/plain", "ok");
}

void color() {
  int red = server.arg("red").toInt();
  int green = server.arg("green").toInt();
  int blue = server.arg("blue").toInt();

  rBright = red;
  gBright = green;
  bBright = blue; 
  analogWrite(RED_LED, rBright);
  analogWrite(GREEN_LED, gBright);
  analogWrite(BLUE_LED, bBright);

  server.send(200, "text/plain", "ok");
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}


// Mode functions 

// Fade 
void fade() { 
    int fadeSpeed = 40;
    int fadeDiff = 5; 

    // Update with breaker variable
    while(true) {

        // Would also check some breaker variable each time within these while blocks
        while(rBright > 0 && rBright < 255) { 
            rBright += fadeDiff;
            analogWrite(RED_LED, rBright); 
        }

        while(gBright > 0 && gBright < 255) {
            gBright += fadeDiff;
            analogWrite(GREEN_LED, gBright); 
        }

        while(bBright > 0 && bBright < 255) { 
            bBright += fadeDiff; 
            analogWrite(BLUE_LED, bBright); 
        }

        fadeDiff = -fadeDiff;
    }
}
