// display start
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// display end 

// inits
#define SOLAR
#define BATTERY
#define Vref 2.5

float solarVoltage;
float batteryVoltage;

void setup() {
  Serial.begin(9600);
  // display start 
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  // Clear the buffer
  display.clearDisplay();
  // display end
  // do in real scenario
  //pinMode(SOLAR,INPUT);
  //pinMode(BATTERY,INPUT);
}

void loop() {
  display.clearDisplay();
  // read voltages in real scenario
  // batteryVoltage = analogRead(BATTERY);
  // solarVoltage = analogRead(SOLAR);
  
  // for testing
  batteryVoltage = 1.2;
  solarVoltage = 3;
  if(solarVoltage > Vref){ // if solar is greater than 2.5V, run solar
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(10, 10);
    display.print("Solar Supply");
  } else { // if solar is low
    if (batteryVoltage < solarVoltage){ // connect battery to solar panel
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);
      display.setCursor(10, 10);
      display.print("Charging Battery");
    } else { // battery can power uP
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);
      display.setCursor(10, 10);
      display.print("Battery Supply");
    } 
  }
  display.display();
}
