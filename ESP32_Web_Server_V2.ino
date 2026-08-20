#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

const char* password = "YOUR_WIFI_PASSWORD";
const char* ssid = "YOUR_WIFI_NAME";

Adafruit_NeoPixel strip(1, 8, NEO_GRB + NEO_KHZ800);
// Create web server on HTTP port 80
WiFiServer server(80);
// Function to change the LED colors
void setLED(int red, int green, int blue) {
  strip.setPixelColor(0, strip.Color(red, green, blue));
  strip.show();
}

void setup() {
  // Initialize the RGB LED
  strip.begin();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  server.begin();

  Serial.println();
  Serial.println("Connected!");
  Serial.println(WiFi.localIP());

}

void loop() {
  WiFiClient client = server.available();
  if (client) {
        String request;
        char requestbyte;
    while(client.connected()) {
        while(client.available() == 0) {
          delay(500);
        }
        // Read the HTTP request from the browser
        while (client.available()) {
          requestbyte = client.read();
          request +=  requestbyte;
        }
        // Turn the LED ON or OFF and CHANGE THE COLOR based on the requested URL
        if (request.startsWith("GET /led/red")) {
          setLED(255, 0, 0);
          }
        else if(request.startsWith("GET /led/yellow")) {
          setLED(255, 255, 0);
        }
        else if(request.startsWith("GET /led/blue")) {
          setLED(0, 0, 255);
        }
        else if(request.startsWith("GET /led/green")) {
          setLED(0, 255, 0);
        }
        else if(request.startsWith("GET /led/off")) {
          setLED(0, 0, 0);
          }

        // Gather information to display on the webpage
        String html;
        unsigned long uptime;
        uptime = millis() / 1000;

        unsigned long freeMemory;
        freeMemory = ESP.getFreeHeap();

        String chipModel;
        chipModel = ESP.getChipModel();

        unsigned long chipRevision;
        chipRevision = ESP.getChipRevision();

        unsigned long chipCores;
        chipCores = ESP.getChipCores();

        unsigned long cpuFreqMhz;
        cpuFreqMhz = ESP.getCpuFreqMHz();

        int wifiStrength;
        wifiStrength = WiFi.RSSI();

        // Build the HTML Webpage

        Serial.println(request);
        html = "<h1>ESP 32 WEB SERVER</h1> <h2>Hello from my ESP32</h2>";
        html += "<h2>---------------------------------</h2>";
        html += "<h2>SYSTEM INFORMATION</h2>";
        html += " <p>Uptime: ";
        html += uptime;
        html += " seconds </p>";
        html += "<p>Free Memory: ";
        html += freeMemory;
        html += " bytes </p>";
        html += "<p>Chip Model: ";
        html += chipModel;
        html += "</p>";
        html += "<p>Chip Revision: ";
        html += chipRevision;
        html += "</p>";
        html += "<p>Number of Chip Cores: ";
        html += chipCores;
        html += "</p>";
        html += "<p>CPU Frequency: ";
        html += cpuFreqMhz;
        html += " MHz </p>";
        html += "<h2>NETWORK</h2>";
        html += "<p> Wi-Fi Signal: ";
        html += wifiStrength;
        html += " dBm </p>";
        html += "<h2>LED CONTROL</h2>";
        html += "<form action=\"/led/red\" method = \"GET\">  <button type=\"submit\"> Set LED to RED! </button> </form>";
        html += "<form action=\"/led/yellow\" method = \"GET\">  <button type=\"submit\"> Set LED to YELLOW! </button> </form>";
        html += "<form action=\"/led/blue\" method = \"GET\">  <button type=\"submit\"> Set LED to BLUE! </button> </form>";
        html += "<form action=\"/led/green\" method = \"GET\">  <button type=\"submit\"> Set LED to GREEN! </button> </form>";
        html += "<form action=\"/led/off\" method = \"GET\">  <button type=\"submit\"> Turn off LED! </button> </form>";

        // Send the HTTP response and webpage to the browser
        

        client.print("HTTP/1.1 200 OK \r\n");
        client.print("Content-Type: text/html \r\n");
        client.print("Content-Length: ");
        client.print(html.length());
        client.print("\r\n");
        client.print("\r\n");
        client.print(html);
    
    
        client.stop();
        break;
    }
  }


}
