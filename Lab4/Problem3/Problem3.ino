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

// states
enum STATE {SOL,BAT,CHAR};
char gState = SOL;

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
  batteryVoltage = 2;
  solarVoltage = 3;
  switch(gState){
    case SOL: // solar state
      // decide to switch to battery state
      if(solarVoltage < Vref){
        gState = BAT;
      }
      // display state
      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);
      display.setCursor(10, 10);
      display.print("Solar Supply");
      break;
    case BAT: // battery state
      // decide state
      if(solarVoltage >= Vref){ // if solar has returned
        gState = SOL;
      } else if (solarVoltage > batteryVoltage){ // charge if solar is > battery
        gState = CHAR;
      }
      // display state
      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);
      display.setCursor(10, 10);
      display.print("Battery Supply");
      break;
    case CHAR: // charging state   
      if(batteryVoltage > solarVoltage){ // use battery 
        gState = BAT;
      }
      // display state
      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);
      display.setCursor(10, 10);
      display.print("Charging Battery");
      break;
  }
  display.display();
}
