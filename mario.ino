#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//niveau1: pièces
//niveau2: pièces + blocs
//niveau3: pièces + blocs + ennemis

WiFiClient client;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "ESP32-IlianLeBoss";
const char* password = "motdepasse123!?";
int status = WL_IDLE_STATUS;
bool isGameStarted = false;
float marioPositionX = 10;
float marioPositionY = 44;
float vitesseY = 0;
bool auSol = true;
const int SOL_Y = 60;
bool goLeft = false;
bool goRight = false;

const unsigned char epd_bitmap_mario_small [] PROGMEM = {
  0xff, 0xff, 0xfc, 0x1f, 0xf8, 0x03, 0xf8, 0xdf, 0xf5, 0xdf, 0xf4, 0xef, 0xf3, 0xc3, 0xff, 0xff, 
  0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xe0, 0x0f, 0xe0, 0x0f, 0xc1, 0x1f, 0xde, 0x3f, 0xfe, 0x1f
};

// const int groundPosition;

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
    movements(touche);
  }
  
  if (isGameStarted == true) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Super Mario ESP !!!");
    delay(50);

    if (goLeft) marioPositionX -= 2.5;
    if (goRight) marioPositionX += 2.5;
    
    if(vitesseY >= 0){
      vitesseY += 2;
    } else {
      vitesseY += 0.5;
    }
    
    marioPositionY += vitesseY;

    if (marioPositionY + 16 >= SOL_Y) {
      marioPositionY = SOL_Y - 16;
      vitesseY = 0;
      auSol = true;
    } else {
      auSol = false;
    }

    display.drawFastHLine(0, SOL_Y, 240, WHITE);
    display.drawBitmap(marioPositionX, marioPositionY, epd_bitmap_mario_small, 16, 16, BLACK, WHITE);
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

void movements(char touche){
  if (touche == 'L') goLeft = true;
  if (touche == 'l') goLeft = false;
  if (touche == 'R') goRight = true;
  if (touche == 'r') goRight = false;
  if (touche == 'U' && auSol) vitesseY = -5;
}

void niveau1(){

}
