#include <WiFi.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your desired access point credentials
const char* ssid = "feed-o-pony";
const char* password = "feedopony";

// Desired domain name
const char* domainName = "pony";

// Create an instance of the server
AsyncWebServer server(80);

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Configure the access point
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP: ");
  Serial.println(IP);

  // Set up mDNS responder
  if (!MDNS.begin(domainName)) {
    Serial.println("Error setting up mDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><body>";
    html += "<form action=\"/save\" method=\"post\">";
    html += "Enter an integer: <input type=\"number\" name=\"number\">";
    html += "<button type=\"submit\">Save</button>";
    html += "</form>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  // Handle the form submission
  server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("number", true)) {
      int receivedInteger = request->getParam("number", true)->value().toInt();
      Serial.print("Received integer: ");
      Serial.println(receivedInteger);
      request->send(200, "text/plain", "Integer saved.");
    } else {
      request->send(400, "text/plain", "Invalid request.");
    }
  });

  // Start the server
  server.begin();
}

void loop() {
  // Nothing to do here
}
