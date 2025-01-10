#define LED_PIN 13
#define PORTB5 0x20

// Data direction register port B 
// Data direction is input/output
#define myDDRB (*(volatile uint8_t*) 0x24)

// Data register port B
// LED PIN 13 is connected to Port B on pinout
#define myPORTB (*(volatile uint8_t*) 0x25)

void setup() {
  //pinMode(LED_PIN,OUTPUT);
  myDDRB |= PORTB5;
}

void loop() {
  //digitalWrite(LED_PIN,HIGH);
  myPORTB |= PORTB5;
  delay(1000);
  //digitalWrite(LED_PIN,LOW);
  myPORTB &= ~PORTB5;
  delay(1000);
}
