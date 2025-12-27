#include <DHT.h>
#include <DHT_U.h>
#include <FluxGarage_RoboEyes.h>
#include <IRremote.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RoboEyes<Adafruit_SSD1306> roboEyes(display);

#define DHTPIN A1
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

// Arduino Hardware Specific Pins
#define IRLEDPIN 3  // Arduino pin for IR LED (PWM capable)
#define IRRECPIN 11 // Arduino pin for IR receiver
IRsend irsend;
IRrecv irrecv(IRRECPIN);
decode_results results;

#define TILTPIN 2

unsigned long eventTimer;
int displayState = 0; // 0: face, 1: Temp, 2: Hum
unsigned char displayMood = DEFAULT;

void setup() {
  Serial.begin(9600); // Standard Arduino baud rate

  // Initialize tilt sensor
  pinMode(TILTPIN, INPUT_PULLUP);

  // Initialize Screen
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  dht.begin();

  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);
  roboEyes.setPosition(DEFAULT);
  roboEyes.setSpacebetween(15);
  roboEyes.setWidth(26, 26);
  roboEyes.setHeight(20, 20);
  roboEyes.setIdleMode(ON);
  roboEyes.setCuriosity(ON);
  roboEyes.setAutoblinker(ON, 3, 2);

  delay(200);

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
      }
    }

    // Send the mapped SkyGlass command
    if (skyglassCommand != 0) {
      Serial.print("Sending SkyGlass: 0x");
      Serial.println(skyglassCommand, HEX);
      irsend.sendRC6(skyglassCommand, 32);
      Serial.println();

      // Force display to face mode so we can see the blink
      displayState = 0;
      eventTimer = millis();
      roboEyes.blink(0, 1);
    }

    irrecv.resume();
  }

  if (digitalRead(TILTPIN) == HIGH) {
    roboEyes.setMood(ANGRY);
    roboEyes.setHFlicker(ON);
    roboEyes.setCuriosity(OFF);

    roboEyes.update();
    return;
  }

  sensors_event_t temperaturEvent;
  sensors_event_t humidityEvent;

  if (displayState == 0) {
    roboEyes.setMood(displayMood);
    roboEyes.setHFlicker(OFF);

    roboEyes.update();
  }

  if (millis() - eventTimer > (displayState == 0 ? 10000 : 5000)) {
    displayState++;
    if (displayState > 2)
      displayState = 0;

    dht.temperature().getEvent(&temperaturEvent);
    float temperature = temperaturEvent.temperature;

    dht.humidity().getEvent(&humidityEvent);
    float humidity = humidityEvent.relative_humidity;

    if (isnan(temperature) || isnan(humidity)) {
      roboEyes.anim_confused();
    } else if (displayState == 0) {

      roboEyes.setCuriosity(ON);

      if (temperature > 26) {
        displayMood = DEFAULT;
        roboEyes.setSweat(ON);
      } else if (temperature < 18) {
        displayMood = TIRED;
        roboEyes.setVFlicker(ON);
      } else if (humidity < 30) {
        displayMood = TIRED;
      } else if (humidity > 70) {
        displayMood = TIRED;
      } else {
        displayMood = DEFAULT;
      }

      roboEyes.setMood(displayMood);
    } else {
      roboEyes.setPosition(DEFAULT);

      if (displayState == 1) {
        displayTemperature(temperature);
      } else if (displayState == 2) {
        displayHumidity(humidity);
      }
    }

    eventTimer = millis();
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
  Serial.println(results.decode_type);
  Serial.print("Address: 0x");
  Serial.println(results.address, HEX);
  Serial.print("Command: 0x");
  Serial.println(results.command, HEX);
  Serial.print("Bits: ");
  Serial.println(results.bits);

  if (results.bits == 32) {
    // uint32_t rawValue = (results.address << 16) | results.command;
    // Serial.print("Raw-Data=0x");
    // Serial.println(rawValue, HEX);
  }

  Serial.println();
}