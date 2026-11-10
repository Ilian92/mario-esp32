#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

WiFiClient client;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "ESP32-IlianLeBoss";
const char* password = "motdepasse123!?";
int status = WL_IDLE_STATUS;
bool isGameStarted = false;

WiFiServer server(8080);

void setup() {
  Wire.begin(17, 18);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  display.println("Point d'acces demarre");
  display.print("SSID: ");
  display.print(ssid);
  display.display();
  server.begin();
}

void loop() {
  int connectedStations = WiFi.softAPgetStationNum();
  
  WiFiClient newClient = server.available();
  if (newClient) {
    client = newClient;
  }

  char touche = 0;
  while (client.available()) {
    touche = client.read();
  }
  
  if (isGameStarted == true) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Super Mario ESP !!!");
    display.display();

  } else if (connectedStations > 0) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Users connected: ");
    display.println(connectedStations);
    display.println("");
    display.println("Press Enter to continue");

    if (touche == 'E') {
      isGameStarted = true;
    }

    if (touche != 0) {
      display.setTextSize(2);
      display.setCursor(58, 30);
      display.print(touche);
      display.setTextSize(1);
    }

    display.display();
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("No users connected.");
    display.print("IP: ");
    display.print(WiFi.softAPIP());
    display.display();
  }

  delay(50);
}

