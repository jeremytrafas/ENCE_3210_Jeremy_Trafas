// display start
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// display end 

// temp start
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// temp end 

// MCU start
#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
#define OUTPUT_READABLE_ACCELGYRO
#define LED_PIN 13
bool blinkState = false;
// MCU end 

#include<avr/wdt.h>

#define ONOFF 2 
#define LOCK 3
#define UNIT 4
#define CAL 5 
#define LIGHT 6
#define LED 9 
#define TIMER1_COMPARE_VALUE 31250 //1s w/ 256 prescale 

// timer counter
volatile unsigned int gCounter = 0;

// flags for ISRs
volatile bool onoffFlag = true; // false off, true on
volatile bool lockFlag = true; // true unlock, false lock

// toggles for rest of buttons 
bool unitToggle = false; // false F, true C 
bool lightToggle = false; // false off, true on 

// variables to track UNIT button state
bool unitButtonPressed = false;
unsigned int unitButtonPressTime = 0;

// temp lock 
float lockedTempF;
float lockedTempC;

// unit button states
enum STATES {
  IDLE,
  PRESS,
  CHANGEUNIT
};

enum STATES STATE = IDLE;

void setup() {
  
  Serial.begin(9600);
  // init I/Os
  pinMode(ONOFF, INPUT);
  pinMode(LOCK, INPUT);
  pinMode(UNIT, INPUT);
  pinMode(LIGHT, INPUT);
  pinMode(CAL, INPUT);
  pinMode(LED, OUTPUT);

  noInterrupts();
  // reset timer regs 
  TCCR1A = 0;
  TCCR1B = 0;

  // compare match 
  OCR1A = TIMER1_COMPARE_VALUE;

  // set prescales 
  TCCR1B |= (1<<CS12);

  // enable timer compare interrupt
  TIMSK1 |= (1<<OCIE1A);
  interrupts();

  // create ISR vector table for onoff and lock
  attachInterrupt(digitalPinToInterrupt(ONOFF),isr_onoff,RISING);
  attachInterrupt(digitalPinToInterrupt(LOCK),isr_lock,RISING);

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
  
  // temp start
  sensors.begin();
  // temp end

  // MPU start
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  pinMode(LED_PIN, OUTPUT);
  // MPU end 
}

void loop() {
  if(onoffFlag){
    // temp start
    sensors.requestTemperatures(); // Send the command to get temperatures
    float tempF = sensors.getTempFByIndex(0);
    float tempC = (tempF-32) * (5.0/9.0);
    // temp end

    // toggle LOCK
    if (lockFlag){ // lock
      lockedTempF = tempF;
      lockedTempC = (lockedTempF-32) * (5.0/9.0); 
    } 
  
    if (digitalRead(LIGHT)){
      lightToggle = !lightToggle;
    }
    
    // toggle LED
    if (lightToggle) { // LED on 
      digitalWrite(LED, HIGH);
    } else { // LED off 
      digitalWrite(LED,LOW);
    }

    // unit button
    switch(STATE){
      case IDLE:
        // wait for button press
        if (digitalRead(UNIT)){
          STATE = PRESS;
        }
        break;
      case PRESS:
        unitButtonPressTime = gCounter;
        // go back to idle if button is released
        if(!digitalRead(UNIT)){
          gCounter = 0;
          STATE = IDLE;
        }
        if(gCounter-unitButtonPressTime > 3){ // if button held for atleast 3 seconds
          gCounter = 0;
          STATE = CHANGEUNIT;
        }
        break;
      case CHANGEUNIT:
        unitToggle = !unitToggle;
        STATE = IDLE;
        break;
    }

    // MPU start
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    // MPU END
    
    // display start
    // clear
    display.clearDisplay();
    // lock
    if (lockFlag){
      display.drawBitmap(0,0, images[1], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    } else {
      display.drawBitmap(0,0, images[0], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    }
    // Temp: text
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(10, 0);
    display.print("Temp");

    // display temp
    display.setTextSize(2);
    display.setCursor(10, 10);
    if (lockFlag){ // if unlocked, display current top
      if(unitToggle == false){ // display F 
        display.print(tempF);
        display.print(" F");
      } else{ // display C
        display.print(tempC);
        display.print(" C");
      }  
    } else {
      if(unitToggle == false){ // display F
        display.print(lockedTempF);
        display.print(" F");
      } else { // display C
        display.print(lockedTempC);
        display.print(" C");
      }
    }
    display.display();
    delay(10);  
    // display end
    //Serial.println(onoffFlag);
    //Serial.println(gCounter);
    //Serial.print(digitalRead(ONOFF));
  } else {
    display.clearDisplay();
    display.display();
    delay(10);
  }
  
  // get gyro position
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // if upright 
  if (ax > 0){
    display.setRotation(0);
  } else { // if upside down
    display.setRotation(2);
  }
}

ISR(TIMER1_COMPA_vect){
  // toggle first 
  gCounter++; // increment counter by 1 every second
}

// isr to turn probe on/off
void isr_onoff() {
  if (digitalRead(ONOFF)){
  //   // if onoff is pressed, toggle onoff flag 
    onoffFlag = !onoffFlag;
  }
}

// isr to lock temp reading
void isr_lock(){
  if (digitalRead(LOCK)){
    // if lock is pressed, toggle onoff flag 
    lockFlag = !lockFlag;
  }
}