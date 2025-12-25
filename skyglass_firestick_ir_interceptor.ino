#include "Face.h"
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Face face(&display);

const uint16_t kIrLedPin = 4;
const uint16_t kIrRecvPin = 5;

const uint16_t kRecvBufferSize = 1024;

IRsend irsend(kIrLedPin);
IRrecv irrecv(kIrRecvPin, kRecvBufferSize);

decode_results results;

void setup() {
  Serial.begin(115200);

  // Initialize Screen
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();

  delay(200);

  irsend.begin();
  irrecv.enableIRIn();
  Serial.println("Ready - listening for FireStick commands...\n");
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.decode_type == UNKNOWN) {
      irrecv.resume();
      return;
    }

    Serial.print("FireStick Button: ");

    // Map FireStick commands to SkyGlass equivalents
    uint32_t skyglassCommand = 0;

    if (results.decode_type == NEC && results.address == 0x40) {
      Serial.println(results.command, HEX);
      
      face.happy(40, 8);

      if (results.command == 0x12) {
        Serial.println("POWER");
        skyglassCommand = 0xC0081A0C; // SkyGlass Power button
      } else if (results.command == 0x1A) {
        Serial.println("VOLUME UP");
        skyglassCommand = 0xC0081610; // SkyGlass Volume Up
      } else if (results.command == 0x1E) {
        Serial.println("VOLUME DOWN");
        skyglassCommand = 0xC0081611; // SkyGlass Volume Down
      } else if (results.command == 0x10) {
        Serial.println("MUTE");
        skyglassCommand = 0xC008160D; // SkyGlass Mute

        // TODO: Wink
      }
    }

    // Send the mapped SkyGlass command
    if (skyglassCommand != 0) {
      Serial.print("Sending SkyGlass: 0x");
      Serial.println(skyglassCommand, HEX);
      irsend.sendRC6(skyglassCommand, 32);
      Serial.println();
    }

    irrecv.resume();
  } else {
    face.confused(40, 8);
  }

  delay(50);
}

void displayTemperature(float temperature) {
  display.clearDisplay();
  
  // Temperature value
  display.setFont();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 8);
  display.print(temperature, 1);
  
  // Degree symbol and Celsius
  display.setTextSize(1);
  display.setCursor(95, 8);
  display.println("o");
  display.setCursor(105, 10);
  display.setTextSize(2);
  display.println("C");
  
  display.display();
}

void displayHumidity(float humidity) {
  display.clearDisplay();
  
  // Humidityvalue
  display.setFont();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 8);
  display.print(humidity, 1);
  
  // Degree symbol and Celsius
  display.setTextSize(2);
  display.setCursor(100, 16);
  display.println("%");
  
  display.display();
}

void debugRemote() {
  Serial.println("=== BUTTON PRESSED ===");
  Serial.print("Protocol: ");
  Serial.println(typeToString(results.decode_type));
  Serial.print("Address: 0x");
  Serial.println(results.address, HEX);
  Serial.print("Command: 0x");
  Serial.println(results.command, HEX);
  Serial.print("Bits: ");
  Serial.println(results.bits);

  if (results.bits == 32) {
    uint32_t rawValue = (results.address << 16) | results.command;
    Serial.print("Raw-Data=0x");
    Serial.println(rawValue, HEX);
  }

  Serial.println();
}